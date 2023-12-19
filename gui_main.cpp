// modified from wxWidgets sample. See COPYING.md for details

#include "wx/wxprec.h"

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include "design/gui.h"
#include "io/bitmap.hpp"
#include "parse/parser.hpp"

class MyApp : public wxApp
{
public:
    virtual bool OnInit() override;
};

class MyFrame : public MyFrame1
{
public:
    // ctor(s)
    MyFrame();

    virtual void Generate( wxCommandEvent& event );
    virtual void OnQuit(wxCommandEvent& event);
};

wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit()
{
    if ( !wxApp::OnInit() )
        return false;

    MyFrame *frame = new MyFrame();

    frame->Show(true);

    return true;
}

MyFrame::MyFrame()
       : MyFrame1(nullptr)
{
}



void MyFrame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
    Close(true);
}

void MyFrame::Generate( wxCommandEvent& event ) {
    std::string code = this->m_code_textbox->GetValue().ToStdString();

    BitmapFile file;
    
    wxSize size = this->m_bitmap->GetSize();

    initialize_bmp_file(&file, size.GetWidth(), size.GetHeight());

    auto shapes = parse(code);
    for (auto &shape: shapes) {
        shape->draw(&file);
        delete shape;
    }

    wxImage image(size);
    for (size_t x = 0; x < size.GetWidth(); x++) {
        for (size_t y = 0; y < size.GetHeight(); y++) {
            size_t position = (y * size.GetWidth() + x) * 3;
            uint8_t b = file.data[position];
            uint8_t g = file.data[position + 1];
            uint8_t r = file.data[position + 2];
            image.SetRGB(x, y, r, g, b);
        }
    }

    wxBitmap bitmap(image);
    wxBitmapBundle bundle(bitmap);
    this->m_bitmap->SetBitmap(bundle);
    release_bmp_file(&file);
}