#include "UIFrameGraph.h"
#include "Walker.h"
#include "DrawableDistanceGraph.h"
#include "DrawableSpeciesGraph.h"
#include "DrawableDistanceHotMap.h"
#include "ThorsUI/UIPanelDrawable.h"

using namespace ThorsAnvil::ValkyrieWalker;

BEGIN_EVENT_TABLE(FrameGraph, wxFrame)
END_EVENT_TABLE()

FrameGraph::FrameGraph(wxWindow* parent, DrawableDistanceGraph& distanceGraph, DrawableSpeciesGraph& speciesGraph, DrawableDistanceHotMap& distanceHotMap)
    : wxFrame(parent, wxID_ANY , wxT("Valkyrie Graph"))
{
    wxPanel* panelDistanceGraph = new ThorsUI::PanelDrawable(this, distanceGraph);
    wxPanel* panelSpeciesGraph  = new ThorsUI::PanelDrawable(this, speciesGraph);
    wxPanel* panelDistanceMap   = new ThorsUI::PanelDrawable(this, distanceHotMap);

    wxSizer* sizer              = new wxBoxSizer(wxHORIZONTAL);
    sizer->Add(panelDistanceGraph, wxSizerFlags());
    sizer->AddSpacer(10);
    sizer->Add(panelSpeciesGraph,  wxSizerFlags());
    sizer->AddSpacer(10);
    sizer->Add(panelDistanceMap,   wxSizerFlags());

    SetSizerAndFit(sizer);
}
