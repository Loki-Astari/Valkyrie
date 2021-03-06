#ifndef THORSANVIL_VALKYRIE_RUNNER_UI_DRAWABLE_DISTANCE_GRAPH_H
#define THORSANVIL_VALKYRIE_RUNNER_UI_DRAWABLE_DISTANCE_GRAPH_H

#include "ThorsUI/UIDrawableLineGraph.h"
#include <vector>
#include <iostream>

namespace ThorsAnvil
{
    namespace ValkyrieRunner
    {

class Runner;
class UIDrawableDistanceGraph: public ThorsAnvil::ThorsUI::UIDrawableLineGraph
{
    static constexpr int    majorSize = 6;
    static constexpr int    minorSize = 10;

    std::vector<Runner> const&      runners;

    public:
        UIDrawableDistanceGraph(std::vector<Runner> const& runners);

        virtual wxSize  getSize()       const override  {return wxSize(400, 400);}

        void tick();
        void load(std::istream& stream);
        void save(std::ostream& stream) const;

        friend std::istream& operator>>(std::istream& stream, UIDrawableDistanceGraph& data)
        {
            data.load(stream);
            return stream;
        }
        friend std::ostream& operator<<(std::ostream& stream, UIDrawableDistanceGraph const& data)
        {
            data.save(stream);
            return stream;
        }
};


    }
}

#endif
