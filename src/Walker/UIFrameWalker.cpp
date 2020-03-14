#include "UIFrameWalker.h"
#include "UIPanelWalker.h"

using namespace ThorsAnvil::ValkyrieWalker;

FrameWalker::FrameWalker(wxWindow* parent, Walker& walker)
    : wxFrame(parent, wxID_ANY , wxT("Walker Runing"))
{
    new PanelWalker(this, walker);
    Show();
}
