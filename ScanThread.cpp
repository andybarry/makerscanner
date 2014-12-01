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

#include "ScanThread.h"
#include <iostream>
using namespace std;

#include <wx/arrimpl.cpp> // this is a magic incantation which must be done! (that's straight from the docs, by the way)

#include <gsl/gsl_errno.h> // required for interpolation
#include <gsl/gsl_spline.h>

#define PIXELS_PER_CM_PER_CM 519 //564.4 // for the logitech camera, divide this number by the distance from the object to get pixels per cm at that location
#define LASER_FLAT_FORWARD_PRE_MOVE 100
#define LASER_FLAT_FORWARD 124
#define DISPARITY_DISTANCE 19.8   // distance between the camera and the laser mirror in cm

//#define CAMERA_X_MIN (-26.67) // angle in degrees for logitech cam
//#define CAMERA_X_MAX 26.67 // angle in degrees for logitech cam

#define CAMERA_X_MIN (-29.35) // angle in degrees for PS3 eye
#define CAMERA_X_MAX 29.35 // angle in degrees for PS3 eye


//#define CAMERA_Y_MIN (-20) // angle in degrees for logitech cam
//#define CAMERA_Y_MAX 20 // angle in degrees for logitech cam

#define CAMERA_Y_MIN (-24.18) // angle in degrees for PS3 eye
#define CAMERA_Y_MAX 24.18 // angle in degrees for PS3 eye

#define BLUR_AMOUNT 5 // paramter in cvSmooth for CV_GAUSSIAN to blur images before subtraction

#define DEBUG_ON 0 // set to 1 to print warning messages

// define events in the implementation
//DEFINE_EVENT_TYPE(IMAGE_UPDATE_EVENT)
DEFINE_EVENT_TYPE(DISPLAY_TEXT_EVENT)
DEFINE_EVENT_TYPE(WRITE_TO_FILE_EVENT)
DEFINE_EVENT_TYPE(SCAN_PROGRESS_EVENT)
DEFINE_EVENT_TYPE(SCAN_FINISHED_EVENT)

// Init values
ScanThread::ScanThread(wxFrame *windowIn, CaptureThread *captureIn, ScanStatus *scanStatusIn, IplImage *noLaserIn,
	IplImage *laserCenteredIn, float distanceToReferenceIn)// : wxThread(wxTHREAD_JOINABLE)
{

	distanceFromFlatReference = distanceToReferenceIn;
	laserCentered = laserCenteredIn;
	noLaser = noLaserIn;

	noLaserBlur = cvCloneImage(noLaser);

    cvSmooth(noLaser, noLaserBlur, CV_GAUSSIAN, BLUR_AMOUNT);

	coveredImage = cvCloneImage(noLaser);

	scanStatus = scanStatusIn;
	captureThread = captureIn;
	window = windowIn;
	SetPixelRange();
	scanning = true;

	//laserIncrement = 1;
	laserIncrement = 2;
	//laserMaxLeft = 0;
	//laserMaxRight = 150;
	laserMaxLeft = 180;
	laserMaxRight = 220;
	minPxVal = 25;

	pointCloud = NULL;

	holdingPoint = NULL;
	holdingPointBefore = NULL;

	pixelsPerCmOnFlatReference = -1;
	determinedTargetDistance = false;

	brightnessThreshold = 0.8;
}

// called on thread quit -- free all memory
void ScanThread::OnExit()
{
	if (pointCloud)
	{
		delete pointCloud;
	}

	if (coveredImage)
	{
		cvReleaseImage(&coveredImage);
	}

	// noLaser image is released by Cameras.cpp

	// laserCentered image is released by Cameras.cpp

}

// Set pixels we are interested in.
// TODO: make this a selection on the GUI frame
void ScanThread::SetPixelRange(int Xmin, int Ymin, int Xmax, int Ymax)
{
   m_Xmin = Xmin; //50; //Xmin;
   m_Ymin = Ymin; //300; //Ymin;
   m_Xmax = Xmax; //210; //Xmax;
   m_Ymax = Ymax; //420; //Ymax;
}

// Called when thread is started
void* ScanThread::Entry()
{
	captureThread->SetCapture(CAPTURE);

	// set up a pointcloud object
	pointCloud = new PointCloud();

	vector<float> *laserPos;

	IplImage *withLaser;

	// determine a distance from reference
	// we might have a distance from reference from the user, or the laser
	// has been centered
	if (distanceFromFlatReference <= 0)
	{
		// we need to compute the distance to the reference wall.
		// we can use the laserCentered image to do this.

		// first, find the laser in the laserCentered image
		laserPos = FindLaser2(laserCentered);

		// with that data, compute the distance to the wall
		// NOTE: this function also sets pixelsPerCmOnFlatReference which is required for operation
		distanceFromFlatReference = GetDistanceToReferenceWall(laserPos);

		if (distanceFromFlatReference < 0)
		{
		    captureThread->SetCapture(PREVIEW);

            SendScanFinishedEvent();

            return NULL;
		}

		// free memory for the laser-centered laser position vector
		delete laserPos;
	} else
	{
		pixelsPerCmOnFlatReference = PIXELS_PER_CM_PER_CM / distanceFromFlatReference;
	}

	while (scanStatus->GetScanning() == true)
	{
		// check to see if the thread should exit
		if (TestDestroy() == true)	break;

		// get an image from the capture thread

		withLaser = captureThread->Pop();

        if (withLaser)
        {

            // find the laser beam in the image
            laserPos = FindLaser2(withLaser);

            // display the laser position for the user
            DisplayLaserPx(laserPos);

            // compute the point cloud points based on the laser position
            AddPointcloudPoints(laserPos);

            // delete the laser position vector
            delete laserPos;
        }

		// give time for the capture thread to do some things
		Sleep(10);
	}


	// out of the loop for whatever reason, shutdown the thread gracefully

	// write the pointcloud file
	WritePointCloudFile();
	DisplayText(wxT("\nPoint cloud generation complete."));

	captureThread->SetCapture(PREVIEW);

	SendScanFinishedEvent();

	return NULL;
}

// Find the laser based on an image without the laser in it and with the laser in it
// Return an array that is the height of the image with a floating-point sub-pixel value of the laser
// You need to release the vector that gets returned
vector<float>* ScanThread::FindLaser2(IplImage *withLaser)
{
	// subtract the image with the laser in (withLaser) it from the image without the laser (noLaser)
	// to find where the laser is

	IplImage *withLaserBlur = cvCloneImage(withLaser);
	cvSmooth(noLaser, noLaserBlur, CV_GAUSSIAN, BLUR_AMOUNT);

	// copy images so we don't modify given images
	IplImage *noLaserCopy = cvCloneImage(noLaserBlur);
	IplImage *withLaserCopy = cvCloneImage(withLaserBlur);

	// create a single-channel image for processing
	CvSize sz = cvSize(noLaser->width & -2, noLaser->height & -2);
	IplImage *bwNoLaser = cvCreateImage(sz, 8, 1);
	IplImage *bwWithLaser = cvCreateImage(sz, 8, 1);
	IplImage *subImage = cvCreateImage(sz, 8, 1);

	// create the return vector
	vector<float> *pxLocations = new vector<float>(sz.height, -1);

	// convert color images to black and white

	// the cvCvtColor function segfaults on windows.  Not sure why.
	cvCvtColor(noLaserCopy, bwNoLaser,CV_BGR2GRAY);
	cvCvtColor(withLaserCopy, bwWithLaser,CV_BGR2GRAY);

	// subtract the no laser image from the with-laser image
	// if nothing else moved, we should just see where the laser is now
	cvSub(bwWithLaser, bwNoLaser, subImage);

	//captureThread->SendFrame(subImage);

	// set up single-pixel access to the subtracted and original image
	RgbImage noLaserPx(noLaserCopy);
	BwImage subPx(subImage);
	BwImage bwWithLaserPx(bwWithLaser);

	// identify the laser in the top 25 rows
	for (int h=0;h<25;h++)
	{
		(*pxLocations)[h] = FindBrightestPointInRow(subPx, h, sz.width);
	}

	// compute the brightness of those laser hits
	float brightSum = 0, brightAverage = 0;

	for (int h=0;h<25;h++)
	{
		brightSum += bwWithLaserPx[h][  int( (*pxLocations)[h] + 0.5)  ];
	}
	brightAverage = brightSum / 25.0;

	int bestPx;
	float bestSubPx;

	// for loop that loops through every row in the image
	for (int h = 25; h < sz.height; h++)
	{
		bestSubPx = FindBrightestPointInRow(subPx, h, sz.width); //Get subpixel location
		bestPx = int(bestSubPx + 0.5); //Use brightness of closest actual pixel for filtering purposes

		// filter out points that don't show high brightness because they are probably noise
		if (float(bwWithLaserPx[h][bestPx]) > brightnessThreshold * brightAverage)
		{
			(*pxLocations)[h] = bestSubPx;
		} else {
			(*pxLocations)[h] = -1;
		}

	}

	// release images created in this function
	cvReleaseImage(&noLaserCopy);
	cvReleaseImage(&withLaserCopy);
	cvReleaseImage(&bwNoLaser);
	cvReleaseImage(&bwWithLaser);
	cvReleaseImage(&subImage);
	cvReleaseImage(&withLaserBlur);

	return pxLocations;

}

float ScanThread::FindBrightestPointInRow(BwImage subPx, int row, int rowWidth)
{
	//Interpolation mode flag
	//0: Off - use average position of brightest pixels (default)
	//1: Max of graph
	//2: Average position of x's where y=ymax/2
	//3: Intersection of straight lines derived from edges of laser
	int INTER_MODE = 0;
	
	// init values we will use
	int laserSum, laserNum, laserCenter, maxPx, maxPxVal;

	int h = row;

	// for each row, find the location of the laser
	laserSum = 0;
	laserNum = 0;
	maxPx = -1;
	maxPxVal = -1;

	for (int w = 0; w < rowWidth; w++)
	{
		// for each row of pixels, find the brightest point on the laser beam

		// for every pixel in the image, check to see if it is non-zero and thus corresponding
		// to a pixel that is illuminated by the laser

		if (subPx[h][w] > maxPxVal && subPx[h][w] > minPxVal)
		{
			// this point is the brightest we have seen so far and it exceeds our minimum value
			maxPxVal = subPx[h][w];
			maxPx = w;

			laserSum = w;
			laserNum = 1;

		} else if (subPx[h][w] == maxPxVal)
		{
			// this point is exactly as bright as our current brightest pixel, so average them
			laserSum += w;
			laserNum ++;
		}
	}

	// we are done with this row -- select the most likely point.

	if (maxPx >= 0)
	{
		// compute the center point of the laser
		laserCenter = laserSum/laserNum;
	} else {
		// no value for this row
		laserCenter = 0;
	}
		
	//If mode 0, done
	if ( INTER_MODE == 0 ) return laserCenter;
	
	//Interpolation 1 =====
	int interpRange = 30;	//Number of pixels left and right of current laser centre to use for interpolation
	if ( !(maxPx >= 0 && laserCenter > interpRange && rowWidth - laserCenter > interpRange) )
	{
		//Not enough data for interpolation
		return laserCenter;
	}
	
	//Fill array with x, y values to pass to build graph from
	int i;
	double xi, yi, x[interpRange*2], y[interpRange*2];

	for (i = 0; i < 2*interpRange; i++)
	{
		x[i] = laserCenter-interpRange+i;
		y[i] = subPx[h][laserCenter-interpRange+i];
	}

	//Create graph
	gsl_interp_accel *acc = gsl_interp_accel_alloc ();
   	gsl_spline *spline = gsl_spline_alloc (gsl_interp_cspline, interpRange*2);

	gsl_spline_init (spline, x, y, interpRange*2);

	double xMax = 0, yMax = 0, yMin = 0;

	//Find max brightness
    for (xi = x[0]; xi < x[interpRange*2-1]; xi += 0.01)
	{
		yi = gsl_spline_eval (spline, xi, acc);
		if (yi > yMax)
		{
			yMax = yi;
			xMax = xi;
		}
		if (yi < yMin) yMin = yi;
	}
	
	laserCenter = xMax;
	
	//If mode 1, done
	if ( INTER_MODE == 1 )
	{
		//Release gsl objects
		gsl_spline_free (spline);
		gsl_interp_accel_free (acc);
		return laserCenter;
	}
	
	//Interpolation 2 =====
	
	//Find 2 points with same y value on curve, average x values to use as pixel

	double errorTolerance = 2;
	double x1 = -1, x2 = -1;
	double targetY = (yMax + yMin) / 2; //y value to find x values for
	
	//Walk graph left until targetY is found
	yi = yMax;
	xi = xMax;
	while ( abs( yi - targetY ) > errorTolerance && xi > x[1] )
	{
		xi -= 0.01;
		yi = gsl_spline_eval (spline, xi, acc);
	}
	if ( xi == x[1] ) //Invalid
	{
		//Release gsl objects
		gsl_spline_free (spline);
		gsl_interp_accel_free (acc);
		return laserCenter;
	}
	
	x1 = xi;
	
	//Walk graph right until targetY is found
	yi = yMax;
	xi = xMax;
	while ( abs( yi - targetY ) > errorTolerance && xi < x[interpRange*2-2] )
	{
		xi += 0.01;
		yi = gsl_spline_eval (spline, xi, acc);
	}
	if ( xi == x[interpRange*2-2] ) return laserCenter;
	
	x2 = xi;
	
	if ( x2 < 0 ) //Invalid
	{
		//Release gsl objects
		gsl_spline_free (spline);
		gsl_interp_accel_free (acc);
		return laserCenter;
	}
	laserCenter = ( x1 + x2 ) / 2;
	
	//If mode 2, done
	if ( INTER_MODE == 2 )
	{
		//Release gsl objects
		gsl_spline_free (spline);
		gsl_interp_accel_free (acc);
		return laserCenter;
	}
	
	//Interpolation 3 =====
	double offset = 5;
	if ( x1 - offset < x[0] || x2 + offset > x[interpRange*2-1] ) //Invalid
	{
		//Release gsl objects
		gsl_spline_free (spline);
		gsl_interp_accel_free (acc);
		return laserCenter;
	}
	
	//Find straight lines of the edges of the laser curve, use point of intersection as pixel
	
	//Pick 2 points on each side to calculate lines a & b
	double x1a = x1 - offset;
	double x1b = x1 + offset;
	double x2a = x2 - offset;
	double x2b = x2 + offset;
	double y1a = gsl_spline_eval (spline, x1a, acc);
	double y1b = gsl_spline_eval (spline, x1b, acc);
	double y2a = gsl_spline_eval (spline, x2a, acc);
	double y2b = gsl_spline_eval (spline, x2b, acc);
	
	//Find line equations
	double m1 = ( y1b - y1a ) / ( x1b - x1a );
	double m2 = ( y2b - y2a ) / ( x2b - x2a );
	
	double c1 = y1a - m1 * x1a;
	double c2 = y2a - m2 * x2a;
	
	//Find intersection
	double intersection_X = (c2 - c1) / (m1 - m2);
	//Only update if value is within valid range
	if ( intersection_X >= 0 && intersection_X < rowWidth ) laserCenter = intersection_X;
	
	//Release gsl objects
	gsl_spline_free (spline);
	gsl_interp_accel_free (acc);
		
	return laserCenter;
}

// Compute where the 3d points are based on where the laser was detected in the image
void ScanThread::AddPointcloudPoints(vector<float> *laserPos)
{
	// compute distance for this pixel
	// calculate phi from camera parameters and Y pixel location
	// (assume dimensions of noLaser are the same as all other images)

	float widthReference = GetReferenceLaserLocation(laserPos);

	if (widthReference < 0)
	{
		// we don't know where the refernce laser is -- abort
		return;
	}

	float laserCenter;

	int r, g, b;
	double pxDist, theta, phi;

	// set up single-pixel access to the subtracted and original image
	RgbImage noLaserPx(noLaser);
	int h = 0;
	for (h=0;h<int(laserPos->size());h++)
	{
		laserCenter = (*laserPos)[h];

		// check to make sure that we have data for this row
		if (laserCenter >= 0)
		{

			phi = (noLaser->height/2 - h) * double(CAMERA_Y_MAX - CAMERA_Y_MIN)/double(noLaser->height);

			// convert to radians
			phi = phi * 3.14159 / 180.0;

			pxDist = PixelToDistance2(laserCenter, widthReference);

			if (pxDist < 0)
			{
				// not a valid point
				continue; //Ignore this point, keep processing the rest
			}


			// compute theta based on laserCenter
			theta = (laserCenter - noLaser->width/2) * double(CAMERA_X_MAX - CAMERA_X_MIN)/double(noLaser->width);

			// convert to radians
			theta = theta * 3.14159 / 180.0;

			r = noLaserPx[h][int(laserCenter + 0.5)].r;
			g = noLaserPx[h][int(laserCenter + 0.5)].g;
			b = noLaserPx[h][int(laserCenter + 0.5)].b;

//			if (h == 100)
//			{
//				wxString tmp = wxT("\ndiff: ");
//				tmp << widthReference - laserCenter;
//				tmp += wxT(" widthR: ");
//				tmp << widthReference;
//				tmp += wxT(" laserCenter: ");
//				tmp << laserCenter;
//				DisplayText(tmp);
//			}

			pointCloud->AddPoint(pxDist, theta, phi, r, g, b, laserCenter, h, widthReference);
		}
	}
}

// send the pointcloud string to the frame for writing to disk
void ScanThread::WritePointCloudFile()
{
	WriteToFile(pointCloud->GetPointCloudPly());
}

// send some text to be displayed
void ScanThread::DisplayText(wxString text)
{
	wxCommandEvent event2(DISPLAY_TEXT_EVENT, GetId());
	event2.SetString(text);
	window->AddPendingEvent(event2);
}

// compute distance to a point given the x pixel location and the width reference x pixel location
double ScanThread::PixelToDistance2(float laserCenter, float widthReference)
{
	// TODO: make generic for other sized cameras

	if (laserCenter > widthReference + 20)
	{
		// the detected laser appears to be to the right of the width reference.
		// this is almost certainly noise and will result in crazy distance calculations
		return -1;
	} else if (laserCenter > widthReference)
	{
		// might just be a slightly tilted laser
		laserCenter = widthReference;
	}

	double l = (widthReference) / pixelsPerCmOnFlatReference;
	double lPlusM = (laserCenter) / pixelsPerCmOnFlatReference;
	double m = l - lPlusM; // m is the object position projected on the flat reference background

	//double distFromRef = m * m_DistanceFromFlatReference / (DISPARITY_DISTANCE + m);

	double ret = distanceFromFlatReference - m * distanceFromFlatReference / (DISPARITY_DISTANCE + m);

	if (ret > 100)
	{
		DisplayText(wxT("\nWarning: large distance detected."));
	}

	return ret;

}

// Write a string to disk via an event to the frame
void ScanThread::WriteToFile(wxString str)
{
	wxCommandEvent event(WRITE_TO_FILE_EVENT, GetId());
	event.SetString(str);
	window->AddPendingEvent(event);
}

// Send an event with the progress of the laser
void ScanThread::SendScanProgress(int laserPos)
{
	// compute percentage done
	int progress = int( 100.0 * float(laserPos - laserMaxLeft)/(float(laserMaxRight - laserMaxLeft)) + 0.5);

	wxCommandEvent event2(SCAN_PROGRESS_EVENT, GetId());
	event2.SetInt(progress);
	window->AddPendingEvent(event2);
}

// Send a scan finished event
void ScanThread::SendScanFinishedEvent()
{
	wxCommandEvent event2(SCAN_FINISHED_EVENT, GetId());
	window->AddPendingEvent(event2);
}

float ScanThread::GetReferenceLaserLocation(vector<float> *laserCenterPx)
{
	// find the laser in the top 25 pixels in the image

	// average the top 25 pixel locations to find the center
	float refSum = 0;
	int numRefHits = 0;

	for (int h=0;h<25;h++)
	{
		// this means that we don't accept the laser line on the exact left edge of the image
		if ((*laserCenterPx)[h] > 0)
		{
			refSum += (*laserCenterPx)[h];
			numRefHits ++;
		}
	}

	if (numRefHits < 10)
	{
		// we probably don't have a good idea where the laser is
		if (DEBUG_ON == 1)
		{
			DisplayText(wxT("\nWarning: failed to find reference laser."));
		}

		return -1;
	}

	float refCenter = refSum/float(numRefHits);

	return refCenter;
}

// NOTE: this function also sets pixelsPerCmOnFlatReference which is required for operation
float ScanThread::GetDistanceToReferenceWall(vector<float> *laserCenterPx)
{
	// first, find the laser in the top 25 pixels in the image
	float refCenter = GetReferenceLaserLocation(laserCenterPx);

	if (refCenter < 0)
	{
		// failed to find reference laser.
		DisplayText(wxT("\nError: failed to compute distance to target.  Most likely didn't get a clear enough picture of the laser line.  Aborting."));
		return -1;
	}

	// compute the distance to the target based on camera parameters and refCenter
	// refCenter holds the number of pixels the center is from the left of the image.
	// For this computation, we want to know the number of pixels from the camera's center -- convert
	float pixelsFromCameraCenter = (laserCentered->width / 2) - refCenter;

	distanceFromFlatReference = 0;

	pixelsPerCmOnFlatReference = pixelsFromCameraCenter / float(DISPARITY_DISTANCE);

	distanceFromFlatReference +=  float(PIXELS_PER_CM_PER_CM) / pixelsPerCmOnFlatReference;

	wxString strDist = wxT("\nEstimated distance from flat reference = ");
	wxString numstr;
	numstr.Printf(wxT("%.0f"), distanceFromFlatReference);
	strDist += numstr + wxT("cm");

	DisplayText(strDist);

	return distanceFromFlatReference;

}

void ScanThread::DisplayLaserPx(vector<float> *laserPx)
{
	// create an image that we can use to display where we found the laser
	CvSize sz = cvSize(noLaser->width & -2, noLaser->height & -2);

	// allow for single-pixel access to the laserHitImage
	RgbImage coveredImagePx(coveredImage);

	// make the current laser line red
	IplImage *outImage = cvCloneImage(coveredImage);
	RgbImage outImagePx(outImage);

	for (int h=0;h<int(laserPx->size());h++)
	{
		// add this point to our display image for where we found laser points (for the future)
		coveredImagePx[h][int((*laserPx)[h])].r = 0;
		coveredImagePx[h][int((*laserPx)[h])].g = 0;
		coveredImagePx[h][int((*laserPx)[h])].b = 255;

		// add the laser line in red for display right now
		outImagePx[h][int((*laserPx)[h])].r = 255;
		outImagePx[h][int((*laserPx)[h])].g = 0;
		outImagePx[h][int((*laserPx)[h])].b = 0;

	}

	// display the image (SendFrame copies the image, so we can release it here).
	//captureThread->SendFrame(pDstImg);
	captureThread->SendFrame(outImage);

	// release images used for display image
	cvReleaseImage(&outImage);

}
