#include "UIPanelRunnerCrowd.h"

using namespace ThorsAnvil::ValkyrieRunner;

UIPanelRunnerCrowd::UIPanelRunnerCrowd(std::vector<Runner>& runners)
    : runners(runners)
{}

void UIPanelRunnerCrowd::addItems(wxWindow* /*panel*/, wxSizer* /*sizer*/) const
{
    for (auto& runner: runners)
    {
        std::cout << runner.species() << "\n";
    }

    //wxButton* buttonR1K  = new wxButton(panel, UIAppValkyrieRunner::BUTTON_RUN1K_ID,  wxT("Run 1K"));

    //sizer->Add(buttonSave, wxSizerFlags());
}
