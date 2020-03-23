#include "UIAppValkyrieRunner.h"
#include "ThorsUtil/Random.h"

using namespace ThorsAnvil::ValkyrieRunner;

const wxCmdLineEntryDesc UIAppValkyrieRunner::cmdLineDesc[] =
{
     { wxCMD_LINE_SWITCH, "h", "help",      "displays help on the command line parameters", wxCMD_LINE_VAL_NONE,    wxCMD_LINE_OPTION_HELP },
     { wxCMD_LINE_OPTION, "r", "seed",      "Random Seed",                                  wxCMD_LINE_VAL_NUMBER,  wxCMD_LINE_PARAM_OPTIONAL},
     { wxCMD_LINE_OPTION, "c", "count",     "Count of runners generated",                   wxCMD_LINE_VAL_NUMBER,  wxCMD_LINE_PARAM_OPTIONAL},
     { wxCMD_LINE_NONE, 0, 0, 0, wxCMD_LINE_VAL_NONE, 0 }
};

BEGIN_EVENT_TABLE(UIAppValkyrieRunner, wxApp)
    EVT_MENU(wxID_ABOUT,            UIAppValkyrieRunner::onAbout)
    EVT_MENU(wxID_EXIT,             UIAppValkyrieRunner::onQuit)
    EVT_BUTTON(BUTTON_SAVE_ID,      UIAppValkyrieRunner::onSave)
    EVT_BUTTON(BUTTON_RUN1_ID,      UIAppValkyrieRunner::onRun1)
    EVT_BUTTON(BUTTON_SORT_ID,      UIAppValkyrieRunner::onSort)
    EVT_BUTTON(BUTTON_UPDATE_ID,    UIAppValkyrieRunner::onUpdate)
    EVT_BUTTON(BUTTON_EVOLVE_ID,    UIAppValkyrieRunner::onEvolve)
    EVT_BUTTON(BUTTON_RUN1K_ID,     UIAppValkyrieRunner::onRun1K)
END_EVENT_TABLE()

UIAppValkyrieRunner::UIAppValkyrieRunner()
    : runnerCount(100)
    , distanceGraph(runners)
    , distanceHotMap(runners)
    , speciesGraph(runners)
{}

bool UIAppValkyrieRunner::OnInit()
{
    std::cout.sync_with_stdio(false);
    std::cin.tie(nullptr);

    if (!parseCommandLine())
    {
        return false;
    }

    std::default_random_engine& randomGenerator = ThorsUtil::Random::getRandomGenerator();

    runners.reserve(runnerCount);
    for (int loop = 0; loop < runnerCount; ++loop)
    {
        runners.emplace_back(randomGenerator);
    }

    wxMenu* fileMenu        = new wxMenu;
    fileMenu->Append(wxID_EXIT,         wxT("E&xit\tAlt-X"),  wxT("Quit Valkyrie Runner"));

    wxMenu* helpMenu        = new wxMenu;
    helpMenu->Append(wxID_ABOUT, wxT("&About...\tF1"), wxT("Show about dialog"));

    wxMenuBar* menuBar      = new wxMenuBar{};
    menuBar->Append(fileMenu, wxT("&File"));
    menuBar->Append(helpMenu, wxT("&Help"));

    wxFrame*    frameButton = make_FrameSimpleHorz(nullptr, wxID_ANY, wxT("Runner Button"), wxPoint{100, 100}, buttonBuilder);
    frameButton->SetMenuBar(menuBar);
    frameButton->CreateStatusBar(1);
    frameButton->SetStatusText(wxT("Welcome to Valkyrie"));
    frameButton->Fit();
    frameButton->Show();
    wxSize  sizeButton      = frameButton->GetSize();

    wxFrame*    frameGraph  = make_FrameSimpleHorz(nullptr, wxID_ANY, wxT("Runner Graph"), wxPoint{100, 100 + sizeButton.y},
                                                   distanceGraph);
                                                   //distanceGraph, speciesGraph, distanceHotMap);
    frameGraph->Show();
    wxSize      sizeGraph   = frameGraph->GetSize();

    wxFrame*    frameWalker = make_FrameSimpleHorz(nullptr, wxID_ANY, wxT("Runner Walker Farm"), wxPoint{100, 100 + sizeGraph.y + sizeButton.y}, tmp);
    frameWalker->Show();

    return true;
}

bool UIAppValkyrieRunner::parseCommandLine()
{
    wxCmdLineParser     commandLineParser(cmdLineDesc, argc, argv);

    int res = commandLineParser.Parse(false);
    if (res == -1 || res > 0 || commandLineParser.Found(wxT("h")))
    {
        commandLineParser.Usage();
        return false;
    }

    long seed;
    if (commandLineParser.Found(wxT("r"), &seed))
    {
        // Set seed for random number generator.
        ThorsUtil::Random::defaltGeneratorSeedSet(seed);
    }

    long count;
    if (commandLineParser.Found(wxT("c"), &count))
    {
        runnerCount = count;
    }

    return true;
}
