#include "ValkyrieRunnerApp.h"
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
            return wxSize(500, 500);
        }
};

Tmp     tmp;

bool ValkyrieRunnerApp::OnInit()
{
    frameButton     = make_FrameSimpleHorz(nullptr, wxID_ANY , wxT("Runner Button"),        wxPoint{100, 100}, tmp);
    frameGraph      = make_FrameSimpleHorz(nullptr, wxID_ANY , wxT("Runner Graph"),         wxPoint{100, 600}, tmp);
    frameWalkerFarm = make_FrameSimpleHorz(nullptr, wxID_ANY , wxT("Runner Walker Farm"),   wxPoint{600, 100}, tmp);

    frameButton->Show();
    frameGraph->Show();
    frameWalkerFarm->Show();
    return true;
}
