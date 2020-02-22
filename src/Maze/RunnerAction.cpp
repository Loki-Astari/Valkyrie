#include "RunnerAction.h"
#include "Maze.h"
#include "RunnerMaze.h"

using namespace ThorsAnvil::ValkyrieMaze;

RunnerAction::RunnerAction(Maze& maze, RunnerMaze& runner)
    : maze(maze)
    , runner(runner)
{}

void RunnerAction::draw(wxDC& dc) const
{
    runner.drawAnimation(dc, 0);
    maze.draw(dc);
}

wxSize RunnerAction::getSize() const
{
    return maze.getSize();
}

void RunnerAction::drawAnimation(wxDC& dc, int step) const
{
    return runner.drawAnimation(dc, step);
}

std::unique_ptr<wxDC> RunnerAction::animationDC()
{
    return runner.animationDC();
}

void RunnerAction::animateResetDo(wxDC& dc)
{
    runner.animateResetDo(dc);
}

void RunnerAction::animationStepDo(wxDC& dc, int step)
{
    runner.animationStepDo(dc, step);
}

int RunnerAction::animationMaxStep() const
{
    return runner.animationMaxStep();
}
