#ifndef THORSANVIL_VALKYRIE_WALKER_DRAWABLE_SPECIES_GRAPH_H
#define THORSANVIL_VALKYRIE_WALKER_DRAWABLE_SPECIES_GRAPH_H

#include "ThorsUI/Drawable.h"

namespace ThorsAnvil
{
    namespace ValkyrieWalker
    {

class Walker;
class DrawableSpeciesGraph: public ThorsAnvil::ThorsUI::Drawable
{
    std::vector<Walker>&            walkers;

    public:
        DrawableSpeciesGraph(std::vector<Walker>& walkers);

        virtual void    draw(wxDC& dc)  const override;
        virtual wxSize  getSize()       const override;
};


    }
}

#endif