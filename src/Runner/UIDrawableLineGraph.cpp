#include "UIDrawableLineGraph.h"
#include <algorithm>

using namespace ThorsAnvil::ValkyrieRunner;

const wxColor UIDrawableLineGraph::lineColors[] = {*wxBLACK, *wxBLUE, *wxCYAN, *wxGREEN, *wxYELLOW, *wxLIGHT_GREY, *wxRED, *wxWHITE};

UIDrawableLineGraph::UIDrawableLineGraph()
    : maxPoint(0)
    , minPoint(0)
    , nextColour(0)
{
    addPoints("A Line going up", {0, 10, 12, 18, 30, 45, 100, 110, 112, 123});
    addPoints("Bad person",      {0, -2, -8, -8, -2, -6, -22, -34, -34, -40});
}

void UIDrawableLineGraph::draw(wxDC& dc) const
{
    drawLegend(dc);
}

void UIDrawableLineGraph::drawLegend(wxDC& dc) const
{
    wxCoord     maxNameWidth  = 0;
    wxCoord     maxNameHeight = 0;
    wxCoord     width;
    wxCoord     height;
    for (auto const& line: lines)
    {
        dc.GetTextExtent(line.first, &width, &height);
        maxNameWidth    = std::max(maxNameWidth, width);
        maxNameHeight   = std::max(maxNameHeight,height);
    }

    wxSize  size = getSize();
    int     pointsPerLegend     = maxNameWidth + lineLen;
    int     legendsPerRow       = size.x / pointsPerLegend;
    int     noLines             = lines.size();
    int     rowsNeeded          = noLines / legendsPerRow;
    int     maxLegendsPerRow    = noLines < legendsPerRow ? noLines : legendsPerRow;
    rowsNeeded += ((noLines % legendsPerRow) == 0 ? 0 : 1);

    int y    = size.y - rowsNeeded * (maxNameHeight + gap);
    int x    = gap;
    int item = 0;
    dc.DrawRectangle(wxPoint{gap/2, y - gap/2}, wxSize{pointsPerLegend * maxLegendsPerRow + gap, rowsNeeded * maxNameHeight + gap});
    for (auto const& line: lines)
    {
        dc.DrawText(line.first, wxPoint{x, y});
        wxPen pen = dc.GetPen();
        pen.SetColour(line.second.lineColor);
        pen.SetWidth(2);
        dc.SetPen(pen);
        dc.DrawLine(wxPoint{x + maxNameWidth + 5, y + maxNameHeight/2}, wxPoint{x + maxNameWidth + 25, y + maxNameHeight/2});
        x += pointsPerLegend;
        ++item;
        if (item == legendsPerRow)
        {
            x = gap;
            y += (maxNameHeight + gap);
        }
    }
}

wxSize UIDrawableLineGraph::getSize() const
{
    return wxSize(400, 400);
}

void UIDrawableLineGraph::addPoints(std::string const& name, std::initializer_list<int> const& list)
{
    for (auto point: list)
    {
        addPoint(name, point);
    }
}

void UIDrawableLineGraph::addPoint(std::string const& name, int point)
{
    auto find = lines.find(name);
    if (find == std::end(lines))
    {
        auto insert = lines.insert(LineValue{name, LineInfo{}});
        find = insert.first;
        find->second.lineColor = lineColors[nextColour];
        nextColour = (nextColour + 1) % std::size(lineColors);
    }
    LineInfo& data = find->second;
    data.maxPoint = std::max(data.maxPoint, point);
    data.minPoint = std::min(data.minPoint, point);
    data.points.emplace_back(point);

    maxPoint = std::max(maxPoint, point);
    minPoint = std::min(minPoint, point);
}
