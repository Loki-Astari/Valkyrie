#ifndef THORSANVIL_VALKYRIE_WALKER_DRAWABLE_SPECIES_GRAPH_H
#define THORSANVIL_VALKYRIE_WALKER_DRAWABLE_SPECIES_GRAPH_H

#include "ThorsUI/Drawable.h"
#include <map>
#include <vector>
#include <string>

namespace ThorsAnvil
{
    namespace ValkyrieWalker
    {

class Walker;
class DrawableSpeciesGraph: public ThorsAnvil::ThorsUI::Drawable
{
    using Counter       = std::vector<int>;
    using SpecCounter   = std::map<std::string, Counter>;

    std::vector<Walker> const&  walkers;
    SpecCounter                 speciesCount;
    int                         count;

    public:
        DrawableSpeciesGraph(std::vector<Walker> const& walkers);

        virtual void    draw(wxDC& dc)  const override;
        virtual wxSize  getSize()       const override;

        void tick(bool update = true);
};


    }
}

#endif
