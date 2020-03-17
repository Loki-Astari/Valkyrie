#include "UIDrawableDistanceGraph.h"
#include "Runner.h"
#include <set>

using namespace ThorsAnvil::ValkyrieRunner;

UIDrawableDistanceGraph::UIDrawableDistanceGraph(std::vector<Runner> const& runners)
    : runners(runners)
    , distance(20)
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
        std::set<std::size_t>    smallSet{0, 7, 9, 11, 13, 19};

        for (std::size_t lineId = 0; lineId < distance.size(); ++lineId)
        {
            if (drawSmall && smallSet.find(lineId) == smallSet.end())
            {
                continue;
            }
            std::vector<wxPoint>    points(count);

            for (std::size_t loop = 0; loop < distance[lineId].size(); ++loop)
            {
                points[loop]    = wxPoint{static_cast<int>(centX + distXRel * loop), static_cast<int>(centY - distYRel * distance[lineId][loop])};
            }
            dc.DrawLines(count, points.data());
        }
    }
}

void UIDrawableDistanceGraph::tick(bool update)
{
    distance[0].emplace_back(runners[0].score());
    distance[1].emplace_back(runners[1].score());
    distance[2].emplace_back(runners[2].score());
    distance[3].emplace_back(runners[3].score());
    distance[4].emplace_back(runners[4].score());
    distance[5].emplace_back(runners[5].score());
    distance[6].emplace_back(runners[10].score());
    distance[7].emplace_back(runners[20].score());
    distance[8].emplace_back(runners[30].score());
    distance[9].emplace_back(runners[40].score());
    distance[10].emplace_back(runners[50].score());
    distance[11].emplace_back(runners[60].score());
    distance[12].emplace_back(runners[70].score());
    distance[13].emplace_back(runners[80].score());
    distance[14].emplace_back(runners[90].score());
    distance[15].emplace_back(runners[95].score());
    distance[16].emplace_back(runners[96].score());
    distance[17].emplace_back(runners[97].score());
    distance[18].emplace_back(runners[98].score());
    distance[19].emplace_back(runners[99].score());

    maxValue = std::max(maxValue, distance[0].back());
    minValue = std::min(minValue, distance[19].back());

    ++count;
    if (update)
    {
        refresh();
    }
}

void UIDrawableDistanceGraph::load(std::istream& stream)
{
    std::vector<std::vector<int>>   tmpDistance;
    int                             tmpMinValue;
    int                             tmpMaxValue;
    int                             tmpCount;
    int                             sizeCount;
    if (stream >> tmpMinValue >> tmpMaxValue >> tmpCount >> sizeCount)
    {
        for (int loop = 0; loop < sizeCount; ++loop)
        {
            int  count;
            if (stream >> count)
            {
                tmpDistance.emplace_back();
                for (int index = 0; index < count; ++index)
                {
                    int val;
                    if (stream >> val)
                    {
                        tmpDistance.back().emplace_back(val);
                    }
                }
            }
        }
    }
    if (stream)
    {
        distance    = std::move(tmpDistance);
        minValue    = tmpMinValue;
        maxValue    = tmpMaxValue;
        count       = tmpCount;
        drawSmall   = true;
    }
}

void UIDrawableDistanceGraph::save(std::ostream& stream) const
{
    stream << minValue << " " << maxValue << " " << count << " " << distance.size() << "\n";
    for (auto const& row: distance)
    {
        stream << row.size();
        for (auto const& val: row)
        {
            stream << " " << val;
        }
        stream << "\n";
    }
}
