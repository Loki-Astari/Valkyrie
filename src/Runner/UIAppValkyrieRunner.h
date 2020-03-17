#ifndef THORSANVIL_VALKYRIE_VALKYRIE_RUNNER_APP_H
#define THORSANVIL_VALKYRIE_VALKYRIE_RUNNER_APP_H

#include "ValkyrieRunnerConfig.h"
#include <wx/app.h>
#include <iostream>

namespace ThorsAnvil
{
    namespace ValkyrieRunner
    {

class UIAppValkyrieRunner: public wxApp
{
    public:
        UIAppValkyrieRunner();
#pragma vera-pushoff
        virtual bool OnInit() override;
#pragma vera-pop


        static constexpr int BUTTON_SAVE_ID       = wxID_HIGHEST + 1;
        static constexpr int BUTTON_RUN1_ID       = wxID_HIGHEST + 2;
        static constexpr int BUTTON_SORT_ID       = wxID_HIGHEST + 3;
        static constexpr int BUTTON_UPDATE_ID     = wxID_HIGHEST + 4;
        static constexpr int BUTTON_EVOLVE_ID     = wxID_HIGHEST + 5;
        static constexpr int BUTTON_RUN1K_ID      = wxID_HIGHEST + 6;


    private:
        wxFrame*        frameButton;
        wxFrame*        frameGraph;
        wxFrame*        frameWalkerFarm;

        DECLARE_EVENT_TABLE()

        void onQuit(wxCommandEvent& /*event*/)              {std::cerr << "onQuit()\n";}
        void onAbout(wxCommandEvent& /*event*/)             {std::cerr << "onAbout()\n";}
        void onSave(wxCommandEvent& /*event*/)              {std::cerr << "onSave()\n";}
        void onRun1(wxCommandEvent& /*event*/)              {std::cerr << "onRun1()\n";}
        void onRun1K(wxCommandEvent& /*event*/)             {std::cerr << "onRun1K()\n";}
        void onSort(wxCommandEvent& /*event*/)              {std::cerr << "onSort()\n";}
        void onUpdate(wxCommandEvent& /*event*/)            {std::cerr << "onUpdate()\n";}
        void onEvolve(wxCommandEvent& /*event*/)            {std::cerr << "onEvolve()\n";}
};

    }
}

#endif
