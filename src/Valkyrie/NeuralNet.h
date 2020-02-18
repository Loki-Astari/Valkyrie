#ifndef THORSANVIL_VALKYRIE_NEURAL_NET_H
#define THORSANVIL_VALKYRIE_NEURAL_NET_H

#include <iomanip>
#include <vector>

namespace ThorsAnvil
{
    namespace Valkyrie
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

        friend std::ostream& operator<<(std::ostream& stream, Net const& data);
        friend std::istream& operator>>(std::istream& stream, Net& data);
};

    }
}

#endif
