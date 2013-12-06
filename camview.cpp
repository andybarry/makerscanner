////////////////////////////////////////////////////////////////////
// Name:		implementation of the CCamView class
// File:		camview.cpp
// Purpose:		eye/camera view/GUI system control methods
//
// Created by:	Larry Lart on 22-Feb-2006
// Updated by:	Andrew Barry Jan 2010
//
// Copyright:	(c) 2006 Larry Lart
////////////////////////////////////////////////////////////////////


// on windows, you must include the wx headers before
// other headers - Andy
#include <wx/image.h>


// main header -- this has wx headers in it so make
// sure to include it before other headers - Andy
#include "camview.h"


// system header
#include <math.h>
#include <stdio.h>
#include "cv.h"
#include "highgui.h"


#ifdef __GNUG__
#pragma implementation
#pragma interface
#endif

// implement message map
BEGIN_EVENT_TABLE(CCamView, wxWindow)
	EVT_PAINT( CCamView::OnPaint )
	EVT_SIZE( CCamView::OnSize )
END_EVENT_TABLE()

////////////////////////////////////////////////////////////////////
// Method:	Constructor
// Class:	CCamView
// Purose:	build my CCamView object
// Input:	nothing
// Output:	nothing
////////////////////////////////////////////////////////////////////
CCamView::CCamView( wxWindow *frame, const wxPoint& pos, const wxSize& size ):
			wxWindow(frame, -1, pos, size, wxSIMPLE_BORDER )
{
	//m_pCamera = NULL;

	// set my canvas width/height
	m_nWidth = size.GetWidth( );
	m_nHeight = size.GetHeight( );

	m_bDrawing = false;

	m_bNewImage = 0;

	m_pBitmap = NULL;

}

////////////////////////////////////////////////////////////////////
// Method:	Destructor
// Class:	CCamView
// Purose:	destroy my object
// Input:	nothing
// Output:	nothing
////////////////////////////////////////////////////////////////////
CCamView::~CCamView( )
{
	//m_pCamera = NULL;
}

////////////////////////////////////////////////////////////////////
// Method:	Is Capture Enabled
// Class:	CCamView
// Purose:	check if camera is initialized
// Input:	nothing
// Output:	bool yes/no
////////////////////////////////////////////////////////////////////
bool CCamView::IsCaptureEnabled( )
{
//	return( m_pCamera->IsInitialized( ) );
	return( 1 );
}

////////////////////////////////////////////////////////////////////
// Method:	OnPaint
// Class:	CCamView
// Purose:	on paint event
// Input:	reference to paint event
// Output:	nothing
////////////////////////////////////////////////////////////////////
void CCamView::OnPaint( wxPaintEvent& event )
{
	wxPaintDC dc(this);
	Draw( dc );
}

////////////////////////////////////////////////////////////////////
// Method:	Draw
// Class:	CCamView
// Purose:	camera drawing
// Input:	reference to dc
// Output:	nothing
////////////////////////////////////////////////////////////////////
void CCamView::Draw( wxDC& dc )
{
	// check if dc available
	if( !dc.Ok( ) || m_bDrawing == true ){ return; }

		m_bDrawing = true;

		int x,y,w,h;
		dc.GetClippingBox( &x, &y, &w, &h );
		// if there is a new image to draw
		if( m_bNewImage )
		{
			dc.DrawBitmap( *m_pBitmap, x, y );
			m_bNewImage = false;
		} else
		{
			// draw inter frame ?
		}

		m_bDrawing = false;

	return;
}

////////////////////////////////////////////////////////////////////
// Method:	OnDraw
// Class:	CCamView
// Purose:	CCamView drawing
// Input:	nothing
// Output:	nothing
////////////////////////////////////////////////////////////////////
void CCamView::DrawCam( IplImage* pImg )
{
//	return;
	if( m_bDrawing ) return;
	m_bDrawing = true;
	// if there was an image then we need to update view
    if( pImg )
    {
        // copy the image (will be deleted after display)
		IplImage *pDstImg = pImg;//cvCloneImage(pImg);

		int nCamWidth = pImg->width;//m_pCamera->m_nWidth;
		int nCamHeight = pImg->height;//m_pCamera->m_nHeight;


		// draw a vertical line through the center of the image
		cvLine(pDstImg, cvPoint(nCamWidth/2, 0), cvPoint(nCamWidth/2, nCamHeight), CV_RGB( 0,255,0 ));

		// draw a horizontal line at pixel 25
		cvLine(pDstImg, cvPoint(0, 25), cvPoint(nCamWidth, 25), CV_RGB( 0,255,0 ));

		// draw a horizontal line through the center of the image
		//cvLine(pDstImg, cvPoint(0, nCamHeight/2), cvPoint(nCamWidth, nCamHeight/2), CV_RGB( 0,255,0 ));

		// process image from opencv to wxwidgets
		unsigned char *rawData;
		// draw my stuff to output canvas
		CvSize roiSize;
		int step = 0;

		// get raw data from ipl image
		cvGetRawData( pDstImg, &rawData, &step, &roiSize );

		// convert data from raw image to wxImg


		wxImage *pWxImg = new wxImage( nCamWidth, nCamHeight, rawData, TRUE );

		// convert to bitmap to be used by the window to draw

		if (m_pBitmap)
		{
			delete m_pBitmap;
		}

		m_pBitmap = new wxBitmap( pWxImg->Scale(m_nWidth, m_nHeight) );

		m_bNewImage = true;
		m_bDrawing = false;

		Refresh( FALSE );

		//Update( );
		delete pWxImg;

		//cvReleaseImage( &pDstImg );


    }

}

////////////////////////////////////////////////////////////////////
// Method:	CheckUpdate
// Class:	CCamView
// Purose:	CHeck for updates
// Input:	reference to size event
// Output:	nothing
////////////////////////////////////////////////////////////////////
void CCamView::CheckUpdate()
{
	Update( );
}

////////////////////////////////////////////////////////////////////
// Method:	OnSize
// Class:	CCamView
// Purose:	adjust on windows resize
// Input:	reference to size event
// Output:	nothing
////////////////////////////////////////////////////////////////////
void CCamView::OnSize( wxSizeEvent& even )
{
	int nWidth = even.GetSize().GetWidth();
	int nHeight = even.GetSize().GetHeight();

	m_nWidth = nWidth;
	m_nHeight = nHeight;

}
