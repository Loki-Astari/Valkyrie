#include "UIFrameGraph.h"
#include "Walker.h"

using namespace ThorsAnvil::ValkyrieWalker;

BEGIN_EVENT_TABLE(FrameGraph, wxFrame)
END_EVENT_TABLE()

FrameGraph::FrameGraph(std::vector<Walker>& walk)
    : wxFrame(nullptr, wxID_ANY , wxT("Valkyrie Graph"))
    , walkers(walk)
{
    (void)walkers;
}
