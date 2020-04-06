#include "UIPanelRunnerCrowd.h"
#include "UIPanelDrawingMiniRunner.h"

using namespace ThorsAnvil::ValkyrieRunner;

UIPanelRunnerCrowd::UIPanelRunnerCrowd(std::vector<Runner>& runners)
    : runners(runners)
{}

void UIPanelRunnerCrowd::addItems(wxWindow* panel, wxSizer* sizer) const
{
    for (auto& runner: runners)
    {
        std::cout << runner.species() << "\n";
        wxPanel* runner  = new UIPanelDrawingMiniRunner(panel, runner);

        sizer->Add(runner, wxSizerFlags());
    }
}
