#include "Runner.h"

using namespace ThorsAnvil::Valkyrie;

void Runner::print(std::ostream& stream) const
{
    stream << "Not Serializable\n";
}

std::ostream& ThorsAnvil::Valkyrie::operator<<(std::ostream& stream, ThorsAnvil::Valkyrie::Runner const& data)
{
    data.print(stream);
    return stream;
}

std::istream& ThorsAnvil::Valkyrie::operator>>(std::istream& stream, ThorsAnvil::Valkyrie::RunnerReader const& data)
{
    data.runner = nullptr;

    std::string runnerType;
    if (stream >> runnerType)
    {
        auto find = data.creatorMap.find(runnerType);
        if (find == data.creatorMap.end())
        {
            std::cerr << "throw: ThorsAnvil::Valkyrie::operator>>\n";
            throw std::runtime_error("Unknown Runner Type");
        }
        data.runner = find->second(stream);
    }
    return stream;
}
