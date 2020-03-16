#include "ValkyrieRunnerApp.h"
#include "PanelButtonBuilder.h"
#include "ThorsUI/UIFrameSimple.h"

using namespace ThorsAnvil::ValkyrieRunner;

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
