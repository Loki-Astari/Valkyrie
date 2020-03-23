#ifndef THORSANVIL_VALKYRIE_RUNNER_RUNNER_H
#define THORSANVIL_VALKYRIE_RUNNER_RUNNER_H

#include "ThorsUI/Drawable.h"
#include <random>
#include <string>

namespace ThorsAnvil
{
    namespace ValkyrieRunner
    {

class Runner
{
    public:
        Runner(std::default_random_engine&) {}

        int         score()     const {return 0;}
        std::string species()   const {return "";}
};


    }
}

#endif
