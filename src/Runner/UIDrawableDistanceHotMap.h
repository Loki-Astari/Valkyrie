#ifndef THORSANVIL_VALKYRIE_RUNNER_UI_DRAWABLE_DISTANCE_HOT_MAP_H
#define THORSANVIL_VALKYRIE_RUNNER_UI_DRAWABLE_DISTANCE_HOT_MAP_H

#include "ThorsUI/Drawable.h"
#include <vector>

namespace ThorsAnvil
{
    namespace ValkyrieRunner
    {

class Runner;
class UIDrawableDistanceHotMap: public ThorsAnvil::ThorsUI::Drawable
{
    std::vector<Runner> const&      runners;
    std::vector<int>                count;
    int                             minScore;
    int                             maxScore;


    public:
        UIDrawableDistanceHotMap(std::vector<Runner> const& runners);

        virtual void    draw(wxDC& dc)  const override;
        virtual wxSize  getSize()       const override  {return wxSize(400, 400);}

        void tick(bool update = true);
        void load(std::istream& stream);
        void save(std::ostream& stream) const;

        friend std::istream& operator>>(std::istream& stream, UIDrawableDistanceHotMap& data)
        {
            data.load(stream);
            return stream;
        }
        friend std::ostream& operator<<(std::ostream& stream, UIDrawableDistanceHotMap const& data)
        {
            data.save(stream);
            return stream;
        }
};


    }
}

#endif
