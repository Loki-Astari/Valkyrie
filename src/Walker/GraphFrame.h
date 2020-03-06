#ifndef THORSANVIL_GRAPH_FRAME_H
#define THORSANVIL_GRAPH_FRAME_H

#include "ValkyrieWalkerConfig.h"
#include <wx/wx.h>


namespace ThorsAnvil
{
    namespace ValkyrieWalker
    {

class Walker;
class GraphFrame: public wxFrame
{
    std::vector<Walker>&        walkers;

    public:
        GraphFrame(std::vector<Walker>& walkers);

    private:

        DECLARE_EVENT_TABLE()
};

    }
}

#endif
