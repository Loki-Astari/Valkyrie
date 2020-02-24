#include "ValkyrieWalker.h"
#include <ThorsUI/UIPanelAnimateableSteper.h>

using namespace ThorsAnvil::ValkyrieWalker;

DECLARE_APP(ValkyrieWalkerApp)

IMPLEMENT_APP(ValkyrieWalkerApp)

BEGIN_EVENT_TABLE(ValkyrieWalkerFrame, wxFrame)
    EVT_MENU(wxID_ABOUT,        ValkyrieWalkerFrame::onAbout)
    EVT_MENU(wxID_EXIT,         ValkyrieWalkerFrame::onQuit)
END_EVENT_TABLE()

/************ UI APP ********************/

ValkyrieWalkerApp::SeedSetter::SeedSetter()
{
    ThorsAnvil::ThorsUtil::Random::defaltGeneratorSeedSet(1);
}

bool ValkyrieWalkerApp::OnInit()
{
    std::cout.sync_with_stdio(false);
    std::cin.tie(nullptr);

    ValkyrieWalkerFrame* frame = new ValkyrieWalkerFrame(walker);

    frame->Show(true);
    return true;
}

/************ UI Frame ********************/

ValkyrieWalkerFrame::ValkyrieWalkerFrame(Walker& walk)
    : wxFrame(nullptr, wxID_ANY, wxT("Valkyrie"))
    , walker(walk)
{
    wxSizer* sizer      = new wxBoxSizer(wxVERTICAL);

    wxPanel* walkerPanel = new ThorsUI::PanelAnimateableSteper(this, walker);

    sizer->Add(walkerPanel, 1, 0, 10, nullptr);

    wxSizer* robotSizer = new wxGridSizer(10, 5, 5);
    sizer->Add(robotSizer, wxSizerFlags());

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

    SetSizer(sizer);
    sizer->Fit(this);
    sizer->SetSizeHints(this);
}


void ValkyrieWalkerFrame::onAbout(wxCommandEvent& /*event*/)
{
    wxString        msg;
    msg.Printf(wxT("Hello and Welcome to %s"), wxVERSION_STRING);
    wxMessageBox(msg, wxT("About Minimal"), wxOK | wxICON_INFORMATION, this);
}

void ValkyrieWalkerFrame::onQuit(wxCommandEvent& /*event*/)
{
    Close();
}
