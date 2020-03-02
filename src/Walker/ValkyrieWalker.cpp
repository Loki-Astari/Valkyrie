#include "ValkyrieWalker.h"
#include <ThorsUI/UIPanelDrawable.h>
#include <ThorsUtil/BackgroundWorker.h>
#include <wx/filename.h>
#include <wx/progdlg.h>
#include <fstream>

using namespace ThorsAnvil::ValkyrieWalker;

DECLARE_APP(ValkyrieWalkerApp)

IMPLEMENT_APP(ValkyrieWalkerApp)

BEGIN_EVENT_TABLE(ValkyrieWalkerFrame, wxFrame)
    EVT_MENU(wxID_ABOUT,        ValkyrieWalkerFrame::onAbout)
    EVT_MENU(wxID_EXIT,         ValkyrieWalkerFrame::onQuit)
    EVT_BUTTON(BUTTON_SAVE_ID,  ValkyrieWalkerFrame::onSave)
    EVT_BUTTON(BUTTON_RUN1_ID,  ValkyrieWalkerFrame::onRun1)
    EVT_BUTTON(BUTTON_SORT_ID,  ValkyrieWalkerFrame::onSort)
    EVT_BUTTON(BUTTON_EVOLVE_ID,ValkyrieWalkerFrame::onEvolve)
END_EVENT_TABLE()

const wxCmdLineEntryDesc ValkyrieWalkerApp::cmdLineDesc[] =
{
     { wxCMD_LINE_SWITCH, "h", "help",      "displays help on the command line parameters", wxCMD_LINE_VAL_NONE,    wxCMD_LINE_OPTION_HELP },
     { wxCMD_LINE_OPTION, "l", "load",      "Load Walkers",                                 wxCMD_LINE_VAL_STRING,  wxCMD_LINE_PARAM_OPTIONAL},
     { wxCMD_LINE_OPTION, "r", "seed",      "Random Seed",                                  wxCMD_LINE_VAL_NUMBER,  wxCMD_LINE_PARAM_OPTIONAL},
     { wxCMD_LINE_NONE, 0, 0, 0, wxCMD_LINE_VAL_NONE, 0 }
};

/************ UI APP ********************/

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

        walkers.emplace_back(walkerFile);
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

    while (walkers.size() < 625)
    {
        walkers.emplace_back();
    }

    ValkyrieWalkerFrame* frame = new ValkyrieWalkerFrame(walkers);

    frame->Show(true);
    return true;
}

/************ UI Frame ********************/

ValkyrieWalkerFrame::ValkyrieWalkerFrame(std::vector<Walker>& walk)
    : wxFrame(nullptr, BUTTON_SAVE_ID, wxT("Valkyrie"))
    , walkers(walk)
    , panelWalker(nullptr)
{

    wxPanel*  panelButton= new wxPanel(this);
    wxButton* buttonSave = new wxButton(panelButton, BUTTON_SAVE_ID, wxT("Save"));
    wxButton* buttonRun  = new wxButton(panelButton, BUTTON_RUN1_ID, wxT("Run 1"));
    wxButton* buttonSort = new wxButton(panelButton, BUTTON_SORT_ID, wxT("Sort"));
    wxButton* buttonEvlv = new wxButton(panelButton, BUTTON_EVOLVE_ID, wxT("Evolve"));

    wxSizer* buttonSizer = new wxStdDialogButtonSizer();
    buttonSizer->Add(buttonSave, wxSizerFlags());
    buttonSizer->Add(buttonRun,  wxSizerFlags());
    buttonSizer->Add(buttonSort, wxSizerFlags());
    buttonSizer->Add(buttonEvlv, wxSizerFlags());
    panelButton->SetSizer(buttonSizer);

    panelWalker = new PanelWalkerCrowd(this, walkers);

    wxSizer* sizer      = new wxBoxSizer(wxVERTICAL);
    sizer->Add(panelButton, wxSizerFlags());
    sizer->Add(panelWalker, wxSizerFlags());

    wxSize clientArea = DoGetBestSize();
    clientArea.y += 32;
    SetSize(clientArea);

    wxMenu* fileMenu = new wxMenu;
    fileMenu->Append(wxID_EXIT,         wxT("E&xit\tAlt-X"),  wxT("Quit Valkyrie"));

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


void ValkyrieWalkerFrame::onAbout(wxCommandEvent& /*event*/)
{
    wxString        msg;
    msg.Printf(wxT("Hello and Welcome to %s"), wxVERSION_STRING);
    wxMessageBox(msg, wxT("About Minimal"), wxOK | wxICON_INFORMATION, this);
}

void ValkyrieWalkerFrame::onQuit(wxCommandEvent& /*event*/)
{
    Close();
}

void ValkyrieWalkerFrame::onSave(wxCommandEvent& /*event*/)
{
    std::ofstream   save("save.walker");
    for (auto const& walker: walkers)
    {
        save << walker;
    }
}

void ValkyrieWalkerFrame::onRun1(wxCommandEvent& /*event*/)
{
    int count = walkers.size();
    int div   = count / 100;
    int max   = count / div + 1;

    wxProgressDialog dialog(wxT("Running all Walkers: "), wxT("Progress: "), max, this, wxPD_APP_MODAL);
    count = 0;
    for (auto& walker: walkers)
    {
        walker.run();
        if (count % div == 0)
        {
            dialog.Update(count / div);
        }
        ++count;
    }
}

void ValkyrieWalkerFrame::onSort(wxCommandEvent& /*event*/)
{
    panelWalker->shuffle();
}

void ValkyrieWalkerFrame::onEvolve(wxCommandEvent& /*event*/)
{
    panelWalker->evolve();
}
