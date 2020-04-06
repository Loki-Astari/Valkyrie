#ifndef THORSANVIL_VALKYRIE_RUNNER_UI_PANEL_DRAWING_MINI_RUNNER_H
#define THORSANVIL_VALKYRIE_RUNNER_UI_PANEL_DRAWING_MINI_RUNNER_H

#include "Runner.h"
#include <wx/window.h>
#include <ThorsUI/UIPanelDrawableClickAction.h>

namespace ThorsAnvil
{
    namespace ValkyrieRunner
    {

class UIPanelDrawingMiniRunner: public ThorsUI::PanelDrawableClickAction
{
    Runner&        runner;
    public:
        UIPanelDrawingMiniRunner(wxWindow* parent, Runner& runner);
};

    }
}

#endif
