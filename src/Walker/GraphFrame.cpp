#include "GraphFrame.h"
#include "Walker.h"

using namespace ThorsAnvil::ValkyrieWalker;

BEGIN_EVENT_TABLE(GraphFrame, wxFrame)
END_EVENT_TABLE()

GraphFrame::GraphFrame(std::vector<Walker>& walk)
    : wxFrame(nullptr, wxID_ANY , wxT("Valkyrie Graph"))
    , walkers(walk)
{
    (void)walkers;
}
