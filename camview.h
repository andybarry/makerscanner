////////////////////////////////////////////////////////////////////
// Name:		camera view header
// File:		camview.h
// Purpose:		interface for the CCamView class.
//
// Created by:	Larry Lart on 06-Jul-2006
// Updated by:	Andrew Barry Jan 2010
//
////////////////////////////////////////////////////////////////////

#ifndef _CCAMVIEW_H
#define _CCAMVIEW_H

#include <wx/wx.h>

#include "cv.h"
#include "highgui.h"

class CCamView : public wxWindow
{
public:
	CCamView( wxWindow *frame, const wxPoint& pos, const wxSize& size );

// Operations
public:


// Implementation
public:
	virtual ~CCamView( );
	bool IsCaptureEnabled( );
	void CheckUpdate( );

	// Draw method
	void DrawCam( IplImage* pImg );
	void Draw( wxDC& dc );

// Public data
public:
	//CCamera *m_pCamera;

// Protected data
protected:
    //CImage	m_Canvas;
	wxBitmap *m_pBitmap;

  	bool	m_bDrawing;
	bool	m_bNewImage;

	int m_nWidth;
	int m_nHeight;

// private methods
private:
	void	OnPaint(wxPaintEvent& event);
	void	OnSize( wxSizeEvent& even );

// protected data
protected:

	DECLARE_EVENT_TABLE()
};

#endif

