#include "UIValkyrieRunnerApp.h"
#include "UIPanelButtonBuilder.h"
#include "ThorsUI/UIFrameSimple.h"

using namespace ThorsAnvil::ValkyrieRunner;

BEGIN_EVENT_TABLE(UIValkyrieRunnerApp, wxApp)
    EVT_MENU(wxID_ABOUT,            UIValkyrieRunnerApp::onAbout)
    EVT_MENU(wxID_EXIT,             UIValkyrieRunnerApp::onQuit)
    EVT_BUTTON(BUTTON_SAVE_ID,      UIValkyrieRunnerApp::onSave)
    EVT_BUTTON(BUTTON_RUN1_ID,      UIValkyrieRunnerApp::onRun1)
    EVT_BUTTON(BUTTON_SORT_ID,      UIValkyrieRunnerApp::onSort)
    EVT_BUTTON(BUTTON_UPDATE_ID,    UIValkyrieRunnerApp::onUpdate)
    EVT_BUTTON(BUTTON_EVOLVE_ID,    UIValkyrieRunnerApp::onEvolve)
    EVT_BUTTON(BUTTON_RUN1K_ID,     UIValkyrieRunnerApp::onRun1K)
END_EVENT_TABLE()

UIValkyrieRunnerApp::UIValkyrieRunnerApp()
    : frameButton(nullptr)
    , frameGraph(nullptr)
    , frameWalkerFarm(nullptr)
{
}

class Tmp: public ThorsAnvil::ThorsUI::Drawable
{
    public:
        virtual void    draw(wxDC& /*dc*/)  const override
        {
        }
        virtual wxSize  getSize()       const override
        {
            return wxSize(100, 100);
        }
};

Tmp                 tmp;
UIPanelButtonBuilder  buttonBuilder;

bool UIValkyrieRunnerApp::OnInit()
{
    frameButton         = make_FrameSimpleHorz(nullptr, wxID_ANY , wxT("Runner Button"),    wxPoint{100, 100}, buttonBuilder);
    wxSize  sizeButton  = frameButton->GetSize();

    frameGraph          = make_FrameSimpleHorz(nullptr, wxID_ANY , wxT("Runner Graph"),     wxPoint{100, 100 + sizeButton.y}, tmp);
    wxSize  sizeGraph   = frameGraph->GetSize();

    frameWalkerFarm     = make_FrameSimpleHorz(nullptr, wxID_ANY , wxT("Runner Walker Farm"),   wxPoint{100, 100 + sizeGraph.y + sizeButton.y}, tmp);

    wxMenu* fileMenu = new wxMenu;
    fileMenu->Append(wxID_EXIT,         wxT("E&xit\tAlt-X"),  wxT("Quit Valkyrie Runner"));

    wxMenu* helpMenu = new wxMenu;
    helpMenu->Append(wxID_ABOUT, wxT("&About...\tF1"), wxT("Show about dialog"));

    wxMenuBar* menuBar = new wxMenuBar{};
    menuBar->Append(fileMenu, wxT("&File"));
    menuBar->Append(helpMenu, wxT("&Help"));

    frameButton->SetMenuBar(menuBar);
    frameButton->CreateStatusBar(2);
    frameButton->SetStatusText(wxT("Welcome to Valkyrie"));

    frameButton->Show();
    frameGraph->Show();
    frameWalkerFarm->Show();
    return true;
}
