#ifndef THORSANVIL_VALKYRIE_WALKER_UI_PANEL_WALKER_H
#define THORSANVIL_VALKYRIE_WALKER_UI_PANEL_WALKER_H

#include "ThorsUI/UIPanelAnimateableRunner.h"

namespace ThorsAnvil
{
    namespace ValkyrieWalker
    {

class Walker;
class PanelWalker: public ThorsUI::PanelAnimateableRunner
{
    Walker&     walker;

    public:
        PanelWalker(wxWindow* parent, Walker& walker);
};


    }
}

#endif
