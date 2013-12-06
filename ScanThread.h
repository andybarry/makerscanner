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

#ifndef SCAN_THREAD_H
#define SCAN_THREAD_H

#include <wx/thread.h>
#include <wx/string.h>
#include <wx/textctrl.h>
#include <wx/dialog.h>
#include <wx/wx.h>
#include "cv.h"
#include "highgui.h"
#include "IndividualPixels.h"

#include <wx/dynarray.h>
#include <wx/ffile.h>

#include "PointCloud.h"
#include "CaptureThread.h"
#include "ScanStatus.h"

// include USB code
extern "C"
{
    #include "SetHorz.h"
}

#define MAX_X 640 // TODO: remove this define and auto-detect image size
#define MAX_Y 480 // TODO: remove this define and auto-detect image size

#include <vector>

using namespace std;


// clear event types in the header of the source file
BEGIN_DECLARE_EVENT_TYPES()
	DECLARE_EVENT_TYPE(DISPLAY_TEXT_EVENT, -2)
	DECLARE_EVENT_TYPE(WRITE_TO_FILE_EVENT, -6)
	DECLARE_EVENT_TYPE(SCAN_PROGRESS_EVENT, -7)
	DECLARE_EVENT_TYPE(SCAN_FINISHED_EVENT, -8)
END_DECLARE_EVENT_TYPES()

// Thread that performs image capture and point cloud creation
// Uses events to communicate with GUI thread for frame and disk interactions
class ScanThread : public wxThread
{
	public:
		// Constructor
		ScanThread(wxFrame *windowIn, CaptureThread *captureIn, ScanStatus *scanStatusIn, IplImage *noLaserIn,
			IplImage *laserCenteredIn, float distanceToReferenceIn);

		// Function that is run on thread init
		virtual void* Entry();

		// TODO: Remove
		void SetPixelRange(int Xmin = 0, int Ymin = 0, int Xmax = MAX_X, int Ymax = MAX_Y);

		// set image subtraction threshold value
		void SetThresholdPixelValue(int thres) { minPxVal = thres; }

		float FindReferenceLaser(IplImage *img);

		void SetBrightnessThreshold(float newThreshold) { brightnessThreshold = newThreshold; }

	private:

		// Find the laser by subtracting noLaser (class variable) and withLaser images
		// the compute distances and add to the pointcloud object
		vector<float>* FindLaser2(IplImage *withLaser);

		// send an UpdateImage event to the frame dialog
		//void DisplayImage(IplImage *frame);

		// send a display text event to the frame
		void DisplayText(wxString text);

		// send a write to file event to the frame
		void WriteToFile(wxString str);

		// write the current pointcloud to a file
		void WritePointCloudFile();

		// compute distance to pixel based on width reference pixel position and the laser pixel position
		double PixelToDistance2(float laserCenter, float widthReference);

		// Add a point to the pointcloud, with some filtering
		void AddPointCloudPoint(PointCloudPoint *newPoint, BwImage laserHitPx, BwImage laserFilterPx);

		// point cloud points for storing filtering
		PointCloudPoint *holdingPointBefore;
		PointCloudPoint *holdingPoint;

		// Send a scan progress event to update the frame progress bar
		void SendScanProgress(int laserPos);

		// Send a scan finished event to the frame
		void SendScanFinishedEvent();

		// Safely close
		void OnExit();

		void AddPointcloudPoints(vector<float> *laserPos);

		// give the laser locations, find the reference position in the top 25 pixels
		float GetReferenceLaserLocation(vector<float> *laserCenterPx);

		// Computes the distance to the reference wall based on the member variables
		// noLaser image and laserCentered image, which must already be set when this is called
		float GetDistanceToReferenceWall(vector<float> *laserCenterPx);

		void DisplayLaserPx(vector<float> *laserPx);

		float FindBrightestPointInRow(BwImage subPx, int row, int rowWidth);

		// ---- variables ---- //

		// the frame we send events to
		wxFrame *window;

		// the thread that is running the camera capture
		CaptureThread *captureThread;

		// current distance from flat reference
		double pixelsPerCmOnFlatReference;

		// our current pointcloud
		PointCloud *pointCloud;

		// Object that allows us to communicate between the frame
		ScanStatus *scanStatus;

		// values sent to us from the frame
		int laserIncrement, laserMaxLeft, laserMaxRight, minPxVal;

		// relative amount of brightness needed to be counted as a valid point
		// ranges from 0-1.0.  Valid points have brightness > brightnessThreshold * topBrightness
		float brightnessThreshold;

		IplImage *noLaser, *laserCentered, *noLaserBlur, *coveredImage;

		bool scanning, determinedTargetDistance;
		int m_Xmin, m_Ymin, m_Xmax, m_Ymax;

		// if true, this is the first line we are doing in this scan
		bool m_firstLaserPosition;

		// distance from flat reference in cm
		float distanceFromFlatReference;

};

#endif
