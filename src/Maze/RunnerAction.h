#ifndef THORSANVIL_VALKYRIE_RUNNER_ACTION_H
#define THORSANVIL_VALKYRIE_RUNNER_ACTION_H

#include <ThorsUI/Animateable.h>

namespace ThorsAnvil
{
    namespace ValkyrieMaze
    {

class Maze;
class RunnerMaze;

class RunnerAction: public ThorsUI::Animateable
{
    Maze&           maze;
    RunnerMaze&     runner;
    public:
        RunnerAction(Maze& maze, RunnerMaze& runner);
        // Drawable
        virtual void    draw(wxDC& dc)  const override;
        virtual wxSize  getSize()       const override;

        // Animateable
        virtual void    drawAnimation(wxDC& dc) const           override;
        virtual std::unique_ptr<wxDC>   animationDC()           override;
        virtual void    animateResetDo(wxDC& dc)                override;
        virtual void    animationStepDo(wxDC& dc, int step)     override;
        virtual int     animationMaxStep() const                override;

        void print()
        {
            std::cerr << "M: " << &maze << "R: " << &runner << "\n";
        }
};

    }
}

#endif
