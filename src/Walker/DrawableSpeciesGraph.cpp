#include "DrawableSpeciesGraph.h"
#include "Walker.h"

using namespace ThorsAnvil::ValkyrieWalker;

DrawableSpeciesGraph::DrawableSpeciesGraph(std::vector<Walker> const& walkersR)
    : walkers(walkersR)
    , count(0)
{
    (void)walkers;
}

void DrawableSpeciesGraph::draw(wxDC& dc) const
{
    wxSize size = getSize();
    dc.SetPen(wxNullPen);
    dc.DrawRectangle(wxPoint{0,0}, size);

    float   distYRel    = (size.y - 20) * 1.0 / (walkers.size() - 1);
    float   distXRel    = (size.x - 20) * 1.0 / (count == 1 ? 1 : count - 1);

    dc.SetPen(*wxBLACK_PEN);
    std::vector<int>    total(count, 0);
    for (auto& specCount: speciesCount)
    {
        Counter const&          counter = specCount.second;
        std::vector<wxPoint>    points;
        points.reserve(count * 2);

        for (int loop = 0; loop < count; ++loop)
        {
            points.emplace_back(loop * distXRel, total[loop] * distYRel);
            total[loop] += counter[loop];
        }
        if (count == 1)
        {
            points.emplace_back(distXRel, (total[0] - counter[0]) * distYRel);
            points.emplace_back(distXRel, total[0] * distYRel);
        }
        for (int loop = 0; loop < count; ++loop)
        {
            int index = count - 1 - loop;
            points.emplace_back(index * distXRel, total[index] * distYRel);
        }
        dc.DrawPolygon(points.size(), points.data(), 10, 10);
    }
}

wxSize DrawableSpeciesGraph::getSize() const
{
    return wxSize(400, 400);
}

extern wxPanel* panelSpeciesGraph;

void DrawableSpeciesGraph::tick(bool update)
{
    ++count;
    for (auto& counter: speciesCount)
    {
        counter.second.emplace_back(0);
    }
    for (auto const& walker: walkers)
    {
        std::string species = walker.species();
        auto find = speciesCount.find(species);
        if (find == std::end(speciesCount))
        {
            auto insert = speciesCount.emplace(species, std::vector<int>(count));
            ++(insert.first->second.back());
        }
        else
        {
            ++(find->second.back());
        }
    }
    if (panelSpeciesGraph && update)
    {
        panelSpeciesGraph->Refresh();
    }
}
