#include "UIDrawableDistanceGraph.h"
#include "Runner.h"
#include <set>

using namespace ThorsAnvil::ValkyrieRunner;

UIDrawableDistanceGraph::UIDrawableDistanceGraph(std::vector<Runner> const& runners)
    : runners(runners)
{
    addPoint("100%", 0);
    addPoint("80%", 0);
    addPoint("60%", 0);
    addPoint("40%", 0);
    addPoint("20%", 0);
    addPoint("0%", 0);


    addPoint("98%", 0);
    addPoint("96%", 0);
    addPoint("94%", 0);
    addPoint("92%", 0);
    addPoint("90%", 0);
    addPoint("10%", 0);
    addPoint("8%", 0);
    addPoint("6%", 0);
    addPoint("4%", 0);
    addPoint("2%", 0);

    setState("98%", false);
    setState("96%", false);
    setState("94%", false);
    setState("92%", false);
    setState("90%", false);
    setState("10%", false);
    setState("8%", false);
    setState("6%", false);
    setState("4%", false);
    setState("2%", false);
}

void UIDrawableDistanceGraph::tick()
{
    int oneP        = runners.size() / 100;

    int runners100  = 0;
    int runners80   = oneP * (100 - 80);
    int runners60   = oneP * (100 - 60);
    int runners40   = oneP * (100 - 40);
    int runners20   = oneP * (100 - 20);
    int runners00   = runners.size() - 1;
    addPoint("100%", runners[runners100].score());
    addPoint("80%",  runners[runners80].score());
    addPoint("60%",  runners[runners60].score());
    addPoint("40%",  runners[runners40].score());
    addPoint("20%",  runners[runners20].score());
    addPoint("0%",   runners[runners00].score());

    int runners98   = oneP * (100 - 98);
    int runners96   = oneP * (100 - 96);
    int runners94   = oneP * (100 - 94);
    int runners92   = oneP * (100 - 92);
    int runners90   = oneP * (100 - 90);
    int runners10   = oneP * (100 - 10);
    int runners08   = oneP * (100 - 8);
    int runners06   = oneP * (100 - 6);
    int runners04   = oneP * (100 - 4);
    int runners02   = oneP * (100 - 2);
    addPoint("98%",  runners[runners98].score());
    addPoint("96%",  runners[runners96].score());
    addPoint("94%",  runners[runners94].score());
    addPoint("92%",  runners[runners92].score());
    addPoint("90%",  runners[runners90].score());
    addPoint("10%",  runners[runners10].score());
    addPoint("8%",  runners[runners08].score());
    addPoint("6%",  runners[runners06].score());
    addPoint("4%",  runners[runners04].score());
    addPoint("2%",  runners[runners02].score());

    refresh();
}

void UIDrawableDistanceGraph::load(std::istream& /*stream*/)
{
}

void UIDrawableDistanceGraph::save(std::ostream& /*stream*/) const
{
}
