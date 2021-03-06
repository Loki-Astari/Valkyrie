#ifndef THORSANVIL_VALKYRIE_WALKER_UI_PANEL_WALKER_CROWD_H
#define THORSANVIL_VALKYRIE_WALKER_UI_PANEL_WALKER_CROWD_H

#include "Walker.h"
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
        PanelWalkerCrowd&       parent;
        Walker&                 walker;
        float                   scale;
        mutable int             flashBorderTime;
        mutable wxPen           flashBorderPen;
        mutable int             flashBackgroundTime;
        mutable wxBrush         flashBackgroundBrush;

        public:
            WalkerButton(PanelWalkerCrowd& parent, Walker& w, float scale);
            virtual void    draw(wxDC& dc)  const override;
            virtual wxSize  getSize()       const override;

            void kill()                                 {walker.kill();}
            bool wasKilled() const                      {return walker.wasKilled();}
            void mutate()                               {walker.mutate();}
            void spawn(WalkerButton const& parent)      {walker.spawn(parent.walker);}

            void refresh()                              {parent.Refresh();}
            void flashBorder(int time, wxPen const& pen);
            void flashBackground(int time, wxBrush const& brush);
    };

    std::vector<Walker>&            walkers;
    std::vector<WalkerButton>       buttons;
    wxSizer*                        panelSizer;
    bool                            background;

    public:
        PanelWalkerCrowd(wxWindow* parent, std::vector<Walker>& walkers);

        void shuffle();
        void evolve();
    private:
        virtual wxSize  getSize()                           const override;
        virtual void    animateResetActionDone(wxDC& dc)          override;
};


    }
}

#endif
