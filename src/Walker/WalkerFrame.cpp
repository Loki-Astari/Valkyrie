#include "WalkerFrame.h"
#include "Walker.h"
#include <wx/progdlg.h>
#include <fstream>

using namespace ThorsAnvil::ValkyrieWalker;

BEGIN_EVENT_TABLE(WalkerFrame, wxFrame)
    EVT_MENU(wxID_ABOUT,        WalkerFrame::onAbout)
    EVT_MENU(wxID_EXIT,         WalkerFrame::onQuit)
    EVT_BUTTON(BUTTON_SAVE_ID,  WalkerFrame::onSave)
    EVT_BUTTON(BUTTON_RUN1_ID,  WalkerFrame::onRun1)
    EVT_BUTTON(BUTTON_SORT_ID,  WalkerFrame::onSort)
    EVT_BUTTON(BUTTON_EVOLVE_ID,WalkerFrame::onEvolve)
END_EVENT_TABLE()

WalkerFrame::WalkerFrame(std::vector<Walker>& walk)
    : wxFrame(nullptr, wxID_ANY , wxT("Valkyrie"))
    , walkers(walk)
    , panelWalker(nullptr)
{

    wxPanel*  panelButton= new wxPanel(this);
    wxButton* buttonSave = new wxButton(panelButton, BUTTON_SAVE_ID, wxT("Save"));
    wxButton* buttonRun  = new wxButton(panelButton, BUTTON_RUN1_ID, wxT("Run 1"));
    wxButton* buttonSort = new wxButton(panelButton, BUTTON_SORT_ID, wxT("Sort"));
    wxButton* buttonEvlv = new wxButton(panelButton, BUTTON_EVOLVE_ID, wxT("Evolve"));

    wxSizer* buttonSizer = new wxStdDialogButtonSizer();
    buttonSizer->Add(buttonSave, wxSizerFlags());
    buttonSizer->Add(buttonRun,  wxSizerFlags());
    buttonSizer->Add(buttonSort, wxSizerFlags());
    buttonSizer->Add(buttonEvlv, wxSizerFlags());
    panelButton->SetSizer(buttonSizer);

    panelWalker = new PanelWalkerCrowd(this, walkers);

    wxSizer* sizer      = new wxBoxSizer(wxVERTICAL);
    sizer->Add(panelButton, wxSizerFlags());
    sizer->Add(panelWalker, wxSizerFlags());

    wxSize clientArea = DoGetBestSize();
    clientArea.y += 32;
    SetSize(clientArea);

    wxMenu* fileMenu = new wxMenu;
    fileMenu->Append(wxID_EXIT,         wxT("E&xit\tAlt-X"),  wxT("Quit Valkyrie"));

    wxMenu* helpMenu = new wxMenu;
    helpMenu->Append(wxID_ABOUT, wxT("&About...\tF1"), wxT("Show about dialog"));

    wxMenuBar* menuBar = new wxMenuBar{};
    menuBar->Append(fileMenu, wxT("&File"));
    menuBar->Append(helpMenu, wxT("&Help"));

    SetMenuBar(menuBar);

    CreateStatusBar(2);
    SetStatusText(wxT("Welcome to Valkyrie"));

    SetSizerAndFit(sizer);
}


void WalkerFrame::onAbout(wxCommandEvent& /*event*/)
{
    wxString        msg;
    msg.Printf(wxT("Hello and Welcome to %s"), wxVERSION_STRING);
    wxMessageBox(msg, wxT("About Minimal"), wxOK | wxICON_INFORMATION, this);
}

void WalkerFrame::onQuit(wxCommandEvent& /*event*/)
{
    Close();
}

void WalkerFrame::onSave(wxCommandEvent& /*event*/)
{
    std::ofstream   save("save.walker");
    for (auto const& walker: walkers)
    {
        save << walker;
    }
}

void WalkerFrame::onRun1(wxCommandEvent& /*event*/)
{
    int count = walkers.size();
    int div   = count / 100;
    int max   = count / div + 1;

    wxProgressDialog dialog(wxT("Running all Walkers: "), wxT("Progress: "), max, this, wxPD_APP_MODAL);
    count = 0;
    for (auto& walker: walkers)
    {
        walker.run();
        if (count % div == 0)
        {
            dialog.Update(count / div);
        }
        ++count;
    }
}

void WalkerFrame::onSort(wxCommandEvent& /*event*/)
{
    panelWalker->shuffle();
}

void WalkerFrame::onEvolve(wxCommandEvent& /*event*/)
{
    panelWalker->evolve();
}
