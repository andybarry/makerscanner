/*
* Copyright 2009-2010, Andrew Barry
*
* This file is part of MakerScanner.
*
* MakerScanner is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License (Version 2, June 1991) as published by
* the Free Software Foundation.
*
* MakerScanner is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "Cameras.h"

// Init camera capture and get ready to start the scanning thread
Cameras::Cameras(wxTextCtrl *pMemo, wxFrame *windowIn, ScanStatus *scanStatusIn, int cameraNumIn)
{
	window = windowIn;
	m_MyCapture = NULL;
	m_LastFrame = NULL;
	m_pMemo = pMemo;
	m_CamPaused = false;
	m_CamPlaying = false;
	cameraNum = cameraNumIn;
//	m_CamStopped = false;
	distanceToReferenceWall = -1;

	scanStatus = scanStatusIn;

	myScanThread = NULL;

	noLaserImage = NULL;
	laserCenteredImage = NULL;

	captureThread = NULL;

	thresholdPixelValue = 25;

	brightnessFilterValue = 0.8;

	// filename format for the first frame from camera 0 is MyFrame0.bmp
	// the 12th frame is MyFrame12.bmp
	m_filename =  wxT("MyFrame");   // This is the base name, integers will be appended for each frame captured
	m_fileformat = wxT(".bmp");	// can choose bmp, jpg, etc for saving images
	m_LastCapturedFrameFilename = wxT("");
	m_FilenameIndex = wxT("");
	m_NumberOfCapturedFrames = 0;

	//cvNamedWindow("My Camera", CV_WINDOW_AUTOSIZE);

	if (InitializeCamera() == true)
	{

		captureThread = new CaptureThread(window, m_MyCapture);
		if ( captureThread->Create() != wxTHREAD_NO_ERROR )
		{
			m_pMemo->AppendText(wxT("\nFailed to create capture thread."));
		} else {
			captureThread->Run();
			captureThread->SetCapture(PREVIEW);
		}
	}
}

// kill the thread and release the camera
Cameras::~Cameras()
{
	StopCaptureThread();

	if (noLaserImage)
	{
		cvReleaseImage(&noLaserImage);
	}

	if (laserCenteredImage)
	{
		cvReleaseImage(&laserCenteredImage);
	}

	if (m_MyCapture) cvReleaseCapture(&m_MyCapture);

}

// Attempt to connect to the camera and grab a frame
bool Cameras::InitializeCamera()
{
	//m_MyCapture = cvCreateCameraCapture(CV_CAP_ANY);
    m_MyCapture = cvCaptureFromCAM(cameraNum);
	if (!m_MyCapture)
	{
		m_pMemo->AppendText(wxT("\nFailed to connect to camera."));
		return false;
	}
	cvQueryFrame(m_MyCapture); // this call is necessary to get correct capture properties

	// TODO: These resolution selection calls don't appear to work right now.
	//cvSetCaptureProperty(m_MyCapture, CV_CAP_PROP_FRAME_WIDTH, 1280);
	//cvSetCaptureProperty(m_MyCapture, CV_CAP_PROP_FRAME_HEIGHT, 960);
	m_FrameHeight   = (int) cvGetCaptureProperty(m_MyCapture, CV_CAP_PROP_FRAME_HEIGHT);
	m_FrameWidth    = (int) cvGetCaptureProperty(m_MyCapture, CV_CAP_PROP_FRAME_WIDTH);
	//m_FrameRate     = (int) cvGetCaptureProperty(m_MyCapture, CV_CAP_PROP_FPS);
	wxString TempString = wxT("\nCamera online at ");
	TempString << m_FrameWidth << wxT("x") <<	m_FrameHeight << wxT(".");
	m_pMemo->AppendText(TempString);

	FrameGrab();

	return true;
}

// Determine if we are OK to capture from the camera
bool Cameras::CaptureExists()
{
	if (!m_MyCapture)
	{
		m_pMemo->AppendText(wxT("\nCamera not initialized."));
		return false;
	}
	return true;
}

// Grab a frame (note that this is NOT the same as the FrameGrab in ScanThread.cpp)

// TODO: remove this
IplImage *Cameras::FrameGrab()
{
	if (!CaptureExists()) return NULL;
   for (int i=0; i < 8; i++) cvGrabFrame(m_MyCapture); // it takes a few images to get to the newest one
   m_LastFrame = cvRetrieveFrame(m_MyCapture);
   //cvShowImage("My Camera", m_LastFrame);
	return m_LastFrame;

}

// Save an image
// TODO: remove this
void Cameras::SaveSingleFrame()
{
	if (!CaptureExists()) return;
	m_NumberOfCapturedFrames++;
	cvSaveImage(GetLastCapturedFrameFilename().char_str(), m_LastFrame);
	//m_pMemo->AppendText(wxChar((char*)GetLastCapturedFrameFilename().c_str()));
	//cvSaveImage("testImage.bmp", m_LastFrame);
}

// TODO: remove this
wxString Cameras::GetLastCapturedFrameFilename()
{
	m_LastCapturedFrameFilename = m_filename;	// basic filename
	m_LastCapturedFrameFilename << m_NumberOfCapturedFrames;
	m_LastCapturedFrameFilename.Append(m_fileformat);
		m_pMemo->AppendText(m_LastCapturedFrameFilename);

	return m_LastCapturedFrameFilename;
}

// Start the scan thread!
void Cameras::StartScan()
{
	if (GetInitialData() == true)
	{
		myScanThread = new ScanThread(window, captureThread, scanStatus, noLaserImage, laserCenteredImage, distanceToReferenceWall);
		if ( myScanThread->Create() != wxTHREAD_NO_ERROR )
		{
			m_pMemo->AppendText(wxT("\nFailed to create scan thread."));
		} else {
			scanStatus->SetScanning(true);
			myScanThread->SetThresholdPixelValue(thresholdPixelValue);
			myScanThread->SetBrightnessThreshold(brightnessFilterValue);
			myScanThread->Run();
		}
	}
}

// Shutdown the thread
void Cameras::StopCaptureThread()
{
	if (captureThread)
	{
		captureThread->SetCapture(STOP);
		captureThread->Wait();
		captureThread = NULL;
	}
}

bool Cameras::GetInitialData()
{
	// pop up the dialog box to ask the user to center the laser or give us the distance
	// to the reference wall.

	DistanceToReferenceDialog distDiag(captureThread, window, wxID_ANY);

	int returnNum = distDiag.ShowModal();

	if (returnNum == wxID_OK)
	{
		distanceToReferenceWall = distDiag.GetWallDistance();
		noLaserImage = distDiag.GetNoLaserImage();

	} else if (returnNum == USER_CENTERED_LASER)
	{

		noLaserImage = distDiag.GetNoLaserImage();
		laserCenteredImage = distDiag.GetLaserCenteredImage();


	} else {
		// cancelled
		return false;
	}
	return true;
}
