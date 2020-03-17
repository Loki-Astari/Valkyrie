#include "UIPanelButtonBuilder.h"
#include "UIValkyrieRunnerApp.h"

using namespace ThorsAnvil::ValkyrieRunner;

void UIPanelButtonBuilder::addItems(wxWindow* panel, wxSizer* sizer) const
{
    wxButton* buttonSave = new wxButton(panel, UIValkyrieRunnerApp::BUTTON_SAVE_ID, wxT("Save"));
    wxButton* buttonRun  = new wxButton(panel, UIValkyrieRunnerApp::BUTTON_RUN1_ID, wxT("Run 1"));
    wxButton* buttonSort = new wxButton(panel, UIValkyrieRunnerApp::BUTTON_SORT_ID, wxT("Sort"));
    wxButton* buttonUpd  = new wxButton(panel, UIValkyrieRunnerApp::BUTTON_UPDATE_ID, wxT("Update"));
    wxButton* buttonEvlv = new wxButton(panel, UIValkyrieRunnerApp::BUTTON_EVOLVE_ID, wxT("Evolve"));
    wxButton* buttonR1K  = new wxButton(panel, UIValkyrieRunnerApp::BUTTON_RUN1K_ID,  wxT("Run 1K"));

    sizer->Add(buttonSave, wxSizerFlags());
    sizer->Add(buttonRun,  wxSizerFlags());
    sizer->Add(buttonSort, wxSizerFlags());
    sizer->Add(buttonUpd,  wxSizerFlags());
    sizer->Add(buttonEvlv, wxSizerFlags());
    sizer->AddSpacer(20);
    sizer->Add(buttonR1K,  wxSizerFlags());
}
