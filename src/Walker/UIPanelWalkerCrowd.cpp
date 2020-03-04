#include "UIPanelWalkerCrowd.h"
#include "Walker.h"
#include "ThorsUI/UIPanelDrawable.h"

using namespace ThorsAnvil::ValkyrieWalker;

PanelWalkerCrowd::PanelWalkerCrowd(wxWindow* parent, std::vector<Walker>& walkers)
    : PanelSpriteRunner(parent)
    , walkers(walkers)
    , panelSizer(nullptr)
    , background(true)
{
    buttons.reserve(walkers.size());

    wxSizer* walkerSizer = new wxGridSizer(20, 5, 5);

    for (auto& walker: walkers)
    {
        buttons.emplace_back(*this, walker, 1.0 / 20);
        wxPanel* walkerPanel = new ThorsUI::PanelDrawable(this, buttons.back());
        walkerSizer->Add(walkerPanel, 1, 0, 0, nullptr);
    }
    SetSizerAndFit(walkerSizer);
}

void PanelWalkerCrowd::shuffle()
{
    if (panelSizer)
    {
        return;
    }
    background  = false;
    panelSizer  = GetSizer();

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

        addSprite([startP, delta, window](int step)
        {
            wxPoint pos {startP.first, startP.second};
            pos.x += step * delta.first;
            pos.y += step * delta.second;
            window->Move(pos);
        }, move + 1);
    }
}

void PanelWalkerCrowd::evolve()
{
    wxSizer*    panelSizer  = GetSizer();
    std::size_t maxCount    = panelSizer->GetItemCount();

    for (std::size_t loop = 0;loop < buttons.size(); ++loop)
    {
        addSprite([&button = buttons[loop], flashBeg = 4 * loop, flashEnd = 4 * loop + 4, maxCount, loop](int step)
        {
            std::default_random_engine&     generator   = ThorsUtil::Random::getRandomGenerator();
            std::uniform_real_distribution  random;

            if (static_cast<std::size_t>(step) == flashBeg)
            {
                button.flash(true);
                // We want to kill approx 60% of the walkers.
                // The better scoring Walkers will have a higher chance of surviving.
                // The worse scoring Walkers will have a lower chance of surviving.
                if (random(generator) < (loop / 0.75 * maxCount))
                {
                    button.kill();
                }
                // Even if we don't die there is an additional 25% chance of
                // of a random mutation.
                else if (random(generator) < 0.25)
                {
                }
                button.refresh();
            }
            if (static_cast<std::size_t>(step) == flashEnd)
            {
                button.flash(false);
                button.refresh();
            }
        }, buttons.size() * 4 + 8);
    }
}

wxSize PanelWalkerCrowd::getSize() const
{
    wxSizer*        sizer = GetSizer();
    return sizer->GetMinSize();
}

void PanelWalkerCrowd::animateResetActionDone(wxDC& /*dc*/)
{
    if (panelSizer)
    {
        background  = true;
        std::stable_sort(std::begin(walkers), std::end(walkers), [](Walker const& lhs, Walker const& rhs){return lhs.score() > rhs.score();});
        panelSizer->Layout();
        panelSizer = nullptr;
        Refresh();
    }
}

PanelWalkerCrowd::WalkerButton::WalkerButton(PanelWalkerCrowd& parent, Walker& w, float scale)
    : parent(parent)
    , walker(w)
    , scale(scale)
    , flashBackground(false)
{}

void PanelWalkerCrowd::WalkerButton::kill()
{
    walker.kill();
}

void PanelWalkerCrowd::WalkerButton::mutate()
{
    walker.mutate();
}

void PanelWalkerCrowd::WalkerButton::spawn(WalkerButton const& parent)
{
    walker.spawn(parent.walker);
}

void PanelWalkerCrowd::WalkerButton::refresh()
{
    parent.Refresh();
}

void PanelWalkerCrowd::WalkerButton::draw(wxDC& dc) const
{
    wxSize size = getSize();
    dc.SetLogicalScale(scale, scale);
    if (parent.background)
    {
        if (flashBackground)
        {
            dc.SetBrush(*wxYELLOW_BRUSH);
        }
        dc.DrawRectangle(0, 0, 1000, 1000);
    }
    dc.SetLogicalOrigin(-size.x / scale / 2, 0);

    const wxPen& currentPen = dc.GetPen();
    wxPen        widePen(currentPen);

    widePen.SetWidth(widePen.GetWidth() + 1);
    dc.SetPen(widePen);

    walker.draw(dc);
}

wxSize PanelWalkerCrowd::WalkerButton::getSize() const
{
    wxSize  result  = walker.getSize();
    result.x *= scale;
    result.y *= scale;
    return result;
}

void PanelWalkerCrowd::WalkerButton::flash(bool b)
{
    flashBackground = b;
}
