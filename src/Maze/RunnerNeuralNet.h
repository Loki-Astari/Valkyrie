#ifndef THORSANVIL_VALKYRIE_RUNNER_NEURAL_NET_H
#define THORSANVIL_VALKYRIE_RUNNER_NEURAL_NET_H

#include "RunnerMaze.h"
#include "MazeDetail.h"
#include <ThorsUtil/Util.h>
#include <iomanip>

namespace ThorsAnvil
{
    namespace ValkyrieMaze
    {
        namespace AI
        {

class Net
{
    using Axions    = std::vector<float>;
    using Neurons   = std::vector<Axions>;
    using Layers    = std::vector<Neurons>;

    Layers      axionLayers;
    public:
        Net(int inputSize, int outputSize);
        std::vector<float> step(std::vector<float>& input);
        void mutate();

        friend std::ostream& operator<<(std::ostream& stream, Net const& data)
        {
            stream << data.axionLayers.size() << " ";
            for (auto const& layer: data.axionLayers)
            {
                stream << layer.size() << " ";
                for (auto const& neuron: layer)
                {
                    stream << neuron.size() << " ";
                    for (auto const& axion: neuron)
                    {
                        stream << std::hexfloat << std::setprecision(12) << axion << " ";
                    }
                }
            }
            return stream;
        }
        friend std::istream& operator>>(std::istream& stream, Net& data)
        {
            Layers  layers;
            int layerCount;
            if (stream >> layerCount)
            {
                layers.resize(layerCount);
                for (int layerLoop = 0; layerLoop < layerCount; ++layerLoop)
                {
                    int neuronCount;
                    if (stream >> neuronCount)
                    {
                        layers[layerLoop].resize(neuronCount);
                        for (int neuronLoop = 0; neuronLoop < neuronCount; ++neuronLoop)
                        {
                            int axionCount;
                            if (stream >> axionCount)
                            {
                                layers[layerLoop][neuronLoop].resize(axionCount);
                                for (int axionLoop = 0; axionLoop < axionCount; ++axionLoop)
                                {
                                    stream >> layers[layerLoop][neuronLoop][axionLoop];
                                }
                            }
                        }
                    }
                }
            }
            if (stream)
            {
                data.axionLayers.swap(layers);
            }

            return stream;
        }
};

        }

class RunnerNeuralNet: public RunnerMaze
{
    int         generationCount;
    AI::Net     neurialNet;
    int         directionToInfoMap(int dir);
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
