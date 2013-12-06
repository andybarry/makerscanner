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

#ifndef CAMERAS_H
#define CAMERAS_H

#include <wx/wx.h>

#ifndef OPENCV_INCLUDES
#define OPENCV_INCLUDES
#include "cv.h"
#include "highgui.h"
#endif // OPENCV_INCLUDES


#include "ScanThread.h"
#include "CaptureThread.h"
#include "ScanStatus.h"
#include "DistanceToReferenceDialog.h"

/*
* Init camera capture and starts scanning thread which also supports image preview.
*/

class Cameras
{
	public:
		// pMemo is the text box to output strings to, frame gets events sent to it.
		Cameras(wxTextCtrl *pMemo, wxFrame *windowIn, ScanStatus *scanStatusIn, int cameraNumIn);
		virtual ~Cameras();

		// Attempt to connect to the camera
		bool InitializeCamera();

		// Get a frame
		IplImage* FrameGrab();

		IplImage* GetLastFrame() { return m_LastFrame; }
		void SaveSingleFrame();
		wxString GetLastCapturedFrameFilename();

		// Get a reference to the scan thread
		ScanThread* GetScanThread() { return myScanThread; }

		void SetThresholdPixelValue(int val) { thresholdPixelValue = val; }

		void SetBrightnessFilterValue(float val) { brightnessFilterValue = val; }

		// Start the scan thread
		void StartScan();

		void StopCaptureThread();

		bool GetInitialData();


	private:
		// window to send events to (Cameras doesn't, but ScanThread does send events)
		wxFrame *window;

		// text control to send log events to
		wxTextCtrl *m_pMemo;

		// OpenCV camera capture object
		CvCapture * m_MyCapture;

		// contains the last frame captured
		IplImage *m_LastFrame;

		// contains the object we are scanning without the laser
		IplImage *noLaserImage;

		// contains the laser centered image
		IplImage *laserCenteredImage;

		// ScanThread that does most of the work
		ScanThread *myScanThread;

		CaptureThread *captureThread;

		ScanStatus *scanStatus;

		float distanceToReferenceWall;

		float brightnessFilterValue;

		wxString m_filename, m_CamSpacer, m_fileformat, m_FilenameIndex, m_LastCapturedFrameFilename;
		int m_NumberOfCapturedFrames, m_FrameHeight, m_FrameWidth, m_FrameRate, thresholdPixelValue, cameraNum;
		bool m_CamPaused, m_CamPlaying, m_Camped;
		bool CaptureExists();

};

#endif // CAMERAS_H
