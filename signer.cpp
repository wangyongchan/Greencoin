#ifdef _WIN32
#define _WIN32_WINNT 0x0A00
#endif

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif
#include <wx/clipbrd.h>

#include "crp.h"

bool loaded = false;
class MyApp: public wxApp
{
public:
    virtual bool OnInit();
};
class MainFrame: public wxFrame
{
public:
    MainFrame(const wxString& title, const wxPoint& pos, const wxSize& size);
private:
    void OnExit(wxCommandEvent& event);
    void OnLoad(wxCommandEvent& event);
    void OnCreate(wxCommandEvent& event);
    void OnSave(wxCommandEvent& event);
    void OnSign(wxCommandEvent& event);
    void OnVerify(wxCommandEvent& event);
    wxDECLARE_EVENT_TABLE();
};
enum
{
    ID_Create = wxID_HIGHEST + 101,
    ID_Save = wxID_HIGHEST + 202,
    ID_Load = wxID_HIGHEST + 303,
    ID_Sign = wxID_HIGHEST + 404,
    ID_Verify = wxID_HIGHEST + 505
};
class cryptoFunc : public crp::crypto_functions{};
wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
    EVT_MENU(wxID_EXIT, MainFrame::OnExit)
    EVT_MENU(ID_Load, MainFrame::OnLoad)
    EVT_BUTTON(ID_Create, MainFrame::OnCreate)
    EVT_BUTTON(ID_Save, MainFrame::OnSave)
    EVT_BUTTON(ID_Sign, MainFrame::OnSign)
    EVT_BUTTON(ID_Verify, MainFrame::OnVerify)
wxEND_EVENT_TABLE()
wxIMPLEMENT_APP(MyApp);
bool MyApp::OnInit()
{
    MainFrame *mframe = new MainFrame( "Greencoin", wxPoint(50, 50), wxSize(700, 500));
    mframe->Show( true );
    return true;
};
MainFrame::MainFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
        : wxFrame(NULL, wxID_ANY, title, pos, size)
{
    wxPanel *background = new wxPanel(this, wxID_ANY);
    wxButton *pbutton = new wxButton(background, ID_Create, "Create Private Key", wxPoint(10,70), wxSize(125,30));
    wxButton *tbutton = new wxButton(background, ID_Save, "Create Public ID", wxPoint(160,70), wxSize(125,30));
    wxButton *sbutton = new wxButton(background, ID_Sign, "Sign Message", wxPoint(310,70), wxSize(125,30));
    wxButton *vbutton = new wxButton(background, ID_Verify, "Verify Message", wxPoint(460,70), wxSize(125,30));
    wxStaticText *name = new wxStaticText(background, wxID_ANY,
        "This application is for creating Private Keys, Public ID's and signing messages ONLY."
        " This application is compatible with future Greencoin applications."
        " All messages besides the Private Key are automatically copied to clipboard."
        " Greencoin recommends Private Keys only be kept offline on paper."
        " Visit github.com/wangyongchan/Greencoin for more information."
        , wxPoint(5,5), wxSize(685, 65));
    wxMenu *menuFile = new wxMenu;
    menuFile->Append(ID_Load, "&Enter Private Key", "Enter a Private Key");
    menuFile->AppendSeparator();
    menuFile->Append(wxID_EXIT);
    wxMenuBar *menuBar = new wxMenuBar;
    SetMenuBar( menuBar );
    menuBar->Append(menuFile, "&File");
    CreateStatusBar();
    SetStatusText( "Greencoin signer" );
};
void MainFrame::OnCreate(wxCommandEvent& event)
{
    cryptoFunc c;
    std::string strCreate = c.pCreate();
    wxString strCreated(strCreate);
    loaded = true;
    wxLogMessage(strCreated);
}
void MainFrame::OnExit(wxCommandEvent& event)
{
    Close( true );
}
void MainFrame::OnLoad(wxCommandEvent& event)
{
     wxString load1 = wxGetTextFromUser("Private Key...", "Enter Private Key", wxEmptyString, this);
     std::string loader = std::string(load1.mb_str());
     cryptoFunc c;
     c.pLoad(loader);
     loaded = true;
}
void MainFrame::OnSave(wxCommandEvent& event)
{
    if(!loaded)
    {
    wxLogMessage("Please load Private Key first");
    }
    else
    {
    cryptoFunc c;
    std::string strsave1 = c.pSave();
    wxString strSaved(strsave1);
    if (wxTheClipboard->Open())
    {
        wxTheClipboard->SetData( new wxTextDataObject (strSaved));
        wxTheClipboard->Close();
    }
    wxLogMessage(strSaved);
    }
}
void MainFrame::OnSign(wxCommandEvent& event)
{
    if(!loaded)
    {
    wxLogMessage("Please load Private Key first");
    }
    else
    {
    wxString signer = wxGetTextFromUser("Message...", "Sign Message", wxEmptyString, this);
    std::string signer1 = std::string(signer.mb_str());
    cryptoFunc c;
    std::string strsign1 = c.pSign(signer1);
    wxString strSigned(strsign1);
    if (wxTheClipboard->Open())
    {
        wxTheClipboard->SetData( new wxTextDataObject (strSigned));
        wxTheClipboard->Close();
    }
    wxLogMessage(strSigned);
    }
}
void MainFrame::OnVerify(wxCommandEvent& event)
{
    wxString mverifier = wxGetTextFromUser("Message...", "Message Before Signature", wxEmptyString, this);
    wxString verifier = wxGetTextFromUser("Signature...", "Signature of Message", wxEmptyString, this);
    wxString verifierid = wxGetTextFromUser("Public ID...", "Signature Public ID", wxEmptyString, this);
    std::string mverifier1 = std::string(mverifier.mb_str());
    std::string verifier1 = std::string(verifier.mb_str());
    std::string verifierid1 = std::string(verifierid.mb_str());
    cryptoFunc c;
    if(!c.pVerify(mverifier1, verifier1, verifierid1))
    {
    wxLogMessage("Message Could Not Be Verified!");
    }
    else
    {
    wxLogMessage("Message Verified Successfully!");
    }
}
