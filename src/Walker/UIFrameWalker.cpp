#include "UIFrameWalker.h"
#include "UIFrameGraph.h"
#include "Walker.h"
#include "DrawableDistanceGraph.h"
#include "DrawableSpeciesGraph.h"
#include "DrawableDistanceHotMap.h"
#include <wx/progdlg.h>
#include <fstream>

using namespace ThorsAnvil::ValkyrieWalker;

BEGIN_EVENT_TABLE(FrameWalker, wxFrame)
    EVT_MENU(wxID_ABOUT,        FrameWalker::onAbout)
    EVT_MENU(wxID_EXIT,         FrameWalker::onQuit)
    EVT_BUTTON(BUTTON_SAVE_ID,  FrameWalker::onSave)
    EVT_BUTTON(BUTTON_RUN1_ID,  FrameWalker::onRun1)
    EVT_BUTTON(BUTTON_SORT_ID,  FrameWalker::onSort)
    EVT_BUTTON(BUTTON_UPDATE_ID,FrameWalker::onUpdate)
    EVT_BUTTON(BUTTON_EVOLVE_ID,FrameWalker::onEvolve)
    EVT_BUTTON(BUTTON_RUN1K_ID, FrameWalker::onRun1K)
END_EVENT_TABLE()

FrameWalker::FrameWalker(std::vector<Walker>& walk, DrawableDistanceGraph& distanceGraph, DrawableSpeciesGraph& speciesGraph, DrawableDistanceHotMap& distanceHotMap)
    : wxFrame(nullptr, wxID_ANY , wxT("Valkyrie"))
    , walkers(walk)
    , distanceGraph(distanceGraph)
    , speciesGraph(speciesGraph)
    , distanceHotMap(distanceHotMap)
    , panelWalker(nullptr)
{

    wxPanel*  panelButton= new wxPanel(this);
    wxButton* buttonSave = new wxButton(panelButton, BUTTON_SAVE_ID, wxT("Save"));
    wxButton* buttonRun  = new wxButton(panelButton, BUTTON_RUN1_ID, wxT("Run 1"));
    wxButton* buttonSort = new wxButton(panelButton, BUTTON_SORT_ID, wxT("Sort"));
    wxButton* buttonUpd  = new wxButton(panelButton, BUTTON_UPDATE_ID, wxT("Update"));
    wxButton* buttonEvlv = new wxButton(panelButton, BUTTON_EVOLVE_ID, wxT("Evolve"));
    wxButton* buttonR1K  = new wxButton(panelButton, BUTTON_RUN1K_ID,  wxT("Run 1K"));

    wxSizer* buttonSizer = new wxStdDialogButtonSizer();
    buttonSizer->Add(buttonSave, wxSizerFlags());
    buttonSizer->Add(buttonRun,  wxSizerFlags());
    buttonSizer->Add(buttonSort, wxSizerFlags());
    buttonSizer->Add(buttonUpd,  wxSizerFlags());
    buttonSizer->Add(buttonEvlv, wxSizerFlags());
    buttonSizer->AddSpacer(20);
    buttonSizer->Add(buttonR1K,  wxSizerFlags());
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

    SetSizerAndFit(sizer);
}


void FrameWalker::onAbout(wxCommandEvent& /*event*/)
{
    wxString        msg;
    msg.Printf(wxT("Hello and Welcome to %s"), wxVERSION_STRING);
    wxMessageBox(msg, wxT("About Minimal"), wxOK | wxICON_INFORMATION, this);
}

void FrameWalker::onQuit(wxCommandEvent& /*event*/)
{
    Close();
}

void FrameWalker::onSave(wxCommandEvent& /*event*/)
{
    std::ofstream   save("save.walker");
    for (auto const& walker: walkers)
    {
        save << walker;
    }
}

void FrameWalker::onRun1(wxCommandEvent& /*event*/)
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

void FrameWalker::onSort(wxCommandEvent& /*event*/)
{
    panelWalker->shuffle();
}

void FrameWalker::onEvolve(wxCommandEvent& /*event*/)
{
    panelWalker->evolve();
}

void FrameWalker::onUpdate(wxCommandEvent& /*event*/)
{
    distanceGraph.tick();
    speciesGraph.tick();
    distanceHotMap.tick();
}

void FrameWalker::onRun1K(wxCommandEvent& /*event*/)
{
    std::default_random_engine&     generator   = ThorsUtil::Random::getRandomGenerator();
    std::uniform_real_distribution  random;

    wxProgressDialog dialog(wxT("Running 1K"), wxT("Progress: "), 100, this, wxPD_APP_MODAL);

    for(int loop = 0; loop < 100; ++loop)
    {
        for (auto& walker: walkers)
        {
            walker.run();
        }

        // ------

        std::stable_sort(std::begin(walkers), std::end(walkers), [](Walker const& lhs, Walker const& rhs){return lhs.score() > rhs.score();});

        // ------

        distanceGraph.tick(true);
        speciesGraph.tick(true);
        distanceHotMap.tick(true);
        if (panelWalker)
        {
            panelWalker->Refresh();
        }

        // ------

        int evolveParent = 0;
        for (std::size_t wakLoop = 0;wakLoop < walkers.size(); ++wakLoop)
        {
            // We want to kill approx 50% of the walkers.
            // The better scoring Walkers will have a higher chance of surviving.
            // The worse scoring Walkers will have a lower chance of surviving.
            if (random(generator) < (wakLoop * 1.0 / walkers.size()) || walkers[wakLoop].wasKilled())
            {
                walkers[wakLoop].kill();
                walkers[wakLoop].spawn(walkers[evolveParent++]);
            }
            // Even if we don't die there is an additional 25% chance of
            // of a random mutation.
            else if (wakLoop != 0 && random(generator) < 0.25)
            {
                walkers[wakLoop].mutate();
            }
        }

        // ------

        dialog.Update(loop);

        // ------
    }
}
