#ifndef THORSANVIL_VALKYRIE_WALKER_H
#define THORSANVIL_VALKYRIE_WALKER_H

#include "ValkyrieWalkerConfig.h"

#include <ThorsUtil/BackgroundWorker.h>
#include <ThorsUI/Drawable.h>
#include <wx/wx.h>
#include <wx/cmdline.h>
#include <memory>


namespace ThorsAnvil
{
    namespace ValkyrieWalker
    {

class ValkyrieWalkerInfo: public ThorsUI::Drawable
{
    public:
        ValkyrieWalkerInfo();

        virtual void        draw(wxDC& dc)  const;
        virtual wxSize      getSize()       const;

    private:
};

class ValkyrieWalkerFrame: public wxFrame
{
    ValkyrieWalkerInfo&                     valkyrieWalker;
    ThorsUtil::BackgroundWorker             worker;

    public:
        ValkyrieWalkerFrame(ValkyrieWalkerInfo& valkyrieWalker);

    private:
        void onQuit(wxCommandEvent& event);
        void onAbout(wxCommandEvent& event);

        DECLARE_EVENT_TABLE()
};

class ValkyrieWalkerApp: public wxApp
{
    static const wxCmdLineEntryDesc cmdLineDesc[];

    std::unique_ptr<ValkyrieWalkerInfo>    valkyrieWalker;

    public:
        virtual bool OnInit() override;
};

    }
}

#endif
