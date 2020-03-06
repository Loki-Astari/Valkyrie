#ifndef THORSANVIL_VALKYRIE_WALKER_GRAPH_FRAME_H
#define THORSANVIL_VALKYRIE_WALKER_GRAPH_FRAME_H

#include "ValkyrieWalkerConfig.h"
#include "DrawableDistanceGraph.h"
#include "DrawableSpeciesGraph.h"
#include <wx/wx.h>


namespace ThorsAnvil
{
    namespace ValkyrieWalker
    {

class Walker;
class FrameGraph: public wxFrame
{
    std::vector<Walker>&        walkers;
    DrawableDistanceGraph       distanceGraph;
    DrawableSpeciesGraph        speciesGraph;

    public:
        FrameGraph(std::vector<Walker>& walkers);
        void tick();

    private:

        DECLARE_EVENT_TABLE()
};

    }
}

#endif
