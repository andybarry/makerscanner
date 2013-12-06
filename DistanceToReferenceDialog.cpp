#include "DistanceToReferenceDialog.h"

//(*InternalHeaders(DistanceToReferenceDialog)
#include <wx/string.h>
#include <wx/intl.h>
//*)

//(*IdInit(DistanceToReferenceDialog)
const long DistanceToReferenceDialog::ID_STATICTEXT1 = wxNewId();
const long DistanceToReferenceDialog::ID_STATICLINE1 = wxNewId();
const long DistanceToReferenceDialog::ID_STATICTEXT4 = wxNewId();
const long DistanceToReferenceDialog::ID_BUTTON1 = wxNewId();
const long DistanceToReferenceDialog::ID_STATICTEXT5 = wxNewId();
const long DistanceToReferenceDialog::ID_BUTTON2 = wxNewId();
const long DistanceToReferenceDialog::ID_STATICTEXT6 = wxNewId();
const long DistanceToReferenceDialog::ID_STATICTEXT7 = wxNewId();
const long DistanceToReferenceDialog::ID_SPINCTRL1 = wxNewId();
const long DistanceToReferenceDialog::ID_PANEL1 = wxNewId();
//*)

BEGIN_EVENT_TABLE(DistanceToReferenceDialog,wxDialog)
	//(*EventTable(DistanceToReferenceDialog)
	//*)
END_EVENT_TABLE()

DistanceToReferenceDialog::DistanceToReferenceDialog(CaptureThread *captureThreadIn, wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(DistanceToReferenceDialog)
	wxFlexGridSizer* FlexGridSizer1;
	wxBoxSizer* BoxSizer2;
	wxBoxSizer* BoxSizer1;
	wxBoxSizer* BoxSizer5;
	
	Create(parent, wxID_ANY, _("Distance to Reference Surface"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T("wxID_ANY"));
	SetClientSize(wxSize(446,289));
	BoxSizer1 = new wxBoxSizer(wxHORIZONTAL);
	Panel1 = new wxPanel(this, ID_PANEL1, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL1"));
	BoxSizer2 = new wxBoxSizer(wxVERTICAL);
	BoxSizer5 = new wxBoxSizer(wxHORIZONTAL);
	StaticText1 = new wxStaticText(Panel1, ID_STATICTEXT1, _("Before we start, we need to know a few things:\n\n1) what your object looks like without the laser.\n2) how far away your flat reference surface is."), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT1"));
	BoxSizer5->Add(StaticText1, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer2->Add(BoxSizer5, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticLine1 = new wxStaticLine(Panel1, ID_STATICLINE1, wxDefaultPosition, wxSize(10,-1), wxLI_HORIZONTAL, _T("ID_STATICLINE1"));
	BoxSizer2->Add(StaticLine1, 0, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer1 = new wxFlexGridSizer(0, 2, 0, 0);
	lblCoverLaser = new wxStaticText(Panel1, ID_STATICTEXT4, _("1) Cover the laser"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT4"));
	FlexGridSizer1->Add(lblCoverLaser, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	butLaserCovered = new wxButton(Panel1, ID_BUTTON1, _("Done"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON1"));
	FlexGridSizer1->Add(butLaserCovered, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	lblCenterLaser = new wxStaticText(Panel1, ID_STATICTEXT5, _("2) Center the laser"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT5"));
	lblCenterLaser->Disable();
	FlexGridSizer1->Add(lblCenterLaser, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	butLaserCentered = new wxButton(Panel1, ID_BUTTON2, _("Done"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON2"));
	butLaserCentered->Disable();
	FlexGridSizer1->Add(butLaserCentered, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	lblOr = new wxStaticText(Panel1, ID_STATICTEXT6, _("or"), wxDefaultPosition, wxSize(45,17), 0, _T("ID_STATICTEXT6"));
	lblOr->Disable();
	FlexGridSizer1->Add(lblOr, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer1->Add(-1,-1,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	lblEnterDistance = new wxStaticText(Panel1, ID_STATICTEXT7, _("Enter the distance to the\nflat reference wall (cm):"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT7"));
	lblEnterDistance->Disable();
	FlexGridSizer1->Add(lblEnterDistance, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	spinDistance = new wxSpinCtrl(Panel1, ID_SPINCTRL1, _T("50"), wxDefaultPosition, wxSize(79,27), 0, 0, 1000, 50, _T("ID_SPINCTRL1"));
	spinDistance->SetValue(_T("50"));
	spinDistance->Disable();
	FlexGridSizer1->Add(spinDistance, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer1->Add(-1,-1,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	butDistanceEntered = new wxButton(Panel1, wxID_OK, _("Done"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_OK"));
	butDistanceEntered->Disable();
	FlexGridSizer1->Add(butDistanceEntered, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer2->Add(FlexGridSizer1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	Panel1->SetSizer(BoxSizer2);
	BoxSizer2->Fit(Panel1);
	BoxSizer2->SetSizeHints(Panel1);
	BoxSizer1->Add(Panel1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	SetSizer(BoxSizer1);
	BoxSizer1->SetSizeHints(this);
	
	Connect(ID_BUTTON1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&DistanceToReferenceDialog::OnButLaserCoveredClick);
	Connect(ID_BUTTON2,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&DistanceToReferenceDialog::OnButLaserCenteredClick);
	//*)

	butLaserCovered->SetFocus();
	captureThread = captureThreadIn;
}

DistanceToReferenceDialog::~DistanceToReferenceDialog()
{
	//(*Destroy(DistanceToReferenceDialog)
	//*)
}


void DistanceToReferenceDialog::OnButLaserCenteredClick(wxCommandEvent& event)
{
	captureThread->Flush();
	IplImage *laserCenteredTemp = captureThread->Pop();

	laserCentered = cvCloneImage(laserCenteredTemp);

	EndModal(USER_CENTERED_LASER);
}


bool DistanceToReferenceDialog::TransferDataFromWindow()
{
	distance = spinDistance->GetValue();

	return true;
}

void DistanceToReferenceDialog::OnButLaserCoveredClick(wxCommandEvent& event)
{
	captureThread->Flush();
	IplImage *tempNoLaserImage = captureThread->Pop();

	noLaserImage = cvCloneImage(tempNoLaserImage);


	butLaserCovered->Enable(false);
	lblCoverLaser->Enable(false);

	lblCenterLaser->Enable(true);
	butLaserCentered->Enable(true);
	lblOr->Enable(true);
	lblEnterDistance->Enable(true);
	spinDistance->Enable(true);
	butDistanceEntered->Enable(true);

	butLaserCentered->SetFocus();

}


