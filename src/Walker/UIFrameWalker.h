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
class DrawableDistanceHotMap;
class FrameWalker: public wxFrame
{
    static constexpr int BUTTON_SAVE_ID       = wxID_HIGHEST + 1;
    static constexpr int BUTTON_RUN1_ID       = wxID_HIGHEST + 2;
    static constexpr int BUTTON_SORT_ID       = wxID_HIGHEST + 3;
    static constexpr int BUTTON_UPDATE_ID     = wxID_HIGHEST + 4;
    static constexpr int BUTTON_EVOLVE_ID     = wxID_HIGHEST + 5;
    static constexpr int BUTTON_RUN1K_ID      = wxID_HIGHEST + 6;

    std::vector<Walker>&        walkers;
    DrawableDistanceGraph&      distanceGraph;
    DrawableSpeciesGraph&       speciesGraph;
    DrawableDistanceHotMap&     distanceHotMap;
    PanelWalkerCrowd*           panelWalker;

    public:
        FrameWalker(std::vector<Walker>& walkers, DrawableDistanceGraph& distanceGraph, DrawableSpeciesGraph& speciesGraph, DrawableDistanceHotMap& distanceHotMap);

    private:
        void onQuit(wxCommandEvent& event);
        void onAbout(wxCommandEvent& event);
        void onSave(wxCommandEvent& event);
        void onRun1(wxCommandEvent& event);
        void onSort(wxCommandEvent& event);
        void onUpdate(wxCommandEvent& event);
        void onEvolve(wxCommandEvent& event);
        void onRun1K(wxCommandEvent& event);

        DECLARE_EVENT_TABLE()
};

    }
}

#endif
