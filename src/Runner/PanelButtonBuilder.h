#ifndef THORSANVIL_VALKYRIE_RUNNER_PANEL_BUTTON_BUILDER_H
#define THORSANVIL_VALKYRIE_RUNNER_PANEL_BUTTON_BUILDER_H

#include <wx/window.h>
#include <ThorsUI/UIFrameSimple.h>

namespace ThorsAnvil
{
    namespace ValkyrieRunner
    {

class PanelButtonBuilder: public ThorsUI::SimplePanelBuilder
{
    public:
        virtual void addItems(wxWindow* panel, wxSizer* sizer) const override;
};

    }
}

#endif
