#ifndef THORSANVIL_VALKYRIE_VALKYRIE_RUNNER_APP_H
#define THORSANVIL_VALKYRIE_VALKYRIE_RUNNER_APP_H

#include "ValkyrieRunnerConfig.h"
#include <wx/app.h>

namespace ThorsAnvil
{
    namespace ValkyrieRunner
    {

class ValkyrieRunnerApp: public wxApp
{
    public:
        ValkyrieRunnerApp();
#pragma vera-pushoff
        virtual bool OnInit() override;
#pragma vera-pop
};

    }
}

#endif
