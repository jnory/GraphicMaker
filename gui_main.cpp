// modified from wxWidgets sample. See COPYING.md for details

#include "wx/wxprec.h"
#include "wx/wfstream.h"

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
    virtual void OnClickOpen( wxCommandEvent& event );
    virtual void OnClickSave( wxCommandEvent& event );
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

void MyFrame::Generate( wxCommandEvent& WXUNUSED(event) ) {
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
    for (int x = 0; x < size.GetWidth(); x++) {
        for (int y = 0; y < size.GetHeight(); y++) {
            size_t position = (y * size.GetWidth() + x) * 3;
            uint8_t b = file.data[position];
            uint8_t g = file.data[position + 1];
            uint8_t r = file.data[position + 2];
            image.SetRGB(x, size.GetHeight() - y - 1, r, g, b);
        }
    }

    wxBitmap bitmap(image);
    wxBitmapBundle bundle(bitmap);
    this->m_bitmap->SetBitmap(bundle);
    release_bmp_file(&file);
}

void MyFrame::OnClickOpen( wxCommandEvent& WXUNUSED(event) )
{
    if (!this->m_code_textbox->GetValue().IsEmpty()) {
        auto ans = wxMessageBox("現在のコードは失われます。よろしいですか？", "確認", wxYES_NO, this);
        if (ans != wxYES) {
            return;
        }
    }

    auto cwd = wxGetCwd();
    wxFileDialog openDialog(
            this,
            "開く",
            cwd,
            wxEmptyString,
            wxFileSelectorDefaultWildcardStr,
            wxFD_OPEN);
    if (openDialog.ShowModal() != wxID_OK) {
        return;
    }

    auto path = openDialog.GetPath();
    if(!this->m_code_textbox->LoadFile(path)){
        wxMessageBox("読み込みに失敗しました。", "エラー", wxCLOSE, this);
    }
}

void MyFrame::OnClickSave( wxCommandEvent& WXUNUSED(event) )
{
    wxFileDialog saveDialog(
            this,
            "保存",
            wxEmptyString,
            wxEmptyString,
            wxFileSelectorDefaultWildcardStr,
            wxFD_SAVE|wxFD_OVERWRITE_PROMPT);
    if (saveDialog.ShowModal() != wxID_OK) {
        return;
    }

    auto path = saveDialog.GetPath();
    if(wxFile::Exists(path)){
        auto ans = wxMessageBox("上書きします。よろしいですか？", "確認", wxYES_NO, this);
        if (ans != wxYES) {
            return;
        }
    }
    if(!this->m_code_textbox->SaveFile(path)){
        wxMessageBox("保存に失敗しました。", "エラー", wxCLOSE, this);
    }
}
