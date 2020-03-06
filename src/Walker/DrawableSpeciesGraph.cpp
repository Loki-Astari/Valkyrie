#include "DrawableSpeciesGraph.h"
#include "Walker.h"

using namespace ThorsAnvil::ValkyrieWalker;

DrawableSpeciesGraph::DrawableSpeciesGraph(std::vector<Walker>& walkersR)
    : walkers(walkersR)
{
    (void)walkers;
}

void DrawableSpeciesGraph::draw(wxDC& /*dc*/) const
{
}

wxSize DrawableSpeciesGraph::getSize() const
{
    return wxSize(400, 400);
}
