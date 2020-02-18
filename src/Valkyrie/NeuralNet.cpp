#include "NeuralNet.h"
#include <ThorsUtil/Random.h>
#include <ThorsUtil/Util.h>

using namespace ThorsAnvil::Valkyrie;

Net::Net(int inputSize, int outputSize)
{
    std::default_random_engine&     generator = ThorsUtil::Random::getRandomGenerator();
    std::uniform_real_distribution  distribution(-1.0, 1.0);

    int layerSize = 2 + 1; // 1 for the output Layer.
    axionLayers.resize(layerSize);

    for (int layerLoop = 0; layerLoop < layerSize; ++layerLoop)
    {
        int neuronCount      = layerLoop == (layerSize - 1) ? outputSize : inputSize * 1.5;
        axionLayers[layerLoop].resize(neuronCount);
        for (int neuronLoop = 0; neuronLoop < neuronCount; ++neuronLoop)
        {
            int axionCount  = (layerLoop == 0) ? inputSize : axionLayers[layerLoop - 1].size();
            axionLayers[layerLoop][neuronLoop].resize(axionCount);
            for (int axionLoop = 0; axionLoop < axionCount; ++axionLoop)
            {
                axionLayers[layerLoop][neuronLoop][axionLoop]   = distribution(generator);
            }
        }
    }
}

std::vector<float> Net::step(std::vector<float>& input)
{
    std::vector<float>  result;
    for (std::size_t layer = 0; layer < axionLayers.size(); ++layer)
    {
        result.resize(axionLayers[layer].size());

        for (std::size_t loop = 0; loop < axionLayers[layer].size(); ++loop)
        {
            for (std::size_t axionLoop = 0; axionLoop < axionLayers[layer][loop].size(); ++axionLoop)
            {
                result[loop] += input[axionLoop] * axionLayers[layer][loop][axionLoop];
            }
        }
        input = std::move(result);
    }

    return input;
}

void Net::mutate()
{
    std::default_random_engine&     generator = ThorsUtil::Random::getRandomGenerator();
    auto layer  = ThorsUtil::getRandomContainerIterator(axionLayers, generator);
    auto neuron = ThorsUtil::getRandomContainerIterator(*layer,      generator);
    auto axion  = ThorsUtil::getRandomContainerIterator(*neuron,     generator);

    std::uniform_real_distribution  distribution(-1.0, 1.0);
    *axion = distribution(generator);
}


std::ostream& ThorsAnvil::Valkyrie::operator<<(std::ostream& stream, Net const& data)
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

std::istream& ThorsAnvil::Valkyrie::operator>>(std::istream& stream, Net& data)
{
    Net::Layers  layers;
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
