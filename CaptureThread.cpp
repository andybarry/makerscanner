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


#include "CaptureThread.h"

DEFINE_EVENT_TYPE(IMAGE_UPDATE_EVENT)

CaptureThread::CaptureThread(wxFrame *windowIn, CvCapture *captureIn) : wxThread(wxTHREAD_JOINABLE)
{
	capturing = IDLE;
	window = windowIn;
	cvCapture = captureIn;
}

// called on thread quit -- free all memory
void CaptureThread::OnExit()
{

}

// Called when thread is started
void* CaptureThread::Entry()
{
	while (true)
	{
		// check to see if the thread should exit
		if (TestDestroy() == true)
		{
		    break;
		}

		if (capturing == CAPTURE)
		{
			// get a new image
			CaptureFrame();
		} else if (capturing == PREVIEW)
		{

			// get a new image and show it on screen
			CaptureFrame();
			SendFrame(imageQueue.back());
		} else if (capturing == IDLE)
		{
			Sleep(10);
		} else if (capturing == STOP)
		{
		    break;
		}

		Yield();
	}

	return NULL;
}

void CaptureThread::CaptureFrame()
{
	if (!cvCapture){
		//fail
		return;
	}

	if (imageQueue.size() > 100)
	{
		// stack too big, throw out some data
		imageQueue.pop();
	}

	for (int i=0; i < 1; i++) cvGrabFrame(cvCapture); // it takes a few images to get to the newest one
	IplImage* lastFrame = cvRetrieveFrame(cvCapture);
	// cvShowImage("My Camera", LastFrame);
	imageQueue.push(lastFrame);

}

IplImage* CaptureThread::Pop()
{
	if (imageQueue.size() <= 0)
	{
		CaptureFrame();
	}

	IplImage *image = imageQueue.front();

	if (imageQueue.size() > 1)
	{
		imageQueue.pop();
	}

	return image;
}

/*
* Flush the stack, allowing the user to make sure s/he gets the most
* up to date image.  Delete all images in the stack.
*/
void CaptureThread::Flush()
{
	CaptureStatus oldCap = capturing;

	capturing = IDLE;

	while (imageQueue.size() > 0)
	{
		imageQueue.pop();

		// since you should never release an image gotten by cvRetrieveFrame,
		// we don't need to release images here.
	}

	capturing = oldCap;
}

// Display the given image on the frame
// Copies the image so it is safe to change it after the function call
void CaptureThread::SendFrame(IplImage *frame)
{
    if (!frame)
    {
        return;
    }

	IplImage* pDstImg;
	CvSize sz = cvSize(frame->width, frame->height);
	pDstImg = cvCreateImage(sz, 8, 3);
	cvZero(pDstImg);
	// convert the image into a 3 channel image for display on the frame
	if (frame->nChannels == 1)
	{
		//cvCvtColor(frame, pDstImg, CV_GRAY2BGR);

		// another way to convert grayscale to RGB
		cvMerge(frame, frame, frame, NULL, pDstImg);
	} else if (frame->nChannels == 3){

		// opencv stores images as BGR instead of RGB so we need to convert
		cvConvertImage(frame, pDstImg, CV_CVTIMG_SWAP_RB);

	} else {
		// we don't know how to display this image based on its number of channels

		// give up
		cvReleaseImage( &pDstImg );
		return;
	}

	wxCommandEvent event(IMAGE_UPDATE_EVENT, GetId());

	// send the image in the event
	event.SetClientData(pDstImg);

	// Send the event to the frame!
	window->AddPendingEvent(event);
}
