#ifndef THORSANVIL_VALKYRIE_RUNNER_UI_PANEL_RUNNER_CROWD_H
#define THORSANVIL_VALKYRIE_RUNNER_UI_PANEL_RUNNER_CROWD_H

#include "Runner.h"
#include <wx/window.h>
#include <ThorsUI/UIFrameSimple.h>

namespace ThorsAnvil
{
    namespace ValkyrieRunner
    {

class UIPanelRunnerCrowd: public ThorsUI::SimplePanelBuilder
{
    std::vector<Runner>&        runners;
    public:
        UIPanelRunnerCrowd(std::vector<Runner>& runners);
        virtual void addItems(wxWindow* panel, wxSizer* sizer) const override;
};

    }
}

#endif
