#ifndef THORSANVIL_VALKYRIE_RUNNER_H
#define THORSANVIL_VALKYRIE_RUNNER_H

#include <ThorsUI/Animateable.h>
#include <map>
#include <string>
#include <functional>

namespace ThorsAnvil
{
    namespace Valkyrie
    {

class Runner;
struct RunnerReader
{
    using Creator = std::function<Runner*(std::istream&)>;

    Runner*&                        runner;
    std::map<std::string, Creator>& creatorMap;
};

class Runner: public ThorsUI::Animateable
{
    public:
        Runner()          {}
        virtual ~Runner() {}
        // Specifically allow Move Assignment.
        // We will use this to mutate the Runner in the Evolve:
        Runner& operator=(Runner const& rhs)    {reproduce(rhs);return *this;}

        // Delete Copy/Move Construction and Move Assignment.
        Runner(Runner const&)             = delete;
        Runner(Runner&&)                  = delete;
        Runner& operator=(Runner&&)       = delete;

        virtual void    run()                           = 0;
        virtual int     score() const                   = 0;
        virtual void    mutate()                        = 0;
        virtual void    mutate(Runner&)                 = 0;

        virtual void    print(std::ostream& stream) const;
        virtual void    reproduce(Runner const& parent) = 0;
};

std::ostream& operator<<(std::ostream& stream, Runner const& data);

std::istream& operator>>(std::istream& stream, RunnerReader const& data);

    }
}

#endif
