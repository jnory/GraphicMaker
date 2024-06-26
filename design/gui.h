///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 4.0.0-0-g0efcecf)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#pragma once

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/string.h>
#include <wx/textctrl.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/button.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/sizer.h>
#include <wx/statbmp.h>
#include <wx/menu.h>
#include <wx/frame.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class MyFrame1
///////////////////////////////////////////////////////////////////////////////
class MyFrame1 : public wxFrame
{
	private:

	protected:
		wxTextCtrl* m_code_textbox;
		wxButton* m_exit_button;
		wxButton* m_generate_button;
		wxStaticBitmap* m_bitmap;
		wxMenuBar* m_menubar1;
		wxMenu* m_menu1;

		// Virtual event handlers, override them in your derived class
		virtual void OnQuit( wxCommandEvent& event ) { event.Skip(); }
		virtual void Generate( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnClickOpen( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnClickSave( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnClickImageSave( wxCommandEvent& event ) { event.Skip(); }


	public:

		MyFrame1( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Graphic"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 628,360 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );

		~MyFrame1();

};

