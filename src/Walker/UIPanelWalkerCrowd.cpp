#include "UIPanelWalkerCrowd.h"
#include "Walker.h"
#include "ThorsUI/UIPanelDrawable.h"

using namespace ThorsAnvil::ValkyrieWalker;

PanelWalkerCrowd::PanelWalkerCrowd(wxWindow* parent, std::vector<Walker>& walkers)
    : PanelSpriteRunner(parent)
    , walkers(walkers)
    , panelSizer(nullptr)
{
    buttons.reserve(walkers.size());

    wxSizer* walkerSizer = new wxGridSizer(25, 5, 5);

    for (auto& walker: walkers)
    {
        buttons.emplace_back(walker, 1.0/10);
        wxPanel* walkerPanel = new ThorsUI::PanelDrawable(this, buttons.back());
        walkerSizer->Add(walkerPanel, 1, 0, 0, nullptr);
    }
    SetSizer(walkerSizer);
}

void PanelWalkerCrowd::shuffle()
{
    if (panelSizer)
    {
        return;
    }
    panelSizer = GetSizer();

    std::vector<int>    newOrder;
    for (std::size_t loop = 0; loop < walkers.size(); ++ loop)
    {
        newOrder.emplace_back(loop);
    }
    std::stable_sort(std::begin(newOrder), std::end(newOrder), [&walkers = this->walkers](int lhs, int rhs){return walkers[lhs].score() > walkers[rhs].score();});

    std::vector<wxWindow*>  windows;

    for (std::size_t loop = 0;loop < panelSizer->GetItemCount(); ++loop)
    {
        wxSizerItem*    sizerItem   = panelSizer->GetItem(loop);
        wxWindow*       sizerWindow = sizerItem->GetWindow();

        windows.emplace_back(sizerWindow);
    }
    for (std::size_t loop = 0;loop < windows.size(); ++loop)
    {
        wxWindow*           window  = windows[newOrder[loop]];
        wxPoint             start   = windows[newOrder[loop]]->GetPosition();
        wxPoint             end     = windows[loop]->GetPosition();
        int                 dis     = std::sqrt((start.x - end.x) * (start.x - end.x) + (start.y - end.y) * (start.y - end.y));
        int                 move    = dis / 15;

        ThorsUtil::Pos      startP{start.x, start.y};
        ThorsUtil::Delta    delta{(end.x - start.x) * 1.0 / move, (end.y - start.y) * 1.0 / move};

        addSprite([window](ThorsUtil::Pos const& start, ThorsUtil::Delta const& delta, int step)
        {
            wxPoint pos {start.first, start.second};
            pos.x += step * delta.first;
            pos.y += step * delta.second;
            window->Move(pos);
        }, startP, delta, move + 1);
    }
}

void PanelWalkerCrowd::evolve()
{
}

wxSize PanelWalkerCrowd::getSize() const
{
    return wxSize{2000, 2000};
}

void PanelWalkerCrowd::animateResetActionDone(wxDC& /*dc*/)
{
    std::stable_sort(std::begin(walkers), std::end(walkers), [](Walker const& lhs, Walker const& rhs){return lhs.score() > rhs.score();});
    panelSizer->Layout();
    panelSizer = nullptr;
    Refresh();
}

PanelWalkerCrowd::WalkerButton::WalkerButton(Walker& w, float scale)
    : walker(w)
    , scale(scale)
{}

void PanelWalkerCrowd::WalkerButton::draw(wxDC& dc) const
{
    wxSize size = getSize();
    dc.SetLogicalOrigin(size.x / 2, 0);
    dc.SetLogicalScale(scale, scale);
    walker.draw(dc);
}

wxSize PanelWalkerCrowd::WalkerButton::getSize() const
{
    wxSize  result  = walker.getSize();
    result.x *= scale;
    result.y *= scale;
    return result;
}
