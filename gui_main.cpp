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
    virtual ~MyFrame();

    virtual void Generate( wxCommandEvent& event );
    virtual void OnQuit(wxCommandEvent& event);
    virtual void OnClickOpen( wxCommandEvent& event );
    virtual void OnClickSave( wxCommandEvent& event );
    virtual void OnClickImageSave( wxCommandEvent& event );

private:
    BitmapFile *file_;
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
       : MyFrame1(nullptr), file_(nullptr)
{
}


MyFrame::~MyFrame()
{
    if (this->file_ != nullptr) {
        release_bmp_file(this->file_);
        delete this->file_;
    }
}

void MyFrame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
    Close(true);
}

void MyFrame::Generate( wxCommandEvent& WXUNUSED(event) ) {
    std::string code = this->m_code_textbox->GetValue().ToStdString();

    if(this->file_ != nullptr) {
        release_bmp_file(file_);
        delete file_;
    }
    file_ = new BitmapFile();
    wxSize size = this->m_bitmap->GetSize();

    initialize_bmp_file(this->file_, size.GetWidth(), size.GetHeight());

    auto ast = parse(code);
    Environment env(this->file_);
    ast->run(env);

    wxImage image(size);
    for (int x = 0; x < size.GetWidth(); x++) {
        for (int y = 0; y < size.GetHeight(); y++) {
            size_t position = (y * size.GetWidth() + x) * 3;
            uint8_t b = this->file_->data[position];
            uint8_t g = this->file_->data[position + 1];
            uint8_t r = this->file_->data[position + 2];
            image.SetRGB(x, size.GetHeight() - y - 1, r, g, b);
        }
    }

    wxBitmap bitmap(image);
    wxBitmapBundle bundle(bitmap);
    this->m_bitmap->SetBitmap(bundle);
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
    if(!this->m_code_textbox->SaveFile(path)){
        wxMessageBox("保存に失敗しました。", "エラー", wxCLOSE, this);
    }
}

void MyFrame::OnClickImageSave( wxCommandEvent& WXUNUSED(event) )
{
    if (this->file_ == nullptr) {
        wxMessageBox("まだ画像を生成していません。", "エラー", wxOK, this);
        return;
    }
    wxFileDialog saveDialog(
            this,
            "保存",
            wxEmptyString,
            wxEmptyString,
            "*.bmp",
            wxFD_SAVE|wxFD_OVERWRITE_PROMPT);
    if (saveDialog.ShowModal() != wxID_OK) {
        return;
    }

    auto path = saveDialog.GetPath();
    // TODO: support const char *
    save_bmp_file((char *)path.ToStdString().c_str(), this->file_);
}
