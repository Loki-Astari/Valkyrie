#include "ValkyrieWalker.h"
#include "UIFrameWalkerCrowd.h"
#include "UIFrameGraph.h"
#include "Walker.h"
#include "ThorsUtil/Random.h"
#include <wx/filename.h>
#include <fstream>
#include <iterator>

using namespace ThorsAnvil::ValkyrieWalker;

DECLARE_APP(ValkyrieWalkerApp)

IMPLEMENT_APP(ValkyrieWalkerApp)

const wxCmdLineEntryDesc ValkyrieWalkerApp::cmdLineDesc[] =
{
     { wxCMD_LINE_SWITCH, "h", "help",      "displays help on the command line parameters", wxCMD_LINE_VAL_NONE,    wxCMD_LINE_OPTION_HELP },
     { wxCMD_LINE_OPTION, "l", "load",      "Load Walkers",                                 wxCMD_LINE_VAL_STRING,  wxCMD_LINE_PARAM_OPTIONAL},
     { wxCMD_LINE_OPTION, "r", "seed",      "Random Seed",                                  wxCMD_LINE_VAL_NUMBER,  wxCMD_LINE_PARAM_OPTIONAL},
     { wxCMD_LINE_NONE, 0, 0, 0, wxCMD_LINE_VAL_NONE, 0 }
};

/************ UI APP ********************/

ValkyrieWalkerApp::ValkyrieWalkerApp()
    : distanceGraph(walkers)
    , speciesGraph(walkers)
    , distanceHotMap(walkers)
{}

bool ValkyrieWalkerApp::parseCommandLine()
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
        ThorsUtil::Random::defaltGeneratorSeedSet(seed);
    }


    wxString        cmdFileName;
    std::string     walkerFileName;
    if (commandLineParser.Found(wxT("l"), &cmdFileName))
    {
        wxFileName  fileName(cmdFileName);
        fileName.Normalize(wxPATH_NORM_LONG | wxPATH_NORM_DOTS | wxPATH_NORM_TILDE | wxPATH_NORM_ABSOLUTE);
        cmdFileName = fileName.GetFullPath();

        walkerFileName = cmdFileName;
        std::ifstream   walkerFile(cmdFileName);

        walkerFile  >> distanceGraph
                    >> speciesGraph
                    >> distanceHotMap;
        walkers.clear();
        walkers.insert(std::end(walkers), std::istream_iterator<Walker>{walkerFile}, std::istream_iterator<Walker>{});
    }
    return true;
}

bool ValkyrieWalkerApp::OnInit()
{
    std::cout.sync_with_stdio(false);
    std::cin.tie(nullptr);

    if (!parseCommandLine())
    {
        return false;
    }

    while (walkers.size() < 1000)
    {
        walkers.emplace_back();
    }

    distanceGraph.tick();
    speciesGraph.tick();

    FrameWalkerCrowd*   walkerFrame = new FrameWalkerCrowd(walkers, distanceGraph, speciesGraph, distanceHotMap);
    FrameGraph*         graphFrame  = new FrameGraph(walkerFrame, distanceGraph, speciesGraph, distanceHotMap);

    walkerFrame->Show(true);
    graphFrame->Show(true);
    return true;
}
