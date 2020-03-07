#include "UIFrameGraph.h"
#include "Walker.h"
#include "DrawableDistanceGraph.h"
#include "DrawableSpeciesGraph.h"
#include "ThorsUI/UIPanelDrawable.h"

using namespace ThorsAnvil::ValkyrieWalker;

BEGIN_EVENT_TABLE(FrameGraph, wxFrame)
END_EVENT_TABLE()

FrameGraph::FrameGraph(DrawableDistanceGraph& distanceGraph, DrawableSpeciesGraph& speciesGraph)
    : wxFrame(nullptr, wxID_ANY , wxT("Valkyrie Graph"))
{
    wxPanel* panelDistanceGraph = new ThorsUI::PanelDrawable(this, distanceGraph);
    wxPanel* panelSpeciesGraph  = new ThorsUI::PanelDrawable(this, speciesGraph);

    wxSizer* sizer              = new wxBoxSizer(wxHORIZONTAL);
    sizer->Add(panelDistanceGraph, wxSizerFlags());
    sizer->Add(panelSpeciesGraph,  wxSizerFlags());

    SetSizerAndFit(sizer);
}
