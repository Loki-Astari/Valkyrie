#include "ValkyrieRunnerApp.h"
#include "PanelButtonBuilder.h"
#include "ThorsUI/UIFrameSimple.h"

using namespace ThorsAnvil::ValkyrieRunner;

BEGIN_EVENT_TABLE(ValkyrieRunnerApp, wxApp)
    EVT_MENU(wxID_ABOUT,            ValkyrieRunnerApp::onAbout)
    EVT_MENU(wxID_EXIT,             ValkyrieRunnerApp::onQuit)
    EVT_BUTTON(BUTTON_SAVE_ID,      ValkyrieRunnerApp::onSave)
    EVT_BUTTON(BUTTON_RUN1_ID,      ValkyrieRunnerApp::onRun1)
    EVT_BUTTON(BUTTON_SORT_ID,      ValkyrieRunnerApp::onSort)
    EVT_BUTTON(BUTTON_UPDATE_ID,    ValkyrieRunnerApp::onUpdate)
    EVT_BUTTON(BUTTON_EVOLVE_ID,    ValkyrieRunnerApp::onEvolve)
    EVT_BUTTON(BUTTON_RUN1K_ID,     ValkyrieRunnerApp::onRun1K)
END_EVENT_TABLE()

ValkyrieRunnerApp::ValkyrieRunnerApp()
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
PanelButtonBuilder  buttonBuilder;

bool ValkyrieRunnerApp::OnInit()
{
    frameButton         = make_FrameSimpleHorz(nullptr, wxID_ANY , wxT("Runner Button"),    wxPoint{100, 100}, buttonBuilder);
    wxSize  sizeButton  = frameButton->GetSize();

    frameGraph          = make_FrameSimpleHorz(nullptr, wxID_ANY , wxT("Runner Graph"),     wxPoint{100, 100 + sizeButton.y}, tmp);
    wxSize  sizeGraph   = frameGraph->GetSize();

    frameWalkerFarm     = make_FrameSimpleHorz(nullptr, wxID_ANY , wxT("Runner Walker Farm"),   wxPoint{100, 100 + sizeGraph.y + sizeButton.y}, tmp);

    frameButton->Show();
    frameGraph->Show();
    frameWalkerFarm->Show();
    return true;
}
