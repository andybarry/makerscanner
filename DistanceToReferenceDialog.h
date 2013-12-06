#ifndef DISTANCETOREFERENCEDIALOG_H
#define DISTANCETOREFERENCEDIALOG_H

//(*Headers(DistanceToReferenceDialog)
#include <wx/spinctrl.h>
#include <wx/dialog.h>
#include <wx/sizer.h>
#include <wx/button.h>
#include <wx/panel.h>
#include <wx/statline.h>
#include <wx/stattext.h>
//*)

#define USER_CENTERED_LASER 999

#include "CaptureThread.h"

class DistanceToReferenceDialog: public wxDialog
{
	public:

		DistanceToReferenceDialog(CaptureThread *captureThreadIn, wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~DistanceToReferenceDialog();

		//(*Declarations(DistanceToReferenceDialog)
		wxPanel* Panel1;
		wxButton* butLaserCentered;
		wxStaticText* lblOr;
		wxStaticText* lblCoverLaser;
		wxStaticText* StaticText1;
		wxStaticLine* StaticLine1;
		wxButton* butLaserCovered;
		wxStaticText* lblCenterLaser;
		wxStaticText* lblEnterDistance;
		wxSpinCtrl* spinDistance;
		wxButton* butDistanceEntered;
		//*)

		float GetWallDistance() { return distance; }
		IplImage* GetLaserCenteredImage() { return laserCentered; }
		IplImage* GetNoLaserImage() { return noLaserImage; }

		virtual bool TransferDataFromWindow();

	protected:

		//(*Identifiers(DistanceToReferenceDialog)
		static const long ID_STATICTEXT1;
		static const long ID_STATICLINE1;
		static const long ID_STATICTEXT4;
		static const long ID_BUTTON1;
		static const long ID_STATICTEXT5;
		static const long ID_BUTTON2;
		static const long ID_STATICTEXT6;
		static const long ID_STATICTEXT7;
		static const long ID_SPINCTRL1;
		static const long ID_PANEL1;
		//*)

	private:

		//(*Handlers(DistanceToReferenceDialog)
		void OnButLaserCenteredClick(wxCommandEvent& event);
		void OnButLaserCoveredClick(wxCommandEvent& event);
		//*)

		float distance;
		IplImage *noLaserImage, *laserCentered;
		CaptureThread *captureThread;

		DECLARE_EVENT_TABLE()
};

#endif
