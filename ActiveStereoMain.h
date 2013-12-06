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

#ifndef ACTIVESTEREOMAIN_H
#define ACTIVESTEREOMAIN_H

//(*Headers(ActiveStereoFrame)
#include <wx/sizer.h>
#include <wx/button.h>
#include <wx/menu.h>
#include <wx/panel.h>
#include <wx/slider.h>
#include <wx/statusbr.h>
#include <wx/statline.h>
#include <wx/frame.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
//*)


#include <wx/numdlg.h>
#include <wx/ffile.h>
#include <wx/dir.h>
#include <wx/datetime.h>
#include <wx/config.h>
#include <wx/filename.h>

// include OpenCV headers
#include "cv.h"
#include "highgui.h"

#include "Cameras.h"
#include "ScanThread.h"
#include "ScanStatus.h"

// include image-viewing widget header
#include "camview.h"

class ActiveStereoFrame: public wxFrame
{
    public:

        ActiveStereoFrame(wxWindow* parent,wxWindowID id = -1);
        virtual ~ActiveStereoFrame();

    private:

        //(*Handlers(ActiveStereoFrame)
        void OnQuit(wxCommandEvent& event);
        void OnAbout(wxCommandEvent& event);
        void OnButSetConfigClick(wxCommandEvent& event);
        void OnButCaptureClick(wxCommandEvent& event);
        void OnSliderImageThresholdCmdScroll(wxScrollEvent& event);
        void OnSliderLaserMaxCmdScrollChanged(wxScrollEvent& event);
        void OnSliderLaserMinCmdScrollChanged(wxScrollEvent& event);
        void OnButCameraConnectClick(wxCommandEvent& event);
        void OnSliderMirrorResolutionCmdScrollChanged(wxScrollEvent& event);
        void OnSliderMirrorPositionCmdScrollChanged(wxScrollEvent& event);
        void OnSliderLaserIntensityCmdScrollChanged(wxScrollEvent& event);
        void OnSliderServoPositionCmdScrollChanged(wxScrollEvent& event);
        void OnSliderMirrorPositionCmdScrollChangedWithMove(wxScrollEvent& event);
        void OnButDoneScanningClick(wxCommandEvent& event);
        void OnSliderBrightnessFilterCmdScrollThumbTrack(wxScrollEvent& event);
        void OnMenuChangeCameraSelected(wxCommandEvent& event);
        //*)

        //(*Identifiers(ActiveStereoFrame)
        static const long ID_STATICTEXT3;
        static const long ID_STATICTEXT4;
        static const long ID_STATICTEXT1;
        static const long ID_FPS_LABEL;
        static const long ID_BUTTON4;
        static const long ID_STATICTEXT7;
        static const long ID_SLIDER3;
        static const long ID_STATICTEXT5;
        static const long ID_SLIDER1;
        static const long ID_STATICTEXT6;
        static const long ID_BUTTON3;
        static const long ID_BUTTON1;
        static const long ID_STATICLINE1;
        static const long ID_TEXTCTRL1;
        static const long ID_PANEL1;
        static const long idMenuQuit;
        static const long idMenuChangeCamera;
        static const long idMenuAbout;
        static const long ID_STATUSBAR1;
        //*)

        //(*Declarations(ActiveStereoFrame)
        wxPanel* Panel1;
        wxMenuItem* menuChangeCamera;
        wxStatusBar* StatusBar1;
        wxMenu* Menu3;
        wxSlider* sliderBrightnessFilter;
        wxStaticText* lblFPS;
        wxStaticText* StaticText1;
        wxStaticText* lblBrightnessFilter;
        wxBoxSizer* headBoxSizer;
        wxStaticLine* StaticLine1;
        wxSlider* sliderImageThreshold;
        wxTextCtrl* txtLog;
        wxStaticText* lblImageThreshold;
        wxStaticText* lblCameraConnected;
        wxStaticText* StaticText2;
        wxFlexGridSizer* flexGridStatus;
        wxButton* butCameraConnect;
        wxButton* butCapture;
        wxStaticBoxSizer* staticBoxSizerStatus;
        wxButton* butDoneScanning;
        wxStaticText* lblStaticCameraConnected;
        //*)

		// We must use events to communicate with the scanning thread because direct GUI
		// modifications of the frame from the thread will fail badly

		// Update image event handler -- get an image and display it on the frame
		void UpdateImage(wxCommandEvent &event);

		// Catch an event and display its text in the terminal box
		void DisplayText(wxCommandEvent &event);

		// Write the point cloud file with the text stored in the event
		void WriteToFile(wxCommandEvent &event);

		// Catch an event to update the scan progress bar
		void UpdateScanProgress(wxCommandEvent &event);

		// Catch the scan finished event and re-enable the frame
		void ScanFinished(wxCommandEvent &event);

		// Enable/disable frame buttons (disable when scanning == true)
		void SetGUIStateDuringScan(bool scanning);

        // Catches an UpdateUI event and updates the FPS display
        void UpdateFps(wxUpdateUIEvent &event);

		// last time a USB event was sent
		wxDateTime lastUsbTime;

		// camera object, allows access to scanning thread
		Cameras *cam;

		// object that holds the current scan's status for communitcating between
		// the GUI and scanning/capturing threads.
		ScanStatus *scanStatus;

		wxString pointcloudFilename;

		// image display widget
		CCamView* m_pCamView;

		// configuration storage object
		wxConfig *config;

		// data for the FPS display
		wxDateTime timeOfLastFpsUpdate;
		int framesSinceLastFpsUpdate, cameraNum;

		bool updateImageRunning, cameraConnected;

        DECLARE_EVENT_TABLE()
};

#endif // ACTIVESTEREOMAIN_H
