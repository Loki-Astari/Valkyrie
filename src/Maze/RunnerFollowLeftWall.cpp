#include "RunnerFollowLeftWall.h"

using namespace ThorsAnvil::ValkyrieMaze;

RunnerFollowLeftWall::RunnerFollowLeftWall(Maze& maze)
    : RunnerMaze(maze)
{}

ThorsAnvil::ThorsUtil::Pos RunnerFollowLeftWall::getNextMove(MD::Info const& info, ThorsUtil::Pos const& currentPosition, MD::Direction const& currentDirection, ThorsUtil::Pos const& /*end*/)
{
    switch (currentDirection)
    {
        case MD::North: return tryMove(currentPosition, info, {MD::West, MD::North, MD::East, MD::South});
        case MD::South: return tryMove(currentPosition, info, {MD::East, MD::South, MD::West, MD::North});
        case MD::East:  return tryMove(currentPosition, info, {MD::North, MD::East, MD::South, MD::West});
        case MD::West:  return tryMove(currentPosition, info, {MD::South, MD::West, MD::North, MD::East});
    }
    std::cerr << "throw: RunnerFollowLeftWall::getNextMove\n";
    throw int(3);
}

int RunnerFollowLeftWall::directionToInfoMap(MD::Direction dir)
{
    switch (dir)
    {
        case MD::North: return 2;
        case MD::South: return 7;
        case MD::East:  return 5;
        case MD::West:  return 4;
    }
    std::cerr << "throw: RunnerFollowLeftWall::directionToInfoMap\n";
    throw int(2);
}

ThorsAnvil::ThorsUtil::Pos RunnerFollowLeftWall::tryMove(ThorsUtil::Pos currentPos, MD::Info const& info, std::initializer_list<MD::Direction> const& order)
{
    for (auto nextDirection: order)
    {
        int infoIndex = directionToInfoMap(nextDirection);
        if (info[infoIndex] != 0)
        {
            //currentDirection = nextDirection;
            switch (nextDirection)
            {
                case MD::North: --currentPos.second;break;
                case MD::South: ++currentPos.second;break;
                case MD::East:  ++currentPos.first;break;
                case MD::West:  --currentPos.first;break;
            }
            return currentPos;
        }
    }
    std::cerr << "throw: RunnerFollowLeftWall::tryMove\n";
    throw int(1);
}
