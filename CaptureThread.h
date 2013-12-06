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

#ifndef CAPTURE_THREAD_H
#define CAPTURE_THREAD_H

#include <wx/wx.h>

#include "cv.h"
#include "highgui.h"

#include <queue>

using namespace std;

/*
* IDLE = don't capture, but stay alive
* CAPTURE = Try to get every frame, enabling a limited-size buffer
* PREVIEW = Get frames and immediatley send them to the frame via events
* STOP = Leave the thread loop.  Set this right before you Wait() for the thread to exit.
*/
enum CaptureStatus
{
	IDLE,
	CAPTURE,
	PREVIEW,
	STOP
};

BEGIN_DECLARE_EVENT_TYPES()
	DECLARE_EVENT_TYPE(IMAGE_UPDATE_EVENT, -1)
END_DECLARE_EVENT_TYPES()


class CaptureThread : public wxThread
{
	public:
		// Constructor
		CaptureThread(wxFrame *windowIn, CvCapture *captureIn);

		// Function that is run on thread init
		virtual void* Entry();

		// Safely close
		virtual void OnExit();

		IplImage* Pop();

		void SetCapture(CaptureStatus newStatus) { capturing = newStatus; }

		int GetQueueSize() { return imageQueue.size(); }
		void SendFrame(IplImage *frame);

		void Flush();

	private:

		CaptureStatus capturing;

		wxFrame *window;
		CvCapture *cvCapture;

		void CaptureFrame();

		queue<IplImage*> imageQueue;



};



#endif //CAPTURE_THREAD_H
