#ifndef THORSANVIL_VALKYRIE_RUNNER_FOLLOW_LEFT_WALL_H
#define THORSANVIL_VALKYRIE_RUNNER_FOLLOW_LEFT_WALL_H

#include "RunnerMaze.h"
#include "MazeDetail.h"
#include <ThorsUtil/Util.h>

namespace ThorsAnvil
{
    namespace ValkyrieMaze
    {

class RunnerFollowLeftWall: public RunnerMaze
{
        ThorsUtil::Pos tryMove(ThorsUtil::Pos currentPos, MD::Info const& info, std::initializer_list<MD::Direction> const& order);
        int       directionToInfoMap(MD::Direction dir);

    public:
        RunnerFollowLeftWall(Maze& maze);
        virtual ThorsUtil::Pos   getNextMove(MD::Info const& info, ThorsUtil::Pos const& currentPosition, MD::Direction const& currentDirection, ThorsUtil::Pos const& end) override;
};

    }
}

#endif
