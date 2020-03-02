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

    std::vector<WalkerButton>       buttons;
    std::vector<int>                newOrder;

    public:
        PanelWalkerCrowd(wxWindow* parent, std::vector<Walker>& walkers);

        void shuffle(std::vector<int>&& nO);
    private:
        virtual wxSize  getSize()                           const override;
};


    }
}

#endif
