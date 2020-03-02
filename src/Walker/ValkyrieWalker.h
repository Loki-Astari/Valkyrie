#ifndef THORSANVIL_VALKYRIE_WALKER_H
#define THORSANVIL_VALKYRIE_WALKER_H

#include "ValkyrieWalkerConfig.h"
#include "Walker.h"
#include "UIPanelWalkerCrowd.h"

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
    static constexpr int BUTTON_SAVE_ID       = wxID_HIGHEST + 1;
    static constexpr int BUTTON_RUN1_ID       = wxID_HIGHEST + 2;
    static constexpr int BUTTON_SORT_ID       = wxID_HIGHEST + 3;

    std::vector<Walker>&        walkers;
    PanelWalkerCrowd*           panelWalker;

    public:
        ValkyrieWalkerFrame(std::vector<Walker>& walkers);

    private:
        void onQuit(wxCommandEvent& event);
        void onAbout(wxCommandEvent& event);
        void onSave(wxCommandEvent& event);
        void onRun1(wxCommandEvent& event);
        void onSort(wxCommandEvent& event);

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
