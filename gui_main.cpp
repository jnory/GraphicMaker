// modified from wxWidgets sample. See COPYING.md for details

#include <fstream>

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
    std::ofstream ofs("tmp.txt");
    std::string code = this->m_code_textbox->GetValue().ToStdString();
    ofs << code << std::endl << std::endl;

    BitmapFile file;
    
    wxSize size = this->m_bitmap->GetSize();
    ofs << size.GetWidth() << " " << size.GetHeight() << std::endl << std::endl;

    initialize_bmp_file(&file, size.GetWidth(), size.GetHeight());

    ofs << "Initialized" << std::endl << std::endl;

    auto shapes = parse(code);
    for (auto &shape: shapes) {
        shape->draw(&file);
        delete shape;
    }
    ofs << "Shape built" << std::endl << std::endl;

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
    ofs << "wxImage built" << std::endl << std::endl;

    wxBitmap bitmap(image);
    ofs << "wxBitmap built" << std::endl << std::endl;
    wxBitmapBundle bundle(bitmap);
    this->m_bitmap->SetBitmap(bundle);
    ofs << "SetBitmap" << std::endl << std::endl;
    release_bmp_file(&file);
}