#include "UIPanelWalkerCrowd.h"
#include "ThorsUI/UIPanelDrawable.h"

using namespace ThorsAnvil::ValkyrieWalker;

PanelWalkerCrowd::PanelWalkerCrowd(wxWindow* parent, std::vector<Walker>& walkers)
    : PanelSpriteRunner(parent)
    , walkers(walkers)
    , panelSizer(nullptr)
    , background(true)
{
    buttons.reserve(walkers.size());

    wxSizer* walkerSizer = new wxGridSizer(40, 5, 5);

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

        addSprite(move + 1, [startP, delta, window](int step)
        {
            wxPoint pos {startP.first, startP.second};
            pos.x += step * delta.first;
            pos.y += step * delta.second;
            window->Move(pos);
        });
    }
}

enum EvolveState {Live, Die, Mutate};
void PanelWalkerCrowd::evolve()
{
    std::default_random_engine&     generator   = ThorsUtil::Random::getRandomGenerator();
    std::uniform_real_distribution  random;

    wxSizer*    panelSizer  = GetSizer();
    std::size_t maxCount    = panelSizer->GetItemCount();

    int evolveParent = 0;
    for (std::size_t loop = 0;loop < buttons.size(); ++loop)
    {
        EvolveState     state = Live;

        // We want to kill approx 50% of the walkers.
        // The better scoring Walkers will have a higher chance of surviving.
        // The worse scoring Walkers will have a lower chance of surviving.
        if (random(generator) < (loop * 1.0 / maxCount) || buttons[loop].wasKilled())
        {
            state = Die;
        }
        // Even if we don't die there is an additional 25% chance of
        // of a random mutation.
        else if (loop != 0 && random(generator) < 0.25)
        {
            state = Mutate;
        }

        switch (state)
        {
            case Live:
                addSprite(buttons.size() * 4 + 12, [&button = buttons[loop], flashBeg = 4 * loop](int step)
                {
                    if (static_cast<std::size_t>(step) == flashBeg)
                    {
                        button.flashBorder(3, *wxBLACK_PEN);
                    }
                    button.refresh();
                });
                break;
            case Die:
                addSprite(buttons.size() * 4 + 12, [&button = buttons[loop], &source = buttons[evolveParent++], flashBeg = 4 * loop](int step)
                {
                    if (static_cast<std::size_t>(step) == flashBeg)
                    {
                        button.flashBackground(8, *wxYELLOW_BRUSH);
                        button.kill();
                    }
                    if (static_cast<std::size_t>(step) == (flashBeg + 8))
                    {
                        button.flashBackground(3, *wxCYAN_BRUSH);
                        source.flashBorder(3, *wxCYAN_PEN);
                        button.spawn(source);
                    }
                    button.refresh();
                });
                break;
            case Mutate:
                addSprite(buttons.size() * 4 + 12, [&button = buttons[loop], flashBeg = 4 * loop](int step)
                {
                    if (static_cast<std::size_t>(step) == flashBeg)
                    {
                        button.flashBackground(3, *wxRED_BRUSH);
                        button.mutate();
                    }
                    button.refresh();
                });
                break;
        }
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
    , flashBorderTime(0)
    , flashBorderPen(*wxBLACK_PEN)
    , flashBackgroundTime(0)
    , flashBackgroundBrush(*wxWHITE_BRUSH)
{}

void PanelWalkerCrowd::WalkerButton::draw(wxDC& dc) const
{
    wxSize size = getSize();
    wxBrush defaultBrush = dc.GetBrush();
    wxPen   defaultPen   = dc.GetPen();

    dc.SetLogicalScale(scale, scale);
    if (parent.background)
    {
        if (flashBackgroundTime > 0)
        {
            --flashBackgroundTime;
            dc.SetBrush(flashBackgroundBrush);
        }
        dc.DrawRectangle(0, 0, 1000, 1000);
    }
    if (flashBorderTime > 0)
    {
        --flashBorderTime;

        dc.SetBrush(wxNullBrush);

        wxPen        widePen(flashBorderPen);
        widePen.SetWidth(4 / scale);
        dc.SetPen(widePen);

        dc.DrawRectangle(0, 0, 1000, 1000);
    }
    dc.SetBrush(defaultBrush);
    dc.SetPen(defaultPen);

    dc.SetLogicalOrigin(-size.x / scale / 2, 0);

    if (scale < 1.0/10)
    {
        wxPen        widePen(defaultPen);
        widePen.SetWidth(widePen.GetWidth() + 1);
        dc.SetPen(widePen);
    }

    walker.draw(dc);
}

wxSize PanelWalkerCrowd::WalkerButton::getSize() const
{
    wxSize  result  = walker.getSize();
    result.x *= scale;
    result.y *= scale;
    return result;
}

void PanelWalkerCrowd::WalkerButton::flashBorder(int time, wxPen const& pen)
{
    flashBorderTime         = time;
    flashBorderPen          = pen;
}

void PanelWalkerCrowd::WalkerButton::flashBackground(int time, wxBrush const& brush)
{
    flashBackgroundTime     = time;
    flashBackgroundBrush    = brush;
}
