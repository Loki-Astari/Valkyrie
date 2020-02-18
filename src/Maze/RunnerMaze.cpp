#include "RunnerMaze.h"

using namespace ThorsAnvil::ValkyrieMaze;

RunnerMaze::RunnerMaze(Maze& maze)
    : maze(maze)
    , currentDirection(MD::North)
    , bitmap(832, 832)
{}

void RunnerMaze::reproduce(Runner const& rhs)
{
    RunnerMaze const& parent = dynamic_cast<RunnerMaze const&>(rhs);

    history.clear();
    covered.clear();
    currentDirection    = parent.currentDirection;
    runningScore        = parent.runningScore;;

    oldPoints.clear();
}

MD::Direction operator-(ThorsAnvil::ThorsUtil::Pos const& current, ThorsAnvil::ThorsUtil::Pos const& next)
{
    int xDiff = current.first  - next.first;
    int yDiff = current.second - next.second;

    if (xDiff == 0 && yDiff > 0)    {return MD::North;}
    if (xDiff == 0 && yDiff < 0)    {return MD::South;}
    if (yDiff == 0 && xDiff < 0)    {return MD::East;}
    if (yDiff == 0 && xDiff > 0)    {return MD::West;}

    std::cerr << "throw: MD::Direction operator-()\n";
    throw int(12);
}

int RunnerMaze::getCoverCount(ThorsUtil::Pos p) const
{
    auto f = covered.find(p);
    return (f == covered.end()) ? 0 : f->second;
}

int RunnerMaze::calcScore(bool wall, ThorsUtil::Pos const& cp, ThorsUtil::Pos const& end, int extra) const
{
    if (wall)
    {
        return -750;
    }
    if (cp == end)
    {
        return 1000;
    }

    int used = getCoverCount(cp) + extra;
    int wallCount = maze.get().check(cp, MD::North) + maze.get().check(cp, MD::South) + maze.get().check(cp, MD::East) + maze.get().check(cp, MD::West);

    if (used <= (4 - wallCount))
    {
        return 100 / (used * used * used);
    }
    return -25;
}

void RunnerMaze::run()
{
    ThorsUtil::Pos pos = maze.get().start();
    currentDirection = MD::North;

    history.clear();
    history.emplace_back(pos);

    covered.clear();
    covered[pos]++;

    runningScore   = 0;
    int wallHits   = 0;

    ThorsUtil::Pos  currentPos   = history[history.size() - 1];
    for (int loop = 0;wallHits < 5 && runningScore > -10000; ++loop)
    {
        MD::Info   currentInfo  = maze.get().info(history.back());
        ThorsUtil::Pos  nextPos      = this->getNextMove(currentInfo, currentPos, currentDirection, maze.get().end());

        MD::Direction newDirection = currentPos - nextPos;

        if (maze.get().check(currentPos, newDirection))
        {
            ++wallHits;
            switch (currentDirection)
            {
                case MD::North: currentDirection = MD::West;break;
                case MD::East:  currentDirection = MD::North;break;
                case MD::South: currentDirection = MD::East;break;
                case MD::West:  currentDirection = MD::South;break;
            }
            runningScore    -= 750;
            continue;
        }

        currentDirection = newDirection;
        currentPos = nextPos;
        history.emplace_back(currentPos);
        covered[currentPos]++;
        runningScore += calcScore(false, currentPos, maze.get().end());

        if (maze.get().atExit(currentPos))
        {
            runningScore += 1000;
            break;
        }
    }
}

void RunnerMaze::draw(wxDC& dc) const
{
    dc.SetPen(*wxGREEN_PEN);
    wxPoint tl{0, 0};
    wxPoint br{20, 20};
    dc.DrawLine(tl, br);
    wxPoint bl{0, 20};
    wxPoint tr{20, 0};
    dc.DrawLine(bl, tr);
}

wxSize RunnerMaze::getSize() const
{
    return wxSize(20, 20);
}

void RunnerMaze::drawAnimation(wxDC& dc) const
{
    dc.DrawBitmap(bitmap, 0, 0);
}

std::unique_ptr<wxDC> RunnerMaze::animationDC()
{
    return std::unique_ptr<wxDC>(new wxMemoryDC(bitmap));
}

void RunnerMaze::animateResetDo(wxDC& dc)
{
    dc.SetBackground(*wxWHITE_BRUSH);
    dc.Clear();
}

int RunnerMaze::animationMaxStep() const
{
    return history.size();
}

void RunnerMaze::animationStepDo(wxDC& dc, int step)
{
    if (step == 0)
    {
        oldPoints.clear();
    }
    else
    {
        ThorsUtil::Pos   next = history[step - 1];
        wxPoint     nextPoint{next.first * 8 + 20, next.second * 8 + 20};
        if (oldPoints.size() >= 2 && oldPoints[oldPoints.size() - 2] == nextPoint)
        {
            dc.SetPen(wxPen{*wxBLUE, 5});
            dc.DrawLine(oldPoints[oldPoints.size() - 1], oldPoints[oldPoints.size() - 2]);
            oldPoints.pop_back();
        }
        else
        {
            oldPoints.push_back(nextPoint);
            dc.SetPen(wxPen{*wxRED, 5});
            if (oldPoints.size() >= 3)
            {
                dc.DrawLines(3, oldPoints.data() + oldPoints.size() - 3);
            }
            else if (oldPoints.size() >= 2)
            {
                dc.DrawLines(2, oldPoints.data() + oldPoints.size() - 2);
            }
        }
    }
}

int RunnerMaze::score() const
{
    return runningScore;
}
