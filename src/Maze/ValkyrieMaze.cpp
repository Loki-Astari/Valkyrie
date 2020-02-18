#include "ValkyrieMaze.h"
#include "RunnerNeuralNet.h"
#include "RunnerAction.h"
#include <Valkyrie/Evolver.h>
#include <ThorsUI/Drawable.h>
#include <ThorsUI/UIPanelDrawable.h>
#include <ThorsUI/UIPanelDrawableClickAction.h>
#include <ThorsUI/UIBasicFrame.h>
#include <ThorsUI/UIPanelAnimateableRunner.h>
#include <ThorsUI/UIPanelAnimateableSteper.h>
#include <wx/filename.h>
#include <algorithm>

using namespace ThorsAnvil::ValkyrieMaze;

DECLARE_APP(ValkyrieMazeApp)

IMPLEMENT_APP(ValkyrieMazeApp)

BEGIN_EVENT_TABLE(ValkyrieMazeFrame, wxFrame)
    EVT_MENU(wxID_ABOUT,        ValkyrieMazeFrame::onAbout)
    EVT_MENU(wxID_EXIT,         ValkyrieMazeFrame::onQuit)
    EVT_MENU(SAVE_MAZE_ID,      ValkyrieMazeFrame::onSaveMaze)
    EVT_MENU(LOAD_MAZE_ID,      ValkyrieMazeFrame::onLoadMaze)
    EVT_MENU(GENERATE_MAZE_ID,  ValkyrieMazeFrame::onGenerateMaze)
    EVT_MENU(RUN_1GEN_ID,       ValkyrieMazeFrame::onRunRobot1)
    EVT_MENU(RUN_10GEN_ID,      ValkyrieMazeFrame::onRunRobot10)
    EVT_MENU(RUN_100GEN_ID,     ValkyrieMazeFrame::onRunRobot100)
    EVT_MENU(RUN_1000GEN_ID,    ValkyrieMazeFrame::onRunRobot1000)
    EVT_MENU(RUN_10000GEN_ID,   ValkyrieMazeFrame::onRunRobot10000)
    EVT_MENU(RUN_100000GEN_ID,  ValkyrieMazeFrame::onRunRobot100000)
    EVT_MENU(RUN_1000000GEN_ID, ValkyrieMazeFrame::onRunRobot1000000)
    EVT_MENU(RUN_10000000GEN_ID,ValkyrieMazeFrame::onRunRobot10000000)
END_EVENT_TABLE()

const wxCmdLineEntryDesc ValkyrieMazeApp::cmdLineDesc[] =
{
     { wxCMD_LINE_SWITCH, "h", "help",      "displays help on the command line parameters", wxCMD_LINE_VAL_NONE,    wxCMD_LINE_OPTION_HELP },
     { wxCMD_LINE_OPTION, "m", "maze",      "Load maze file",                               wxCMD_LINE_VAL_STRING,  wxCMD_LINE_PARAM_OPTIONAL},
     { wxCMD_LINE_OPTION, "r", "robots",    "Load robot file",                              wxCMD_LINE_VAL_STRING,  wxCMD_LINE_PARAM_OPTIONAL},
     { wxCMD_LINE_OPTION, "s", "seed",      "Random Seed",                                  wxCMD_LINE_VAL_NUMBER,  wxCMD_LINE_PARAM_OPTIONAL},
     { wxCMD_LINE_NONE, 0, 0, 0, wxCMD_LINE_VAL_NONE, 0 }
};

/************ Application ********************/

ValkyrieMazeInfo::ValkyrieMazeInfo(std::string const& mazeFile, std::string const& robotFileName)
    : maze(std::ifstream(mazeFile))
    , bitmap(832, 832)
{
    {
        wxMemoryDC  dc(bitmap);
        dc.SetBackground(*wxWHITE_BRUSH);
        dc.Clear();
    }

    std::ifstream robotFile(robotFileName);
    std::map<std::string, Valkyrie::RunnerReader::Creator>    creatorMap{{"RunnerNeuralNet", [&maze = this->maze](std::istream& stream){return new RunnerNeuralNet(maze, stream);}}};

    Valkyrie::Runner*     runner;
    while (robotFile >> Valkyrie::RunnerReader{runner, creatorMap})
    {
        robots.emplace_back(dynamic_cast<RunnerMaze*>(runner));
    }
    while (robots.size() < 100)
    {
        robots.emplace_back(new RunnerNeuralNet(maze));
    }
}

void ValkyrieMazeInfo::draw(wxDC& dc) const
{
    dc.DrawBitmap(bitmap, 0, 0);
    maze.draw(dc);
}

wxSize ValkyrieMazeInfo::getSize() const
{
    return maze.getSize();
}

void ValkyrieMazeInfo::runRobotJob(int iterations, wxProgressDialog* dialog, wxPanel* panel)
{
    Valkyrie::Evolver     evolver;
    for (int iter = 0; iter < iterations; ++iter)
    {
        evolver.evolve(std::begin(robots), std::end(robots), [&dialog, &iter](int distance)
            {
                if (dialog)
                {
                    dialog->Update(iter*100 + distance);
                }
            },
            [vMaze = this, &iter, &iterations, &panel]()
            {
                if (iter != 0 && iter % 100 == 0)
                {
                    vMaze->saveState(iterations, iter, panel);
                }
            }
        );

    }
    saveState(iterations, iterations, panel);
}

void ValkyrieMazeInfo::saveState(int iterations, int iter, wxPanel* panel)
{
    wxMemoryDC      dc(bitmap);
    int             tScore = 0;
    std::ofstream   save("save.robots");
    robots[0]->animateReset(dc);
    for (auto const& robot: robots)
    {
        save << (*robot);
        robot->animateOneLoop(dc);
        tScore += (robot->score() / 100);
    }
    std::cout << "Saved: " << time(nullptr)
              << " Iterations: " << iterations
              << " Iter: " << iter
              << " Max Score: " << robots[0]->score()
              << " Median Score: " << robots[50]->score()
              << " Avr Score: " << tScore
              << " Min Score: " << robots[99]->score()
              << "\n";
    panel->Refresh();
    wxPostEvent(panel, wxPaintEvent{});
}

void ValkyrieMazeInfo::saveMaze(std::string const& fileName)
{
    std::ofstream   mazeFile(fileName);
    mazeFile << maze;
}

void ValkyrieMazeInfo::loadMaze(std::string const& fileName)
{
    std::ifstream   mazeFile(fileName);
    mazeFile >> maze;
}

void ValkyrieMazeInfo::generateMaze()
{
    MD::MazeGenerator   mazeGenerator(100, 100);
    mazeGenerator(maze);
}

/************ UI APP ********************/

bool ValkyrieMazeApp::OnInit()
{
    std::cout.sync_with_stdio(false);
    std::cin.tie(nullptr);

    wxCmdLineParser     commandLineParser(cmdLineDesc, argc, argv);

    int res = commandLineParser.Parse(false);
    if (res == -1 || res > 0 || commandLineParser.Found(wxT("h")))
    {
        commandLineParser.Usage();
        return false;
    }

    std::string     mazeFileName;
    std::string     robotFileName;
    wxString        cmdFileName;
    if (commandLineParser.Found(wxT("m"), &cmdFileName))
    {
        wxFileName  fileName(cmdFileName);
        fileName.Normalize(wxPATH_NORM_LONG | wxPATH_NORM_DOTS | wxPATH_NORM_TILDE | wxPATH_NORM_ABSOLUTE);
        cmdFileName = fileName.GetFullPath();

        mazeFileName = cmdFileName;
    }

    if (commandLineParser.Found(wxT("r"), &cmdFileName))
    {
        wxFileName  fileName(cmdFileName);
        fileName.Normalize(wxPATH_NORM_LONG | wxPATH_NORM_DOTS | wxPATH_NORM_TILDE | wxPATH_NORM_ABSOLUTE);
        cmdFileName = fileName.GetFullPath();

        robotFileName = cmdFileName;
    }

    long seed;
    if (commandLineParser.Found(wxT("s"), &seed))
    {
        ThorsUtil::Random::defaltGeneratorSeedSet(seed);
    }

    valkyrieMaze.reset(new ValkyrieMazeInfo(mazeFileName, robotFileName));
    ValkyrieMazeFrame* frame = new ValkyrieMazeFrame(*valkyrieMaze);

    frame->Show(true);
    return true;
}

/************ UI Frame ********************/

ValkyrieMazeFrame::ValkyrieMazeFrame(ValkyrieMazeInfo& valkyrieMaze)
    : wxFrame(nullptr, wxID_ANY, wxT("Valkyrie"))
    , valkyrieMaze(valkyrieMaze)
    , mazePanel(nullptr)
{
    wxSizer* sizer      = new wxBoxSizer(wxVERTICAL);

    mazePanel = new ThorsUI::PanelDrawable(this, valkyrieMaze);
    sizer->Add(mazePanel, 1, 0, 10, nullptr);

    wxSizer* robotSizer = new wxGridSizer(10, 5, 5);
    sizer->Add(robotSizer, wxSizerFlags());

    for (auto& robot: valkyrieMaze)
    {
        auto robotPanel = new ThorsUI::PanelDrawableClickAction(this, *robot, [&robot, &maze = valkyrieMaze.getMaze()](ThorsUI::PanelDrawableClickAction&)
        {
            auto* frame = new ThorsUI::BasicFrameWithHolder<RunnerAction>(RunnerAction(maze, *robot), wxT("Maze Run"), [](ThorsUI::BasicFrame* frame, wxSizer& sizer)
            {
                RunnerAction& action = dynamic_cast<ThorsUI::BasicFrameWithHolder<RunnerAction>*>(frame)->getData();
#if 1
                auto* actionPanel = new ThorsUI::PanelAnimateableRunner(frame, action, 40);
#else
                auto* actionPanel = new ThorsUI::PanelAnimateableSteper(frame, action);
#endif
                sizer.Add(actionPanel, 1, 0, 10, nullptr);
            });
            frame->Show(true);
        });
        robotSizer->Add(robotPanel, wxSizerFlags());
    }


    wxSize clientArea = DoGetBestSize();
    clientArea.y += 32;
    SetSize(clientArea);

    wxMenu* fileMenu = new wxMenu;
    fileMenu->Append(wxID_EXIT,         wxT("E&xit\tAlt-X"),  wxT("Quit Valkyrie"));
    fileMenu->Append(SAVE_MAZE_ID,      wxT("&Save Maze"),    wxT("Save Current Maze to File"));
    fileMenu->Append(LOAD_MAZE_ID,      wxT("&Load Maze"),    wxT("Load Current Maze to File"));
    fileMenu->Append(GENERATE_MAZE_ID,  wxT("&Generate Maze"),wxT("Generate new Random Maze"));
    fileMenu->AppendSeparator();
    fileMenu->Append(RUN_1GEN_ID,       wxT("&Run Robots 1"),       wxT("Run the robots to see how far they get"));
    fileMenu->Append(RUN_10GEN_ID,      wxT("Run Robots 10"),       wxT("Run the robots to see how far they get"));
    fileMenu->Append(RUN_100GEN_ID,     wxT("Run Robots 100"),      wxT("Run the robots to see how far they get"));
    fileMenu->Append(RUN_1000GEN_ID,    wxT("Run Robots 1000"),     wxT("Run the robots to see how far they get"));
    fileMenu->Append(RUN_10000GEN_ID,   wxT("Run Robots 10000"),    wxT("Run the robots to see how far they get"));
    fileMenu->Append(RUN_100000GEN_ID,  wxT("Run Robots 100000"),   wxT("Run the robots to see how far they get"));
    fileMenu->Append(RUN_1000000GEN_ID, wxT("Run Robots 1000000"),  wxT("Run the robots to see how far they get"));
    fileMenu->Append(RUN_10000000GEN_ID,wxT("Run Robots 10000000"), wxT("Run the robots to see how far they get"));

    wxMenu* helpMenu = new wxMenu;
    helpMenu->Append(wxID_ABOUT, wxT("&About...\tF1"), wxT("Show about dialog"));

    wxMenuBar* menuBar = new wxMenuBar{};
    menuBar->Append(fileMenu, wxT("&File"));
    menuBar->Append(helpMenu, wxT("&Help"));

    SetMenuBar(menuBar);

    CreateStatusBar(2);
    SetStatusText(wxT("Welcome to Valkyrie"));

    SetSizer(sizer);
    sizer->Fit(this);
    sizer->SetSizeHints(this);
}


void ValkyrieMazeFrame::onAbout(wxCommandEvent& /*event*/)
{
    wxString        msg;
    msg.Printf(wxT("Hello and Welcome to %s"), wxVERSION_STRING);
    wxMessageBox(msg, wxT("About Minimal"), wxOK | wxICON_INFORMATION, this);
}

void ValkyrieMazeFrame::onQuit(wxCommandEvent& /*event*/)
{
    Close();
}

void ValkyrieMazeFrame::onSaveMaze(wxCommandEvent& /*event*/)
{
    wxFileDialog saveFile(this, _("Save Maze file"), "", "", "Maze files (*.maze)|*.maze", wxFD_SAVE);
    if (saveFile.ShowModal() == wxID_CANCEL)
    {
        return;
    }

    wxFileName      fileName        = saveFile.GetPath();
    fileName.Normalize(wxPATH_NORM_LONG | wxPATH_NORM_DOTS | wxPATH_NORM_TILDE | wxPATH_NORM_ABSOLUTE);

    valkyrieMaze.saveMaze(fileName.GetFullPath().ToStdString());
}

void ValkyrieMazeFrame::onLoadMaze(wxCommandEvent& /*event*/)
{
    wxFileDialog loadFile(this, _("Load Maze file"), "", "", "Maze files (*.maze)|*.maze", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
    if (loadFile.ShowModal() == wxID_CANCEL)
    {
        return;
    }

    wxFileName      fileName        = loadFile.GetPath();
    fileName.Normalize(wxPATH_NORM_LONG | wxPATH_NORM_DOTS | wxPATH_NORM_TILDE | wxPATH_NORM_ABSOLUTE);

    valkyrieMaze.loadMaze(fileName.GetFullPath().ToStdString());

    Layout();
    GetSizer()->Fit(this);
    Refresh();
}

void ValkyrieMazeFrame::onGenerateMaze(wxCommandEvent& /*event*/)
{
    valkyrieMaze.generateMaze();

    Layout();
    GetSizer()->Fit(this);
    Refresh();
}

void ValkyrieMazeFrame::onRunRobot1(wxCommandEvent& /*event*/)          {runRobot(1);}
void ValkyrieMazeFrame::onRunRobot10(wxCommandEvent& /*event*/)         {runRobot(10);}
void ValkyrieMazeFrame::onRunRobot100(wxCommandEvent& /*event*/)        {runRobot(100);}
void ValkyrieMazeFrame::onRunRobot1000(wxCommandEvent& /*event*/)       {runRobot(1000);}
void ValkyrieMazeFrame::onRunRobot10000(wxCommandEvent& /*event*/)      {runRobot(10000);}
void ValkyrieMazeFrame::onRunRobot100000(wxCommandEvent& /*event*/)     {runRobot(100000);}
void ValkyrieMazeFrame::onRunRobot1000000(wxCommandEvent& /*event*/)    {runRobot(1000000);}
void ValkyrieMazeFrame::onRunRobot10000000(wxCommandEvent& /*event*/)   {runRobot(10000000);}

void ValkyrieMazeFrame::runRobot(int iterations)
{
    if (iterations <= 100)
    {
        wxProgressDialog dialog(wxT("Running all Robots: "), wxT("Progress: "), iterations * 100, this, wxPD_APP_MODAL);
        valkyrieMaze.runRobotJob(iterations, &dialog, mazePanel);
    }
    else
    {
        worker.addWork([&valkyrieMaze = this->valkyrieMaze, &mazePanel = this->mazePanel, iterations](){valkyrieMaze.runRobotJob(iterations, nullptr, mazePanel);});
        std::cout << "Work Added\n";
    }
}
