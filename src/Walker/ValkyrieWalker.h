#ifndef THORSANVIL_VALKYRIE_WALKER_H
#define THORSANVIL_VALKYRIE_WALKER_H

#include "ValkyrieWalkerConfig.h"
#include "Walker.h"

#include <ThorsUI/Animateable.h>
#include <wx/wx.h>
#include <wx/cmdline.h>
#include <memory>


namespace ThorsAnvil
{
    namespace ValkyrieWalker
    {

class ValkyrieWalkerFrame: public wxFrame
{
    Walker&                       walker;

    public:
        ValkyrieWalkerFrame(Walker& walker);

    private:
        void onQuit(wxCommandEvent& event);
        void onAbout(wxCommandEvent& event);

        DECLARE_EVENT_TABLE()
};

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
