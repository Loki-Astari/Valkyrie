#ifndef THORSANVIL_VALKYRIE_WALKER_FRAME_WALKER_H
#define THORSANVIL_VALKYRIE_WALKER_FRAME_WALKER_H

#include "ValkyrieWalkerConfig.h"
#include <wx/wx.h>


namespace ThorsAnvil
{
    namespace ValkyrieWalker
    {

class Walker;
class FrameWalker: public wxFrame
{
    public:
        FrameWalker(wxWindow* parent, Walker& walker);
};

    }
}

#endif
