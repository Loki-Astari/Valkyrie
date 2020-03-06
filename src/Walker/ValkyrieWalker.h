#ifndef THORSANVIL_VALKYRIE_WALKER_H
#define THORSANVIL_VALKYRIE_WALKER_H

#include "ValkyrieWalkerConfig.h"
#include <wx/wx.h>
#include <wx/cmdline.h>

namespace ThorsAnvil
{
    namespace ValkyrieWalker
    {

class Walker;
class ValkyrieWalkerApp: public wxApp
{
    static const wxCmdLineEntryDesc cmdLineDesc[];
    std::vector<Walker> walkers;

    public:
#pragma vera-pushoff
        virtual bool OnInit() override;
#pragma vera-pop
    private:
        bool parseCommandLine();
};

    }
}

#endif
