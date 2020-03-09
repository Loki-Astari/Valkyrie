#ifndef THORSANVIL_VALKYRIE_WALKER_DRAWABLE_DISTANCE_HOT_MAP_H
#define THORSANVIL_VALKYRIE_WALKER_DRAWABLE_DISTANCE_HOT_MAP_H

#include "ThorsUI/Drawable.h"
#include <vector>

namespace ThorsAnvil
{
    namespace ValkyrieWalker
    {

class Walker;
class DrawableDistanceHotMap: public ThorsAnvil::ThorsUI::Drawable
{
    std::vector<Walker> const&      walkers;
    std::vector<int>                count;
    int                             minScore;
    int                             maxScore;


    public:
        DrawableDistanceHotMap(std::vector<Walker> const& walkers);

        virtual void    draw(wxDC& dc)  const override;
        virtual wxSize  getSize()       const override;

        void tick(bool update = true);
};


    }
}

#endif
