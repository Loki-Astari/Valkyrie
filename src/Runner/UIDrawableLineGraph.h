#ifndef THORSANVIL_VALKYRIE_RUNNER_UI_DRAWABLE_LINE_GRAPH_H
#define THORSANVIL_VALKYRIE_RUNNER_UI_DRAWABLE_LINE_GRAPH_H

#include "ThorsUI/Drawable.h"
#include <map>
#include <vector>
#include <initializer_list>

namespace ThorsAnvil
{
    namespace ValkyrieRunner
    {

class UIDrawableLineGraph: public ThorsAnvil::ThorsUI::Drawable
{
    static constexpr int gap        = 5;
    static constexpr int lineLen    = 30;
    static const wxColor lineColors[];

    struct LineInfo
    {
        int                 minPoint;
        int                 maxPoint;
        wxColour            lineColor;
        std::vector<int>    points;
    };
    using LineMap   = std::map<std::string, LineInfo>;
    using LineValue = LineMap::value_type;

    LineMap     lines;
    int         maxPoint;
    int         minPoint;
    int         nextColour;

    public:
        UIDrawableLineGraph();

        virtual void    draw(wxDC& dc)  const override;
        virtual wxSize  getSize()       const override;

    private:
        void drawLegend(wxDC& dc) const;
        void addPoints(std::string const& name, std::initializer_list<int> const& list);
        void addPoint(std::string const& name, int point);
};


    }
}

#endif
