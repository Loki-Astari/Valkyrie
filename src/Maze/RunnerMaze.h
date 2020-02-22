#ifndef THORSANVIL_VALKYRIE_RUNNER_MAZE_H
#define THORSANVIL_VALKYRIE_RUNNER_MAZE_H

#include "Maze.h"
#include "MazeDetail.h"
#include <Valkyrie/Runner.h>
#include <ThorsUtil/Util.h>
#include <ThorsUI/Animateable.h>
#include <map>
#include <vector>
#include <functional>

namespace ThorsAnvil
{
    namespace ValkyrieMaze
    {

namespace MD = ThorsAnvil::ValkyrieMaze::MazeDetail;

class RunnerMaze: public Valkyrie::Runner
{
    std::reference_wrapper<Maze>    maze;
    std::vector<ThorsUtil::Pos>     history;
    std::map<ThorsUtil::Pos, int>   covered;
    MD::Direction                   currentDirection;
    int                             runningScore;

    mutable wxBitmap                bitmap;
    mutable std::vector<wxPoint>    oldPoints;

    public:
        RunnerMaze(Maze& maze);

        virtual void reproduce(Valkyrie::Runner const& parent) override;
        virtual void run() override;
        int getCoverCount(ThorsUtil::Pos p) const;

        virtual ThorsUtil::Pos   getNextMove(MD::Info const& info, ThorsUtil::Pos const& currentPosition, MD::Direction const& currentDirection, ThorsUtil::Pos const& end) = 0;

        // Drawable
        virtual void    draw(wxDC& dc)      const override;
        virtual wxSize  getSize()           const override;

        // Animatable
        virtual void    drawAnimation(wxDC& dc, int step) const   override;
        virtual std::unique_ptr<wxDC>   animationDC()       override;
        virtual void    animateResetDo(wxDC& dc)            override;
        virtual void    animationStepDo(wxDC& dc, int step) override;
        virtual int     animationMaxStep()          const   override;

        virtual int     score() const                       override;
        virtual void    mutate()                            override {}
        virtual void    mutate(Valkyrie::Runner&)           override {mutate();}

                int     calcScore(bool wall, ThorsUtil::Pos const& cp, ThorsUtil::Pos const& end, int extra = 0) const;
        virtual int     generation()        const {return 0;}

};

    }
}

#endif
