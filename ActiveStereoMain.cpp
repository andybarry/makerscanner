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

#include "ActiveStereoMain.h"
#include <wx/msgdlg.h>
#include <wx/numdlg.h>

//(*InternalHeaders(ActiveStereoFrame)
#include <wx/string.h>
#include <wx/intl.h>
//*)


//helper functions
enum wxbuildinfoformat {
    short_f, long_f };

wxString wxbuildinfo(wxbuildinfoformat format)
{
    wxString wxbuild(wxVERSION_STRING);

    if (format == long_f )
    {
#if defined(__WXMSW__)
        wxbuild << _T("-Windows");
        #define PIXMAPS_DIR ""
#elif defined(__UNIX__)
        wxbuild << _T("-Linux");
#endif

#if wxUSE_UNICODE
        wxbuild << _T("-Unicode build");
#else
        wxbuild << _T("-ANSI build");
#endif // wxUSE_UNICODE
    }

    return wxbuild;
}

//(*IdInit(ActiveStereoFrame)
const long ActiveStereoFrame::ID_STATICTEXT3 = wxNewId();
const long ActiveStereoFrame::ID_STATICTEXT4 = wxNewId();
const long ActiveStereoFrame::ID_STATICTEXT1 = wxNewId();
const long ActiveStereoFrame::ID_FPS_LABEL = wxNewId();
const long ActiveStereoFrame::ID_BUTTON4 = wxNewId();
const long ActiveStereoFrame::ID_STATICTEXT7 = wxNewId();
const long ActiveStereoFrame::ID_SLIDER3 = wxNewId();
const long ActiveStereoFrame::ID_STATICTEXT5 = wxNewId();
const long ActiveStereoFrame::ID_SLIDER1 = wxNewId();
const long ActiveStereoFrame::ID_STATICTEXT6 = wxNewId();
const long ActiveStereoFrame::ID_BUTTON3 = wxNewId();
const long ActiveStereoFrame::ID_BUTTON1 = wxNewId();
const long ActiveStereoFrame::ID_STATICLINE1 = wxNewId();
const long ActiveStereoFrame::ID_TEXTCTRL1 = wxNewId();
const long ActiveStereoFrame::ID_PANEL1 = wxNewId();
const long ActiveStereoFrame::idMenuQuit = wxNewId();
const long ActiveStereoFrame::idMenuChangeCamera = wxNewId();
const long ActiveStereoFrame::idMenuAbout = wxNewId();
const long ActiveStereoFrame::ID_STATUSBAR1 = wxNewId();
//*)

BEGIN_EVENT_TABLE(ActiveStereoFrame,wxFrame)
    //(*EventTable(ActiveStereoFrame)
    //*)

    EVT_COMMAND(wxID_ANY, IMAGE_UPDATE_EVENT, ActiveStereoFrame::UpdateImage)
    EVT_COMMAND(wxID_ANY, DISPLAY_TEXT_EVENT, ActiveStereoFrame::DisplayText)
    EVT_COMMAND(wxID_ANY, WRITE_TO_FILE_EVENT, ActiveStereoFrame::WriteToFile)
    EVT_COMMAND(wxID_ANY, SCAN_PROGRESS_EVENT, ActiveStereoFrame::UpdateScanProgress)
    EVT_COMMAND(wxID_ANY, SCAN_FINISHED_EVENT, ActiveStereoFrame::ScanFinished)

    EVT_UPDATE_UI(ID_FPS_LABEL, ActiveStereoFrame::UpdateFps)

END_EVENT_TABLE()

// Much of this is wxSmith generated code that creates (most of) the GUI.
ActiveStereoFrame::ActiveStereoFrame(wxWindow* parent,wxWindowID id)
{
    //(*Initialize(ActiveStereoFrame)
    wxMenuItem* MenuItem2;
    wxMenuItem* MenuItem1;
    wxFlexGridSizer* FlexGridSizer1;
    wxMenu* Menu1;
    wxBoxSizer* BoxSizer2;
    wxStaticBoxSizer* staticBoxSizerSettings;
    wxBoxSizer* topBoxSizer;
    wxBoxSizer* BoxSizer1;
    wxMenuBar* MenuBar1;
    wxMenu* Menu2;

    Create(parent, wxID_ANY, _("MakerScanner v0.3.2"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE, _T("wxID_ANY"));
    SetClientSize(wxSize(1009,746));
    SetMinSize(wxSize(1009,746));
    Panel1 = new wxPanel(this, ID_PANEL1, wxPoint(120,232), wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL1"));
    BoxSizer1 = new wxBoxSizer(wxVERTICAL);
    topBoxSizer = new wxBoxSizer(wxHORIZONTAL);
    headBoxSizer = new wxBoxSizer(wxVERTICAL);
    staticBoxSizerStatus = new wxStaticBoxSizer(wxHORIZONTAL, Panel1, _("System Status"));
    flexGridStatus = new wxFlexGridSizer(0, 2, 0, 0);
    lblStaticCameraConnected = new wxStaticText(Panel1, ID_STATICTEXT3, _("Camera: "), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT3"));
    flexGridStatus->Add(lblStaticCameraConnected, 0, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
    lblCameraConnected = new wxStaticText(Panel1, ID_STATICTEXT4, _("Disconnected"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT, _T("ID_STATICTEXT4"));
    flexGridStatus->Add(lblCameraConnected, 1, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
    StaticText1 = new wxStaticText(Panel1, ID_STATICTEXT1, _("FPS: "), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT1"));
    flexGridStatus->Add(StaticText1, 0, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
    lblFPS = new wxStaticText(Panel1, ID_FPS_LABEL, _("0.0"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_FPS_LABEL"));
    flexGridStatus->Add(lblFPS, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
    staticBoxSizerStatus->Add(flexGridStatus, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    butCameraConnect = new wxButton(Panel1, ID_BUTTON4, _("Camera Connect"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON4"));
    staticBoxSizerStatus->Add(butCameraConnect, 1, wxALL|wxALIGN_TOP|wxALIGN_CENTER_HORIZONTAL, 5);
    headBoxSizer->Add(staticBoxSizerStatus, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    staticBoxSizerSettings = new wxStaticBoxSizer(wxHORIZONTAL, Panel1, _("Scan Settings"));
    FlexGridSizer1 = new wxFlexGridSizer(0, 2, 0, 0);
    lblImageThreshold = new wxStaticText(Panel1, ID_STATICTEXT7, _("Brightness Threshold: 25"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT7"));
    FlexGridSizer1->Add(lblImageThreshold, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
    sliderImageThreshold = new wxSlider(Panel1, ID_SLIDER3, 25, 5, 50, wxDefaultPosition, wxSize(158,30), 0, wxDefaultValidator, _T("ID_SLIDER3"));
    FlexGridSizer1->Add(sliderImageThreshold, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    lblBrightnessFilter = new wxStaticText(Panel1, ID_STATICTEXT5, _("Brightness Filter: 0.80"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT5"));
    FlexGridSizer1->Add(lblBrightnessFilter, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
    sliderBrightnessFilter = new wxSlider(Panel1, ID_SLIDER1, 80, 0, 100, wxDefaultPosition, wxSize(158,35), 0, wxDefaultValidator, _T("ID_SLIDER1"));
    FlexGridSizer1->Add(sliderBrightnessFilter, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    staticBoxSizerSettings->Add(FlexGridSizer1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    headBoxSizer->Add(staticBoxSizerSettings, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticText2 = new wxStaticText(Panel1, ID_STATICTEXT6, _("Brightness Threshold controls how bright the laser must be in the image for it to be detected.  Low values will be more forgiving of light lasers or dark objects, but tend to produce more noise.\n\nBrightness Filter controls how bright pixels must appear relative to other laser pixels.  Low values are more forgiving but tend to produce more noise."), wxDefaultPosition, wxSize(319,170), 0, _T("ID_STATICTEXT6"));
    headBoxSizer->Add(StaticText2, 1, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
    BoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
    butCapture = new wxButton(Panel1, ID_BUTTON3, _("Start Scan"), wxDefaultPosition, wxSize(150,32), 0, wxDefaultValidator, _T("ID_BUTTON3"));
    butCapture->Disable();
    BoxSizer2->Add(butCapture, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    butDoneScanning = new wxButton(Panel1, ID_BUTTON1, _("Done Scanning"), wxDefaultPosition, wxSize(150,32), 0, wxDefaultValidator, _T("ID_BUTTON1"));
    butDoneScanning->Disable();
    BoxSizer2->Add(butDoneScanning, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    headBoxSizer->Add(BoxSizer2, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    topBoxSizer->Add(headBoxSizer, 0, wxALL|wxALIGN_TOP|wxALIGN_CENTER_HORIZONTAL, 5);
    BoxSizer1->Add(topBoxSizer, 0, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticLine1 = new wxStaticLine(Panel1, ID_STATICLINE1, wxDefaultPosition, wxSize(10,-1), wxLI_HORIZONTAL, _T("ID_STATICLINE1"));
    BoxSizer1->Add(StaticLine1, 0, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    txtLog = new wxTextCtrl(Panel1, ID_TEXTCTRL1, _("------------------ Quick Tips ------------------\nThe top 25 pixels (above the green line) are reserved for a flat surface.  If the entire top of the image is not looking at a flat surface, the scan will not work well.\n\nTry not to move the camera or objects during the scan.  The system uses an image-difference technique which means that it is sensitive to any changes in the image.\n\n------------------ Initializing ------------------"), wxDefaultPosition, wxSize(999,133), wxTE_MULTILINE|wxTE_READONLY|wxTE_WORDWRAP, wxDefaultValidator, _T("ID_TEXTCTRL1"));
    txtLog->SetMinSize(wxSize(390,154));
    BoxSizer1->Add(txtLog, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    Panel1->SetSizer(BoxSizer1);
    BoxSizer1->Fit(Panel1);
    BoxSizer1->SetSizeHints(Panel1);
    MenuBar1 = new wxMenuBar();
    Menu1 = new wxMenu();
    MenuItem1 = new wxMenuItem(Menu1, idMenuQuit, _("Quit\tAlt-F4"), _("Quit the application"), wxITEM_NORMAL);
    Menu1->Append(MenuItem1);
    MenuBar1->Append(Menu1, _("&File"));
    Menu3 = new wxMenu();
    menuChangeCamera = new wxMenuItem(Menu3, idMenuChangeCamera, _("Change Camera"), _("Change the camera if you have more than one (requires restart)"), wxITEM_NORMAL);
    Menu3->Append(menuChangeCamera);
    MenuBar1->Append(Menu3, _("Tools"));
    Menu2 = new wxMenu();
    MenuItem2 = new wxMenuItem(Menu2, idMenuAbout, _("About\tF1"), _("Show info about this application"), wxITEM_NORMAL);
    Menu2->Append(MenuItem2);
    MenuBar1->Append(Menu2, _("Help"));
    SetMenuBar(MenuBar1);
    StatusBar1 = new wxStatusBar(this, ID_STATUSBAR1, 0, _T("ID_STATUSBAR1"));
    int __wxStatusBarWidths_1[1] = { -1 };
    int __wxStatusBarStyles_1[1] = { wxSB_NORMAL };
    StatusBar1->SetFieldsCount(1,__wxStatusBarWidths_1);
    StatusBar1->SetStatusStyles(1,__wxStatusBarStyles_1);
    SetStatusBar(StatusBar1);

    Connect(ID_BUTTON4,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&ActiveStereoFrame::OnButCameraConnectClick);
    Connect(ID_SLIDER3,wxEVT_SCROLL_THUMBTRACK,(wxObjectEventFunction)&ActiveStereoFrame::OnSliderImageThresholdCmdScroll);
    Connect(ID_SLIDER3,wxEVT_SCROLL_CHANGED,(wxObjectEventFunction)&ActiveStereoFrame::OnSliderImageThresholdCmdScroll);
    Connect(ID_SLIDER1,wxEVT_SCROLL_THUMBTRACK,(wxObjectEventFunction)&ActiveStereoFrame::OnSliderBrightnessFilterCmdScrollThumbTrack);
    Connect(ID_SLIDER1,wxEVT_SCROLL_CHANGED,(wxObjectEventFunction)&ActiveStereoFrame::OnSliderBrightnessFilterCmdScrollThumbTrack);
    Connect(ID_BUTTON3,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&ActiveStereoFrame::OnButCaptureClick);
    Connect(ID_BUTTON1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&ActiveStereoFrame::OnButDoneScanningClick);
    Connect(idMenuQuit,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&ActiveStereoFrame::OnQuit);
    Connect(idMenuChangeCamera,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&ActiveStereoFrame::OnMenuChangeCameraSelected);
    Connect(idMenuAbout,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&ActiveStereoFrame::OnAbout);
    //*)

    // frame icon
    #if defined(__WXMSW__)
        {
            wxIcon FrameIcon(wxT("laserIcon.ico"), wxBITMAP_TYPE_ICO);
            SetIcon(FrameIcon);
        }
    #elif defined(__UNIX__)
        {
        wxIcon FrameIcon;
        FrameIcon.CopyFromBitmap(wxBitmap(wxImage(PIXMAPS_DIR _T("/laserIcon.png"))));
        SetIcon(FrameIcon);
        }
    #endif

    // create image display window on the frame and insert it into the sizer
    m_pCamView = new CCamView( Panel1, wxPoint(150,0), wxSize(640, 480) );
    topBoxSizer->Insert(0, m_pCamView, 0, wxFIXED_MINSIZE);

    // just added the camera window, so force sizers to reset
    topBoxSizer->Layout();
    BoxSizer1->Layout();

    updateImageRunning = false;

    // init camera and laser connected variables (used for labels)
    cameraConnected = false;

    // init last USB event time (so we don't overload the PIC with USB events)
    lastUsbTime = wxDateTime::UNow();

    scanStatus = new ScanStatus();

    // read the camera preference out of the config
    wxConfig *config = new wxConfig(wxT("makerscanner"));
    config->Set(config);

    if (config->Read(wxT("CameraNum"), &cameraNum))
    {
        // read the value successfully

        // check for the -1 (first camera detected) case
        if (cameraNum == -1)
        {
            cameraNum = CV_CAP_ANY;
        }
    } else {
        cameraNum = CV_CAP_ANY;
    }

    //Start up the camera and look at the image
    cam = new Cameras(txtLog, this, scanStatus, cameraNum);  // create the camera object

    framesSinceLastFpsUpdate = 0;

    timeOfLastFpsUpdate = wxDateTime::UNow();

    //cvNamedWindow( "win1", CV_WINDOW_AUTOSIZE );
}

// Destructor
ActiveStereoFrame::~ActiveStereoFrame()
{
    // make sure to delete cam before m_pCamView otherwise
    // you might get a crash on exit in Windows OS (most likley
    // when you have a UpdateImage event after the viewer is gone)
    if (cam)
    {
        delete cam;
    }
    if (m_pCamView)
    {
        delete m_pCamView;
    }
    if (scanStatus)
    {
        delete scanStatus;
    }
    if (config)
    {
        delete config;
    }
}

// On thread destruction
void ActiveStereoFrame::OnQuit(wxCommandEvent& event)
{
   Close();
}

// On about, show a dialog with name, email, etc.
void ActiveStereoFrame::OnAbout(wxCommandEvent& event)
{
    wxString msg = wxT("MakerScanner v0.3.2\nhttp://makerscanner.com\n\n(C) 2009-2010\nAndrew Barry\nabarry@gmail.com");
    wxMessageBox(msg, _("MakerScanner"));
}

// Scan button clicked -- start a new scan!
void ActiveStereoFrame::OnButCaptureClick(wxCommandEvent& event)
{
    wxString directory = wxT("");

    // read the last directory used
    wxConfig *config = (wxConfig*)wxConfigBase::Get();
    if (config->Read(wxT("ScanDir"), &directory))
    {
        // check for a not valid directory
        if (wxDir::Exists(directory) == false)
        {
            directory = wxT("");
        }
    } else {
        // failed to read config
        directory = wxT("");
    }
    // ask where to save the point cloud file
    wxFileDialog dialog(this, wxT("Save pointcloud file"), directory, wxT("pointcloud.ply"), wxT("*.ply"), wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
    if (dialog.ShowModal() != wxID_OK)
    {
        // user clicked cancel
        return;
    }

    // get the filename and path as a string
    pointcloudFilename = dialog.GetPath();

    wxFileName dir(pointcloudFilename);
    if (dir.IsOk() == true)
    {
        // save the new selected directory
        config->Write(wxT("ScanDir"), dir.GetPath());

    }

    // clear the point cloud file
    wxFFile file(pointcloudFilename, wxT("w"));
    if (file.IsOpened())
    {
        txtLog->AppendText(wxT("\nWriting point cloud file:\n\t"));
        txtLog->AppendText(pointcloudFilename);
        file.Close();
    } else {
        txtLog->AppendText(wxT("\nPoint cloud file did not open!"));
        return;
    }

    // Disable buttons/sliders during scanning
    SetGUIStateDuringScan(true);

    if (!cam)
    {
        cam = new Cameras(txtLog, this, scanStatus, cameraNum);  // create the camera object if it doesn't already exist
    }

    cam->SetThresholdPixelValue(sliderImageThreshold->GetValue());
    cam->SetBrightnessFilterValue(float(sliderBrightnessFilter->GetValue()) / 100.0);

    // start scanning!
    cam->StartScan();
    butDoneScanning->SetFocus();
}

// Catch an update image event and display the new image
void ActiveStereoFrame::UpdateImage(wxCommandEvent &event)
{

    // image is shipped as a pointer in the event
    // cast to IplImage (must release once finished)
    IplImage *img = (IplImage*)event.GetClientData();

    if (updateImageRunning == true)
    {
        txtLog->AppendText(wxT("\nUpdate image was already running"));
        cvReleaseImage(&img);
        return;
    }
    updateImageRunning = true;

    // camera is connected -- update if it isn't
    if (cameraConnected != true)
    {
        cameraConnected = true;
        lblCameraConnected->SetLabel(wxT("Connected"));

        // reset layouts now that the length of the label in lblCameraConnected has changed
        flexGridStatus->Layout();
        staticBoxSizerStatus->Layout();
        headBoxSizer->Layout();

        butCapture->Enable(true);
        butCapture->SetFocus();
    }

    // update image display
    m_pCamView->DrawCam(img);

    // delete the image that was copied for us to display
    cvReleaseImage(&img);

    updateImageRunning = false;

    // update the FPS display
    framesSinceLastFpsUpdate ++;


}

// catch a display text event and append the text to the terminal display
void ActiveStereoFrame::DisplayText(wxCommandEvent &event)
{
    txtLog->AppendText(event.GetString());
}

// catch a write to file event
// the string to write is in the event
// write to the point cloud file
void ActiveStereoFrame::WriteToFile(wxCommandEvent &event)
{
    wxString FilesWritten = wxT("");
    //static int i = 0;  // monitor how many times we do file writes
    //if (i==0) txtLog->AppendText(wxT("\nWriting topoint cloud file: "));
    //i++;
   //FilesWritten << i << wxT(" ");
    wxFFile file(pointcloudFilename, wxT("a"));
    file.Write(event.GetString());
    if (file.IsOpened())  txtLog->AppendText(FilesWritten);
    else txtLog->AppendText(wxT("Point cloud file did not open!"));
    file.Close();
}

// Camera connect button clicked -- attempt to connect to the camera
void ActiveStereoFrame::OnButCameraConnectClick(wxCommandEvent& event)
{
    if (cam)
    {
        delete cam;
    }
    cam = new Cameras(txtLog, this, scanStatus, cameraNum);  // create the camera object
}

// Set buttons to be enabled/disabled during (or after) a scan
void ActiveStereoFrame::SetGUIStateDuringScan(bool scanning)
{
    bool enable;
    if (scanning == true)
    {
        enable = false;
        butCapture->SetLabel(wxT("Scanning..."));

    } else {
        enable = true;
        butCapture->SetLabel(wxT("Start Scan"));
        butDoneScanning->SetLabel(wxT("Done Scanning"));
    }

    butCapture->Enable(enable);
    butDoneScanning->Enable(!enable);
    sliderImageThreshold->Enable(enable);
    sliderBrightnessFilter->Enable(enable);

    butCameraConnect->Enable(enable);

    if (scanning == false)
    {
        butCapture->SetFocus();
    }

}

// Catch an update scan event and move the progress bar
void ActiveStereoFrame::UpdateScanProgress(wxCommandEvent &event)
{
    // nothing here for now
}

// Re-enable buttons now that the scan is finished
void ActiveStereoFrame::ScanFinished(wxCommandEvent &event)
{
    SetGUIStateDuringScan(false);
}

// Done Scanning button event handler
void ActiveStereoFrame::OnButDoneScanningClick(wxCommandEvent& event)
{
    // stop the scan by telling scanStatus to stop scanning.
    // this will let the scanning thread finish it's current tasks
    // once done, the thread will send a scan finished event.
    scanStatus->SetScanning(false);
    butDoneScanning->SetLabel(wxT("Finishing Scan..."));
    butDoneScanning->Enable(false);
}

// Update the image threshold slider label on slider move
void ActiveStereoFrame::OnSliderImageThresholdCmdScroll(wxScrollEvent& event)
{
    // update the label
    wxString str = wxT("Brightness Threshold: ");
    str << sliderImageThreshold->GetValue();
    lblImageThreshold->SetLabel(str);
}

void ActiveStereoFrame::OnSliderBrightnessFilterCmdScrollThumbTrack(wxScrollEvent& event)
{
    // update the label
    wxString str = wxT("Brightness Filter: ");
    wxString numstr;
    numstr.Printf(wxT("%.2f"), float(sliderBrightnessFilter->GetValue()) / 100.0);
    lblBrightnessFilter->SetLabel(str + numstr);
}

void ActiveStereoFrame::OnMenuChangeCameraSelected(wxCommandEvent& event)
{
    // open the camera number dialog
    wxNumberEntryDialog dialog (this, wxT("Cameras are numbered in the order they are connected in.\n\nSet the value to -1 to select the first valid camera detected."), wxT("New camera number:"), wxT("MakerScanner - Change Camera"), cameraNum, -1, 10);

    long cameraNumNew;

    if (dialog.ShowModal() == wxID_OK)
    {
        cameraNumNew = dialog.GetValue();

        wxConfig *config = (wxConfig*)wxConfigBase::Get();

        config->Write(wxT("CameraNum"), cameraNumNew);

        wxMessageBox(wxT("You must restart MakerScanner for your changes to take effect."),
            wxT("MakerScanner - Settings Changed"));
    }
}

void ActiveStereoFrame::UpdateFps(wxUpdateUIEvent &event)
{
    wxDateTime now = wxDateTime::UNow();

    wxTimeSpan timeDiff = now.Subtract(timeOfLastFpsUpdate);

    if (timeDiff.GetMilliseconds() > 100)
    {
        // compute a new FPS
        double timeDiffNum = timeDiff.GetMilliseconds().ToDouble() / 1000.0;

        double fps = framesSinceLastFpsUpdate / timeDiffNum;

        framesSinceLastFpsUpdate = 0;

        timeOfLastFpsUpdate = now;

        wxString fpsString = wxString::Format(wxT("%4.1f"), fps);

        lblFPS->SetLabel(fpsString);

        // disable things if the fps is too low
        if (fps < 0.5)
        {
            cameraConnected = false;
            lblCameraConnected->SetLabel(wxT("Disconnected"));

            // reset layouts now that the length of the label in lblCameraConnected has changed
            flexGridStatus->Layout();
            staticBoxSizerStatus->Layout();
            headBoxSizer->Layout();

            butCapture->Enable(false);
        }
    }

}
