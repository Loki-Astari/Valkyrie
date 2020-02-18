#ifndef THORSANVIL_VALKYRIE_RUNNER_NEURAL_NET_H
#define THORSANVIL_VALKYRIE_RUNNER_NEURAL_NET_H

#include "RunnerMaze.h"
#include "MazeDetail.h"
#include <Valkyrie/NeuralNet.h>
#include <ThorsUtil/Util.h>
#include <iomanip>

namespace ThorsAnvil
{
    namespace ValkyrieMaze
    {

class RunnerNeuralNet: public RunnerMaze
{
    int             generationCount;
    Valkyrie::Net   neurialNet;
    int             directionToInfoMap(int dir);
    public:
        RunnerNeuralNet(Maze& maze);
        RunnerNeuralNet(Maze& maze, std::istream& stream);
        virtual ThorsUtil::Pos   getNextMove(MD::Info const& info, ThorsUtil::Pos const& currentPosition, MD::Direction const& currentDirection, ThorsUtil::Pos const& end) override;
        virtual int         generation()                const override;
        virtual void        print(std::ostream& stream) const override;
        virtual void        mutate()                          override;
        virtual void        mutate(Runner& runner)            override;
};

    }
}

#endif
