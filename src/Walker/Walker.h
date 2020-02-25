#ifndef THORSANVIL_VALKYRIE_WALKER_WALKER_H
#define THORSANVIL_VALKYRIE_WALKER_WALKER_H

#include <ThorsUI/Animateable.h>
#include <ThorsUtil/Random.h>
#include <ThorsUtil/Util.h>
#include <vector>
#include <map>

namespace ThorsAnvil
{
    namespace ValkyrieWalker
    {

// Position in MiliMeters
using ThorsAnvil::ThorsUtil::Pos;
using Force  = std::pair<float, float>;
using Center = std::pair<float, float>;

class Clock
{
    int time;
    public:
        Clock();
        int tick();
};

class Node
{
    Pos         position;
    int         mass;
    public:
        Node();

        Pos const&  getPos()    const;
        int         getMass()   const;
        void updatePos(Force const& f);
        void setPos(Pos const& p);
};

class Muscle
{
    // Lengths are in meters.
    float       extendedLen;
    float       contractLen;
    float       strength;
    int         extendedTime;
    int         contractTime;

    float       currentSize;
    int         currentTick;

    public:
        Muscle();
        float getLen() const;
        void tick(int tick);
};

class Walker: public ThorsUI::Animateable
{
    using Con = std::pair<int, int>;

    Clock                   clock;
    std::vector<Node>       nodes;
    std::vector<Muscle>     muscles;
    std::map<int, Con>      connections;
    public:
        Walker();
        void tick();

        virtual wxSize  getSize()                           const   override;
        virtual void    drawAnimation(wxDC& dc, int step)   const   override;
        // virtual std::unique_ptr<wxDC>   animationDC()               override;
        // virtual void    animateResetDo(wxDC& dc)                    override;
        virtual void    animationStepDo(wxDC& dc, int step)         override;
        virtual int     animationMaxStep()                  const   override;
    private:
        void normalize(int maxRep = 100);
        void applyGravity();
        void dropAndFindRestingPoint();

        // Gravity Functions
        using Bound = std::tuple<Node const&, Node const&>;

        template<typename F>
        float       findSmallestAngleFrom(F const& test, int point) const;
        Node const& findLowestNode()                                const;
        float       calculateCenterOfGravity()                      const;
        Bound       getBaseOfObject(Node const& lowestNode)         const;
        Node const& dropToGround();
        float       rotateAroundLowestPoint(Node const& lowest, float maxTurn);
        void        rotateNodesAround(Node const& node, float alpha);
        void        translateNodes(Pos const& relative);


};

    }
}

#endif
