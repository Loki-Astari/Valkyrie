#include "DrawableDistanceHotMap.h"
#include "Walker.h"

using namespace ThorsAnvil::ValkyrieWalker;

DrawableDistanceHotMap::DrawableDistanceHotMap(std::vector<Walker>& walkersR)
    : walkers(walkersR)
{
    (void)walkers;
}

void DrawableDistanceHotMap::draw(wxDC& dc) const
{
    /*
     * Draw graph.
     * Inside a 220, 220 square (see getSize())
     */

    wxSize size = getSize();
    dc.SetPen(wxNullPen);
    dc.DrawRectangle(wxPoint{0,0}, size);
}

wxSize DrawableDistanceHotMap::getSize() const
{
    return wxSize(400, 400);
}

extern wxPanel* panelDistanceMap;

void DrawableDistanceHotMap::tick()
{
    if (panelDistanceMap)
    {
        panelDistanceMap->Refresh();
    }
}
