#ifndef THORSANVIL_VALKYRIE_WALKER_DRAWABLE_DISTANCE_GRAPH_H
#define THORSANVIL_VALKYRIE_WALKER_DRAWABLE_DISTANCE_GRAPH_H

#include "ThorsUI/Drawable.h"

namespace ThorsAnvil
{
    namespace ValkyrieWalker
    {

class Walker;
class DrawableDistanceGraph: public ThorsAnvil::ThorsUI::Drawable
{
    std::vector<Walker>&            walkers;

    public:
        DrawableDistanceGraph(std::vector<Walker>& walkers);

        virtual void    draw(wxDC& dc)  const override;
        virtual wxSize  getSize()       const override;

        void tick();
};


    }
}

#endif
