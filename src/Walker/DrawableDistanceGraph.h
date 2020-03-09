#ifndef THORSANVIL_VALKYRIE_WALKER_DRAWABLE_DISTANCE_GRAPH_H
#define THORSANVIL_VALKYRIE_WALKER_DRAWABLE_DISTANCE_GRAPH_H

#include "ThorsUI/Drawable.h"
#include <vector>

namespace ThorsAnvil
{
    namespace ValkyrieWalker
    {

class Walker;
class DrawableDistanceGraph: public ThorsAnvil::ThorsUI::Drawable
{
    std::vector<Walker> const&      walkers;
    std::vector<std::vector<int>>   distance;
    int                             minValue;
    int                             maxValue;
    int                             count;
    bool                            drawSmall;

    public:
        DrawableDistanceGraph(std::vector<Walker> const& walkers);

        virtual void    draw(wxDC& dc)  const override;
        virtual wxSize  getSize()       const override;

        void tick(bool update = true);
};


    }
}

#endif
