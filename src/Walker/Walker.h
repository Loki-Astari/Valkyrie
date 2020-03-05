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
        void reset();
};

class Node
{
    Pos         startState;
    Pos         position;
    int         mass;
    public:
        Node();

        Pos const&  getPos()    const;
        int         getMass()   const;
        void updatePos(Force const& f);
        void setPos(Pos const& p);

        void reset();

        void load(std::istream& stream);
        void save(std::ostream& stream) const;

        friend std::ostream& operator<<(std::ostream& stream, Node const& n)    {n.save(stream);return stream;}
        friend std::istream& operator>>(std::istream& stream, Node& n)          {n.load(stream);return stream;}
};

class Muscle
{
    // Lengths are in meters.
    float       extendedLen;
    float       contractLen;
    float       strength;
    int         extendedTime;
    int         contractTime;

    float       startState;
    float       currentSize;
    int         currentTick;

    public:
        Muscle();
        float getLen() const;
        void tick(int tick);

        void reset();

        void load(std::istream& stream);
        void save(std::ostream& stream) const;

        friend std::ostream& operator<<(std::ostream& stream, Muscle const& m)  {m.save(stream);return stream;}
        friend std::istream& operator>>(std::istream& stream, Muscle& m)        {m.load(stream);return stream;}
};

class Walker: public ThorsUI::Animateable
{
    public:
        using Con = std::pair<int, int>;
    private:
        using CM  = ThorsUtil::Pos;

    Clock                   clock;
    std::vector<Node>       nodes;
    std::vector<Muscle>     muscles;
    std::map<int, Con>      connections;
    int                     currentScore;
    public:
        Walker();
        Walker(std::istream& stream);
        void run();
        int  score() const {return currentScore;}
        int  tick();
        void kill();
        void mutate();
        void spawn(Walker const& parent);

        virtual wxSize  getSize()                           const   override;
        virtual void    drawAnimation(wxDC& dc, int step)   const   override;
        // virtual std::unique_ptr<wxDC>   animationDC()               override;
        // virtual void    animateResetDo(wxDC& dc)                    override;
        virtual void    animationStepDo(wxDC& dc, int step)         override;
        virtual int     animationMaxStep()                  const   override;
    private:
        void normalize();

        void reset();

        void minimizeStress(int maxRep = 100);
        float applyGravity();
        void dropAndFindRestingPoint();

        // Gravity Functions
        using Bound = std::tuple<Node const&, Node const&>;

        template<typename F>
        float       findSmallestAngleFrom(F const& test, int point) const;
        Node const& findLowestNode()                                const;
        CM          calculateCenterOfGravity()                      const;
        Bound       getBaseOfObject(Node const& lowestNode)         const;
        Node const& dropToGround();
        float       rotateAroundLowestPoint(Node const& lowest, float maxTurn);
        void        rotateNodesAround(Node const& node, float alpha);
        void        translateNodes(Pos const& relative);

        void load(std::istream& stream);
        void save(std::ostream& stream) const;

        friend std::istream& operator>>(std::istream& stream, Walker& data)         {data.load(stream);return stream;}
        friend std::ostream& operator<<(std::ostream& stream, Walker const& data)   {data.save(stream);return stream;}
};

    }
}

#endif
