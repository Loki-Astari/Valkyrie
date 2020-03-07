#ifndef THORSANVIL_VALKYRIE_WALKER_GRAPH_FRAME_H
#define THORSANVIL_VALKYRIE_WALKER_GRAPH_FRAME_H

#include "ValkyrieWalkerConfig.h"
#include <wx/wx.h>


namespace ThorsAnvil
{
    namespace ValkyrieWalker
    {

class DrawableDistanceGraph;
class DrawableSpeciesGraph;
class DrawableDistanceHotMap;
class FrameGraph: public wxFrame
{
    public:
        FrameGraph(DrawableDistanceGraph& distanceGraph, DrawableSpeciesGraph& speciesGraph, DrawableDistanceHotMap& distanceHotMap);

    private:

        DECLARE_EVENT_TABLE()
};

    }
}

#endif
