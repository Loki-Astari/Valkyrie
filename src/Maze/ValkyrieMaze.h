#ifndef THORSANVIL_VALKYRIE_MAZEX_H
#define THORSANVIL_VALKYRIE_MAZEX_H

#include "ValkyrieMazeConfig.h"

#include "Maze.h"
#include "RunnerMaze.h"
#include <ThorsUtil/BackgroundWorker.h>
#include <ThorsUI/Drawable.h>
#include <wx/wx.h>
#include <wx/progdlg.h>
#include <wx/cmdline.h>
#include <fstream>
#include <vector>
#include <memory>


namespace ThorsAnvil
{
    namespace ValkyrieMaze
    {

class ValkyrieMazeInfo: public ThorsUI::Drawable
{
    using RobotStore    = std::vector<std::unique_ptr<RunnerMaze>>;
    using Iter          = RobotStore::iterator;

    Maze            maze;
    RobotStore      robots;
    wxBitmap        bitmap;

    public:
        ValkyrieMazeInfo(std::string const& mazeFile, std::string const& robotFileName);

        virtual void        draw(wxDC& dc)  const;
        virtual wxSize      getSize()       const;

        void    runRobotJob(int iterations, wxProgressDialog* dialog, wxPanel* panel);

        void    saveMaze(std::string const& fileName);
        void    loadMaze(std::string const& fileName);
        void    generateMaze();

        Iter    begin()   {return std::begin(robots);}
        Iter    end()     {return std::end(robots);}

        Maze&   getMaze() {return maze;}
    private:
        void    saveState(int iterations, int iter, wxPanel* panel);
};

class ValkyrieMazeFrame: public wxFrame
{
    ValkyrieMazeInfo&                       valkyrieMaze;
    ThorsUtil::BackgroundWorker             worker;
    wxPanel*                                mazePanel;

    static constexpr int SAVE_MAZE_ID       = wxID_HIGHEST + 1;
    static constexpr int LOAD_MAZE_ID       = wxID_HIGHEST + 2;
    static constexpr int GENERATE_MAZE_ID   = wxID_HIGHEST + 3;
    static constexpr int RUN_1GEN_ID        = wxID_HIGHEST + 4;
    static constexpr int RUN_10GEN_ID       = wxID_HIGHEST + 5;
    static constexpr int RUN_100GEN_ID      = wxID_HIGHEST + 6;
    static constexpr int RUN_1000GEN_ID     = wxID_HIGHEST + 7;
    static constexpr int RUN_10000GEN_ID    = wxID_HIGHEST + 8;
    static constexpr int RUN_100000GEN_ID   = wxID_HIGHEST + 9;
    static constexpr int RUN_1000000GEN_ID  = wxID_HIGHEST + 10;
    static constexpr int RUN_10000000GEN_ID = wxID_HIGHEST + 11;

    public:
        ValkyrieMazeFrame(ValkyrieMazeInfo& valkyrieMaze);

    private:
        void onQuit(wxCommandEvent& event);
        void onAbout(wxCommandEvent& event);
        void onSaveMaze(wxCommandEvent& event);
        void onLoadMaze(wxCommandEvent& event);
        void onGenerateMaze(wxCommandEvent& event);

        void runRobot(int iterations);
        void onRunRobot1(wxCommandEvent& event);
        void onRunRobot10(wxCommandEvent& event);
        void onRunRobot100(wxCommandEvent& event);
        void onRunRobot1000(wxCommandEvent& event);
        void onRunRobot10000(wxCommandEvent& event);
        void onRunRobot100000(wxCommandEvent& event);
        void onRunRobot1000000(wxCommandEvent& event);
        void onRunRobot10000000(wxCommandEvent& event);

        DECLARE_EVENT_TABLE()
};

class ValkyrieMazeApp: public wxApp
{
    static const wxCmdLineEntryDesc cmdLineDesc[];

    std::unique_ptr<ValkyrieMazeInfo>    valkyrieMaze;

    public:
        virtual bool OnInit() override;
};

    }
}

#endif
