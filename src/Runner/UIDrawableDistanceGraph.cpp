#include "UIDrawableDistanceGraph.h"
#include "Runner.h"
#include <set>

using namespace ThorsAnvil::ValkyrieRunner;

UIDrawableDistanceGraph::UIDrawableDistanceGraph(std::vector<Runner> const& runners)
    : runners(runners)
    , distanceMajor(majorSize)
    , distanceMinor(minorSize)
    , minValue(0)
    , maxValue(0)
    , count(0)
    , drawSmall(true)
{}

void UIDrawableDistanceGraph::draw(wxDC& dc) const
{
    /*
     * Draw graph.
     * Inside a 220, 220 square (see getSize())
     */

    wxSize size = getSize();
    dc.SetPen(wxNullPen);
    dc.DrawRectangle(wxPoint{0,0}, size);

    int     distY       = (maxValue - minValue);
    float   distYRel    = (size.y - 20) * 1.0 / (distY < 1 ? 1 : distY);
    int     centY       = size.y - 10 - (distYRel * (-1 * minValue));

    float   distXRel    = size.x * 1.0 / (count <= 1 ? 1 : (count - 1));
    int     centX       = 10;

    dc.SetPen(*wxGREY_PEN);
    dc.DrawLine(wxPoint{0, centY}, wxPoint{size.x, centY});
    dc.DrawLine(wxPoint{centX, 0}, wxPoint{centX, size.y});

    if (count > 1)
    {
        dc.SetPen(*wxLIGHT_GREY_PEN);
        for (auto line: distanceMajor)
        {
            std::vector<wxPoint>    points;
            points.reserve(count);

            for (auto point: line)
            {
                points.emplace_back(wxPoint{static_cast<int>(centX + distXRel * points.size()), static_cast<int>(centY - distYRel * point)});
            }
            dc.DrawLines(count, points.data());
        }
    }
}

void UIDrawableDistanceGraph::tick()
{
    int oneP        = runners.size() / 100;
    int runners100  = 0;
    int runners80   = oneP * (100 - 80);
    int runners60   = oneP * (100 - 60);
    int runners40   = oneP * (100 - 40);
    int runners20   = oneP * (100 - 20);
    int runners00   = runners.size() - 1;

    distanceMajor[0].emplace_back(runners[runners100].score());
    distanceMajor[1].emplace_back(runners[runners80].score());
    distanceMajor[2].emplace_back(runners[runners60].score());
    distanceMajor[3].emplace_back(runners[runners40].score());
    distanceMajor[4].emplace_back(runners[runners20].score());
    distanceMajor[5].emplace_back(runners[runners00].score());


    int runners98   = oneP * (100 - 98);
    int runners96   = oneP * (100 - 96);
    int runners94   = oneP * (100 - 94);
    int runners92   = oneP * (100 - 92);
    int runners90   = oneP * (100 - 90);
    int runners10   = oneP * (100 - 10);
    int runners08   = oneP * (100 - 8);
    int runners06   = oneP * (100 - 6);
    int runners04   = oneP * (100 - 4);
    int runners02   = oneP * (100 - 2);
    distanceMinor[0].emplace_back(runners[runners98].score());
    distanceMinor[1].emplace_back(runners[runners96].score());
    distanceMinor[2].emplace_back(runners[runners94].score());
    distanceMinor[3].emplace_back(runners[runners92].score());
    distanceMinor[4].emplace_back(runners[runners90].score());
    distanceMinor[5].emplace_back(runners[runners10].score());
    distanceMinor[6].emplace_back(runners[runners08].score());
    distanceMinor[7].emplace_back(runners[runners06].score());
    distanceMinor[8].emplace_back(runners[runners04].score());
    distanceMinor[9].emplace_back(runners[runners02].score());

    maxValue = std::max(maxValue, distanceMajor.front().back());
    minValue = std::min(minValue, distanceMajor.back().back());

    ++count;
    refresh();
}

void UIDrawableDistanceGraph::load(std::istream& stream)
{
    std::vector<std::vector<int>>   tmpDistanceMajor;
    std::vector<std::vector<int>>   tmpDistanceMinor;
    int                             tmpMinValue;
    int                             tmpMaxValue;
    int                             tmpCount;
    if (stream >> tmpMinValue >> tmpMaxValue >> tmpCount)
    {
        for (int loop = 0; loop < majorSize; ++loop)
        {
            int  count;
            if (stream >> count)
            {
                tmpDistanceMajor.emplace_back();
                for (int index = 0; index < count; ++index)
                {
                    int val;
                    if (stream >> val)
                    {
                        tmpDistanceMajor.back().emplace_back(val);
                    }
                }
            }
        }
        for (int loop = 0; loop < minorSize; ++loop)
        {
            int  count;
            if (stream >> count)
            {
                tmpDistanceMinor.emplace_back();
                for (int index = 0; index < count; ++index)
                {
                    int val;
                    if (stream >> val)
                    {
                        tmpDistanceMinor.back().emplace_back(val);
                    }
                }
            }
        }
    }
    if (stream)
    {
        distanceMajor   = std::move(tmpDistanceMajor);
        distanceMinor   = std::move(tmpDistanceMinor);
        minValue        = tmpMinValue;
        maxValue        = tmpMaxValue;
        count           = tmpCount;
        drawSmall       = true;
    }
}

void UIDrawableDistanceGraph::save(std::ostream& stream) const
{
    stream << minValue << " " << maxValue << " " << count << " " << "\n";
    for (auto const& row: distanceMajor)
    {
        stream << row.size();
        for (auto const& val: row)
        {
            stream << " " << val;
        }
        stream << "\n";
    }
    for (auto const& row: distanceMinor)
    {
        stream << row.size();
        for (auto const& val: row)
        {
            stream << " " << val;
        }
        stream << "\n";
    }
}
