#ifndef THORSANVIL_VALKYRIE_RUNNER_UI_DRAWABLE_SPECIES_GRAPH_H
#define THORSANVIL_VALKYRIE_RUNNER_UI_DRAWABLE_SPECIES_GRAPH_H

#include "ThorsUI/Drawable.h"
#include <map>
#include <vector>
#include <string>

namespace ThorsAnvil
{
    namespace ValkyrieRunner
    {

class Runner;
class UIDrawableSpeciesGraph: public ThorsAnvil::ThorsUI::Drawable
{
    using Counter       = std::vector<int>;
    using SpecCounter   = std::map<std::string, Counter>;

    std::vector<Runner> const&  runners;
    SpecCounter                 speciesCount;
    int                         count;

    public:
        UIDrawableSpeciesGraph(std::vector<Runner> const& runners);

        virtual void    draw(wxDC& dc)  const override;
        virtual wxSize  getSize()       const override;

        void tick(bool update = true);
        void load(std::istream& stream);
        void save(std::ostream& stream) const;

        friend std::istream& operator>>(std::istream& stream, UIDrawableSpeciesGraph& data)
        {
            data.load(stream);
            return stream;
        }
        friend std::ostream& operator<<(std::ostream& stream, UIDrawableSpeciesGraph const& data)
        {
            data.save(stream);
            return stream;
        }
};


    }
}

#endif
