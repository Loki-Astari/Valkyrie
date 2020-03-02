#ifndef THORSANVIL_VALKYRIE_WALKER_UI_PANEL_WALKER_CROWD_H
#define THORSANVIL_VALKYRIE_WALKER_UI_PANEL_WALKER_CROWD_H

#include "ThorsUI/Animateable.h"
#include "ThorsUI/UIPanelAnimateableRunner.h"
#include "ThorsUI/UIPanelSpriteRunner.h"
#include "ThorsUtil/Util.h"

namespace ThorsAnvil
{
    namespace ValkyrieWalker
    {

class Walker;
class PanelWalkerCrowd: public ThorsAnvil::ThorsUI::PanelSpriteRunner
{
    class WalkerButton: public ThorsAnvil::ThorsUI::Drawable
    {
        Walker&     walker;
        float       scale;
        public:
            WalkerButton(Walker& w, float scale);
            virtual void    draw(wxDC& dc)  const override;
            virtual wxSize  getSize()       const override;
    };

    std::vector<Walker>&            walkers;
    std::vector<WalkerButton>       buttons;
    wxSizer*                        panelSizer;

    public:
        PanelWalkerCrowd(wxWindow* parent, std::vector<Walker>& walkers);

        void shuffle();
    private:
        virtual wxSize  getSize()                           const override;
        virtual void    animateResetActionDone(wxDC& dc)          override;
};


    }
}

#endif
