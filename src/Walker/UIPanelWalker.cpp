#include "UIPanelWalker.h"
#include "Walker.h"

using namespace ThorsAnvil::ValkyrieWalker;

PanelWalker::PanelWalker(wxWindow* parent, Walker& walker)
    : PanelAnimateableRunner(parent, walker, 40)
    , walker(walker)
{
    (void)this->walker;
}
