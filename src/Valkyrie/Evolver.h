#ifndef THORSANVIL_VALKYRIE_EVOLVER_H
#define THORSANVIL_VALKYRIE_EVOLVER_H

#include <ThorsUtil/Random.h>

namespace ThorsAnvil
{
    namespace Valkyrie
    {

class Evolver
{
    public:
        template<typename I, typename A, typename B>
        void evolve(I begin, I end, A const& feedback, B const& saveState)
        {
            runRobotThrough(begin, end, feedback);
            runRobotSort(begin, end);

            saveState();

            runRobotEvolve(begin, end);
        }
    private:
        template<typename I, typename A>
        void runRobotThrough(I begin, I end, A const& feedback)
        {
            for (auto loop = begin; loop != end; ++loop)
            {
                feedback(std::distance(begin, loop));
                (*loop)->run();
            }
        }

        template<typename I>
        void runRobotSort(I begin, I end)
        {
            std::sort(begin, end, [](auto const& lhs, auto const& rhs){return lhs->score() > rhs->score();});
        }

        template<typename I>
        void runRobotEvolve(I begin, I end)
        {
            std::default_random_engine&     generator = ThorsUtil::Random::getRandomGenerator();
            std::uniform_real_distribution  random;

            auto    parent      = begin;

            for (auto loop = begin; loop != end; ++loop)
            {
                if (random(generator) < (std::distance(begin, loop) / 75.0))
                {
                    // Kill of bad robots.
                    // The worse the robot the more likely to kill. This should cull approx 50% of the robots.
                    // They will be replaced by a clone of more successful robot (with a mutation).
                    //robots[killIndex].reset(new RunnerNeuralNet(dynamic_cast<ThorsAnvil::Valkyrie::RunnerNeuralNet&>(*robots[killCount % killIndex])));
                    //std::cout << "Evolve: " << killIndex << "(" << robots[killIndex]->score() << ") Replaced with: " << killCount % killIndex << " (" << robots[killCount % killIndex]->score() << ")\n";
                    (*loop)->mutate(**parent);
                    ++parent;
                }
                else
                {
                    if (random(generator) < 0.25)
                    {
                        // If not cull 25% chance of mutating anyway.
                        (*loop)->mutate();
                    }
                    else
                    {
                        // 40% should remain unchanged.
                    }
                }
            }
        }
};


    }
}

#endif
