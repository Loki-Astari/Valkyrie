#ifndef THORSANVIL_VALKYRIE_WALKER_H
#define THORSANVIL_VALKYRIE_WALKER_H

#include "ValkyrieWalkerConfig.h"
#include "Walker.h"

#include <ThorsUI/Animateable.h>
#include <wx/wx.h>
#include <wx/cmdline.h>
#include <memory>


namespace ThorsAnvil
{
    namespace ValkyrieWalker
    {

class WalkerButton: public ThorsAnvil::ThorsUI::Drawable
{
    Walker&     walker;
    float       scale;
    public:
        WalkerButton(Walker& w, float scale)
            : walker(w)
            , scale(scale)
        {}
        virtual void    draw(wxDC& dc)  const override
        {
            wxSize size = getSize();
            dc.SetLogicalOrigin(size.x / 2, 0);
            dc.SetLogicalScale(scale, scale);
            walker.draw(dc);
        }
        virtual wxSize  getSize()       const override
        {
            wxSize  result  = walker.getSize();
            result.x *= scale;
            result.y *= scale;
            return result;
        }
};

class ValkyrieWalkerFrame: public wxFrame
{
    std::vector<Walker>&        walkers;
    std::vector<WalkerButton>   buttons;

    public:
        ValkyrieWalkerFrame(std::vector<Walker>& walkers);

    private:
        void onQuit(wxCommandEvent& event);
        void onAbout(wxCommandEvent& event);

        DECLARE_EVENT_TABLE()
};

class ValkyrieWalkerApp: public wxApp
{
    static const wxCmdLineEntryDesc cmdLineDesc[];
    std::vector<Walker> walkers;

    public:
#pragma vera-pushoff
        virtual bool OnInit() override;
#pragma vera-pop
    private:
        bool parseCommandLine();
};

    }
}

#endif
