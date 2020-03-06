#ifndef THORSANVIL_VALKYRIE_WALKER_GRAPH_FRAME_H
#define THORSANVIL_VALKYRIE_WALKER_GRAPH_FRAME_H

#include "ValkyrieWalkerConfig.h"
#include <wx/wx.h>


namespace ThorsAnvil
{
    namespace ValkyrieWalker
    {

class Walker;
class FrameGraph: public wxFrame
{
    std::vector<Walker>&        walkers;

    public:
        FrameGraph(std::vector<Walker>& walkers);

    private:

        DECLARE_EVENT_TABLE()
};

    }
}

#endif
