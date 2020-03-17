#include "UIAppValkyrieRunner.h"

using namespace ThorsAnvil::ValkyrieRunner;

BEGIN_EVENT_TABLE(UIAppValkyrieRunner, wxApp)
    EVT_MENU(wxID_ABOUT,            UIAppValkyrieRunner::onAbout)
    EVT_MENU(wxID_EXIT,             UIAppValkyrieRunner::onQuit)
    EVT_BUTTON(BUTTON_SAVE_ID,      UIAppValkyrieRunner::onSave)
    EVT_BUTTON(BUTTON_RUN1_ID,      UIAppValkyrieRunner::onRun1)
    EVT_BUTTON(BUTTON_SORT_ID,      UIAppValkyrieRunner::onSort)
    EVT_BUTTON(BUTTON_UPDATE_ID,    UIAppValkyrieRunner::onUpdate)
    EVT_BUTTON(BUTTON_EVOLVE_ID,    UIAppValkyrieRunner::onEvolve)
    EVT_BUTTON(BUTTON_RUN1K_ID,     UIAppValkyrieRunner::onRun1K)
END_EVENT_TABLE()

UIAppValkyrieRunner::UIAppValkyrieRunner()
    : distanceGraph(runners)
    , distanceHotMap(runners)
    , speciesGraph(runners)
{}

bool UIAppValkyrieRunner::OnInit()
{
    wxMenu* fileMenu        = new wxMenu;
    fileMenu->Append(wxID_EXIT,         wxT("E&xit\tAlt-X"),  wxT("Quit Valkyrie Runner"));

    wxMenu* helpMenu        = new wxMenu;
    helpMenu->Append(wxID_ABOUT, wxT("&About...\tF1"), wxT("Show about dialog"));

    wxMenuBar* menuBar      = new wxMenuBar{};
    menuBar->Append(fileMenu, wxT("&File"));
    menuBar->Append(helpMenu, wxT("&Help"));

    wxFrame*    frameButton = make_FrameSimpleHorz(nullptr, wxID_ANY, wxT("Runner Button"), wxPoint{100, 100}, buttonBuilder);
    frameButton->SetMenuBar(menuBar);
    frameButton->CreateStatusBar(1);
    frameButton->SetStatusText(wxT("Welcome to Valkyrie"));
    frameButton->Fit();
    frameButton->Show();
    wxSize  sizeButton      = frameButton->GetSize();

    wxFrame*    frameGraph  = make_FrameSimpleHorz(nullptr, wxID_ANY, wxT("Runner Graph"), wxPoint{100, 100 + sizeButton.y},
                                                   lineGraph);
                                                   //distanceGraph, speciesGraph, distanceHotMap);
    frameGraph->Show();
    wxSize      sizeGraph   = frameGraph->GetSize();

    wxFrame*    frameWalker = make_FrameSimpleHorz(nullptr, wxID_ANY, wxT("Runner Walker Farm"), wxPoint{100, 100 + sizeGraph.y + sizeButton.y}, tmp);
    frameWalker->Show();

    return true;
}
