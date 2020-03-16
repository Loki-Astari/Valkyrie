#ifndef THORSANVIL_VALKYRIE_RUNNER_PANEL_BUTTON_BUILDER_H
#define THORSANVIL_VALKYRIE_RUNNER_PANEL_BUTTON_BUILDER_H

#include <wx/window.h>
#include <ThorsUI/UIFrameSimple.h>

namespace ThorsAnvil
{
    namespace ValkyrieRunner
    {

static constexpr int BUTTON_SAVE_ID       = wxID_HIGHEST + 1;
static constexpr int BUTTON_RUN1_ID       = wxID_HIGHEST + 2;
static constexpr int BUTTON_SORT_ID       = wxID_HIGHEST + 3;
static constexpr int BUTTON_UPDATE_ID     = wxID_HIGHEST + 4;
static constexpr int BUTTON_EVOLVE_ID     = wxID_HIGHEST + 5;
static constexpr int BUTTON_RUN1K_ID      = wxID_HIGHEST + 6;

class PanelButtonBuilder: public ThorsUI::SimplePanelBuilder
{
    public:
        virtual void addItems(wxWindow* panel, wxSizer* sizer) const override;
};

    }
}

#endif
