#include "UIFrameGraph.h"
#include "Walker.h"
#include "DrawableDistanceGraph.h"
#include "DrawableSpeciesGraph.h"
#include "ThorsUI/UIPanelDrawable.h"

using namespace ThorsAnvil::ValkyrieWalker;

BEGIN_EVENT_TABLE(FrameGraph, wxFrame)
END_EVENT_TABLE()

wxPanel* panelDistanceGraph = nullptr;
wxPanel* panelSpeciesGraph  = nullptr;

FrameGraph::FrameGraph(DrawableDistanceGraph& distanceGraph, DrawableSpeciesGraph& speciesGraph)
    : wxFrame(nullptr, wxID_ANY , wxT("Valkyrie Graph"))
{
    panelDistanceGraph = new ThorsUI::PanelDrawable(this, distanceGraph);
    panelSpeciesGraph  = new ThorsUI::PanelDrawable(this, speciesGraph);

    wxSizer* sizer              = new wxBoxSizer(wxHORIZONTAL);
    sizer->Add(panelDistanceGraph, wxSizerFlags());
    sizer->AddSpacer(10);
    sizer->Add(panelSpeciesGraph,  wxSizerFlags());

    SetSizerAndFit(sizer);
}
