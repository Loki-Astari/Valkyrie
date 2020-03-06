#include "DrawableDistanceGraph.h"
#include "Walker.h"

using namespace ThorsAnvil::ValkyrieWalker;

DrawableDistanceGraph::DrawableDistanceGraph(std::vector<Walker>& walkersR)
    : walkers(walkersR)
{
    (void)walkers;
}

void DrawableDistanceGraph::draw(wxDC& /*dc*/) const
{
}

wxSize DrawableDistanceGraph::getSize() const
{
    return wxSize(200, 200);
}

void DrawableDistanceGraph::tick()
{
}
