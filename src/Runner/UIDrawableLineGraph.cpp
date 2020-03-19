#include "UIDrawableLineGraph.h"
#include <algorithm>

using namespace ThorsAnvil::ValkyrieRunner;

const wxColor UIDrawableLineGraph::lineColors[] = {*wxBLACK, *wxBLUE, *wxCYAN, *wxGREEN, *wxYELLOW, *wxLIGHT_GREY, *wxRED, *wxWHITE};

UIDrawableLineGraph::UIDrawableLineGraph()
    : maxPoint(0)
    , minPoint(0)
    , nextColour(0)
    , pointCount(0)
{
    addPoints("A Line going up",      {0, 10, 12, 18, 30, 45, 100, 110, 112, 123});
    addPoints("Bad person",           {0, -2, -8, -8, -2, -6, -22, -34, -34, -40});
    addPoints("A Person interest Now",{-40, 40, -40, 40, -40, 40, -40, 40, -40, 40});
    addPoints("Summer Wine",          {0, 10, 20, 30, 40 , 50 , 60, 70, 80, 90});
    addPoints("Tik Tak Wine",         {0, 10, 20, 30, 40 , 50 , 60, 70, 80, 100});
}

void UIDrawableLineGraph::draw(wxDC& dc) const
{
    wxSize      maxNameSize;
    wxSize      maxValueSize;
    std::tie(maxNameSize, maxValueSize) = calcLegendStats(dc);

    wxSize  fullSize    = getSize();
    wxSize  graphSize   = drawLegend(dc, fullSize, maxNameSize);
    Area    graphArea   = drawAxis(dc, graphSize, maxValueSize);
    drawGraphs(dc, graphArea);
}

wxSize UIDrawableLineGraph::getSize(wxDC& dc, std::string const& text) const
{
    wxCoord     width;
    wxCoord     height;

    dc.GetTextExtent(text.c_str(), &width, &height);
    return wxSize(width, height);
}

UIDrawableLineGraph::Spec UIDrawableLineGraph::calcLegendStats(wxDC& dc) const
{
    wxSize      valueSizeMax = getSize(dc, std::to_string(maxPoint));
    wxSize      valueSizeMin = getSize(dc, std::to_string(minPoint));
    wxSize      valueSize{std::max(valueSizeMax.x, valueSizeMin.x), std::max(valueSizeMax.y, valueSizeMin.y)};

    wxSize      nameSize(0,0);
    for (auto const& line: lines)
    {
        wxSize size = getSize(dc, line.first);
        nameSize.x  = std::max(nameSize.x, size.x);
        nameSize.y  = std::max(nameSize.y, size.y);
    }

    return Spec(nameSize, valueSize);
}

void UIDrawableLineGraph::drawGraphs(wxDC& dc, Area const& graphArea) const
{
    wxPoint const&  graphOffset = graphArea.first;
    wxSize  const&  graphSize   = graphArea.second;
    float   const   xScale      = graphSize.x * 1.0 / (pointCount - 1);
    float   const   yScale      = graphSize.y * 1.0 / (maxPoint - minPoint);
    int     const   yOffset     = yScale * maxPoint;

    wxPen pen = dc.GetPen();

    for (auto const& line: lines)
    {
        LineInfo const& info = line.second;

        // Set the colour we will use for the line
        pen.SetColour(info.lineColor);
        dc.SetPen(pen);

        std::vector<wxPoint>    arc;
        arc.reserve(info.points.size());
        int xPoint = 0;
        for (auto const& point: info.points)
        {
            arc.emplace_back(xPoint * xScale, -point * yScale);
            ++xPoint;
        }

        dc.DrawLines(arc.size(), arc.data(), graphOffset.x, graphOffset.y + yOffset);
    }
}

UIDrawableLineGraph::Area UIDrawableLineGraph::drawAxis(wxDC& dc, wxSize const& size, wxSize const& maxValueSize) const
{
    // Get a copy of the pen and set global parameters for drawing lines
    wxPen pen = dc.GetPen();

    wxPoint graphOffset = {gap + maxValueSize.x, gap};
    wxSize  graphSize   = {size.x - maxValueSize.x - 2*gap, size.y - 2*gap};
    float   yScale      = graphSize.y * 1.0 / (maxPoint - minPoint);
    int     yOffset     = yScale * maxPoint;

    pen.SetWidth(2);
    pen.SetColour(wxTransparentColour);
    dc.SetPen(pen);
    dc.DrawRectangle(graphOffset, graphSize);

    wxSize  textSize;

    textSize = getSize(dc, std::to_string(maxPoint));
    dc.DrawText(std::to_string(maxPoint).c_str(), wxPoint{maxValueSize.x - textSize.x, 0});

    textSize = getSize(dc, "0");
    dc.DrawText("0", wxPoint{maxValueSize.x - textSize.x, static_cast<int>(graphOffset.y + yScale * maxPoint - maxValueSize.y/2)});

    textSize = getSize(dc, std::to_string(minPoint));
    dc.DrawText(std::to_string(minPoint).c_str(), wxPoint{maxValueSize.x - textSize.x, size.y - maxValueSize.y});

    pen.SetColour(*wxBLACK);
    dc.SetPen(pen);

    dc.DrawLine(graphOffset, wxPoint{graphOffset.x, graphOffset.y + graphSize.y});
    dc.DrawLine(wxPoint{graphOffset.x, graphOffset.y + yOffset}, wxPoint{graphOffset.x + graphSize.x, graphOffset.y + yOffset});

    return Area(graphOffset, graphSize);
}

wxSize UIDrawableLineGraph::drawLegend(wxDC& dc, wxSize const& size, wxSize const& maxNameSize) const
{
    // Work out how we are going to draw the legend.
    int     pointsPerLegend     = maxNameSize.x + lineLen;
    int     legendsPerRow       = (size.x - gap) / pointsPerLegend;
    int     noLines             = lines.size();
    int     rowsNeeded          = noLines / legendsPerRow;
    int     maxLegendsPerRow    = noLines < legendsPerRow ? noLines : legendsPerRow;
    rowsNeeded += ((noLines % legendsPerRow) == 0 ? 0 : 1);

    // Set up the position we will write text
    int center  = (size.x - (legendsPerRow * pointsPerLegend)) / 2;
    int top     = size.y - gap - rowsNeeded * maxNameSize.y;
    int y       = top;
    int x       = center;

    dc.DrawRectangle(wxPoint{center - gap/2, y - gap/2}, wxSize{pointsPerLegend * maxLegendsPerRow + gap, rowsNeeded * maxNameSize.y + gap});

    // Get a copy of the pen and set global parameters for drawing lines
    wxPen pen = dc.GetPen();
    pen.SetWidth(2);

    int item = 0;
    for (auto const& line: lines)
    {
        // Set the colour we will use for the line
        pen.SetColour(line.second.lineColor);
        dc.SetPen(pen);

        // Draw the name of the row and piece of line in the correct colour
        dc.DrawText(line.first, wxPoint{x, y});
        dc.DrawLine(wxPoint{x + maxNameSize.x + 5, y + maxNameSize.y/2}, wxPoint{x + maxNameSize.x + 25, y + maxNameSize.y/2});

        // Update the drawing position
        // At the end of a row we move down and reset the x position.
        ++item;
        x += pointsPerLegend;
        if (item % legendsPerRow == 0)
        {
            x = center;
            y += maxNameSize.y;
        }
    }
    return wxSize(size.x, top - gap/2);
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

    pointCount= std::max(pointCount, static_cast<int>(data.points.size()));
}
