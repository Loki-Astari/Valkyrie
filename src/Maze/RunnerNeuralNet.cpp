#include "RunnerNeuralNet.h"
#include <ThorsUtil/Random.h>

using namespace ThorsAnvil::ValkyrieMaze;

RunnerNeuralNet::RunnerNeuralNet(Maze& maze)
    : RunnerMaze(maze)
    , generationCount(0)
    , neurialNet(5, 4)
{}

RunnerNeuralNet::RunnerNeuralNet(Maze& maze, std::istream& stream)
    : RunnerMaze(maze)
    , generationCount(-1)
    , neurialNet(0, 0)
{
    stream >> generationCount >> neurialNet;
}
void RunnerNeuralNet::mutate()
{
    ++generationCount;
    neurialNet.mutate();
}

void RunnerNeuralNet::mutate(Runner& runner)
{
    RunnerNeuralNet&  origNet = dynamic_cast<RunnerNeuralNet&>(runner);
    (*this) = origNet;
    mutate();
}


ThorsAnvil::ThorsUtil::Pos RunnerNeuralNet::getNextMove(MD::Info const& info, ThorsUtil::Pos const& currentPosition, MD::Direction const& currentDirection, ThorsUtil::Pos const& end)
{

    std::vector<float>  input(5, 0.0);
    input[0]    = currentDirection;
    input[1]    = calcScore(info[2] == 0, ThorsUtil::Pos{currentPosition.first, currentPosition.second - 1}, end, 1);
    input[2]    = calcScore(info[4] == 0, ThorsUtil::Pos{currentPosition.first - 1, currentPosition.second}, end, 1);
    input[3]    = calcScore(info[5] == 0, ThorsUtil::Pos{currentPosition.first + 1, currentPosition.second}, end, 1);
    input[4]    = calcScore(info[7] == 0, ThorsUtil::Pos{currentPosition.first, currentPosition.second + 1}, end, 1);

    std::vector<float>  output  = neurialNet.step(input);

    ThorsUtil::Pos currentPos = currentPosition;

    auto best   = std::max_element(std::begin(output), std::end(output));
    auto result = std::distance(std::begin(output), best);
    switch (result)
    {
        case 0:     --currentPos.second;break;  // North
        case 1:     ++currentPos.second;break;  // South
        case 2:     ++currentPos.first;break;   // East
        case 3:     --currentPos.first;break;   // West
        default:
            std::cerr << "throw: RunnerNeuralNet::getNextMove\n";
            throw int(22);
    }
    return currentPos;
}

int RunnerNeuralNet::directionToInfoMap(int dir)
{
    switch (dir)
    {
        case 0: return 2;
        case 1: return 7;
        case 2: return 5;
        case 3: return 4;
    }
    std::cerr << "throw: RunnerNeuralNet::directionToInfoMap\n";
    throw int(2);
}

int RunnerNeuralNet::generation() const
{
    return generationCount;
}

void RunnerNeuralNet::print(std::ostream& stream) const
{
    stream << "RunnerNeuralNet " << generationCount << " " << neurialNet << "\n";
}
