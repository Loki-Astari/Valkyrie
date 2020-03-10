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
        void load(std::istream& stream);
        void save(std::ostream& stream) const;

        friend std::istream& operator>>(std::istream& stream, DrawableDistanceHotMap& data)
        {
            data.load(stream);
            return stream;
        }
        friend std::ostream& operator<<(std::ostream& stream, DrawableDistanceHotMap const& data)
        {
            data.save(stream);
            return stream;
        }
};


    }
}

#endif
