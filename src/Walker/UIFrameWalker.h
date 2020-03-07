#ifndef THORSANVIL_VALKYRIE_WALKER_FRAME_WALKER_H
#define THORSANVIL_VALKYRIE_WALKER_FRAME_WALKER_H

#include "ValkyrieWalkerConfig.h"
#include "UIPanelWalkerCrowd.h"
#include <wx/wx.h>


namespace ThorsAnvil
{
    namespace ValkyrieWalker
    {

class Walker;
class DrawableDistanceGraph;
class DrawableSpeciesGraph;
class FrameWalker: public wxFrame
{
    static constexpr int BUTTON_SAVE_ID       = wxID_HIGHEST + 1;
    static constexpr int BUTTON_RUN1_ID       = wxID_HIGHEST + 2;
    static constexpr int BUTTON_SORT_ID       = wxID_HIGHEST + 3;
    static constexpr int BUTTON_EVOLVE_ID     = wxID_HIGHEST + 4;

    std::vector<Walker>&        walkers;
    DrawableDistanceGraph&      distanceGraph;
    DrawableSpeciesGraph&       speciesGraph;
    PanelWalkerCrowd*           panelWalker;

    public:
        FrameWalker(std::vector<Walker>& walkers, DrawableDistanceGraph& distanceGraph, DrawableSpeciesGraph& speciesGraph);

    private:
        void onQuit(wxCommandEvent& event);
        void onAbout(wxCommandEvent& event);
        void onSave(wxCommandEvent& event);
        void onRun1(wxCommandEvent& event);
        void onSort(wxCommandEvent& event);
        void onEvolve(wxCommandEvent& event);

        DECLARE_EVENT_TABLE()
};

    }
}

#endif
