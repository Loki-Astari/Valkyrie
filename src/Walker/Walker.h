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
        Clock()         : time(0)   {}
        int tick()                  {return ++time;}
        void reset()                {time = 0;}
};

class Node
{
    Pos         startState;
    Pos         position;
    int         mass;
    public:
        Node();

        void mutateMass();

        Pos const&  getPos()    const       {return position;}
        int         getMass()   const       {return mass;}
        void updatePos(Force const& f);
        void setPos(Pos const& p)           {position = p;}

        void reset()                        {position = startState;}

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

        void mutateExtendedLen();
        void mutateContractLen();
        void mutateExtendedTime();
        void mutateContractTime();

        float getLen() const        {return currentSize;}
        void tick(int tick);

        void reset()                {currentSize = startState;currentTick = 0;}

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
    bool                    invalidScore;
    public:
        Walker(std::istream& stream)        {stream >> (*this);}
        Walker();

        void        run();
        int         score() const       {return invalidScore ? 0 : currentScore;}
        int         tick();
        void        kill();
        bool        wasKilled() const   {return nodes.size() < 2 || muscles.size() < 1;}
        void        mutate();
        void        spawn(Walker const& parent);
        std::string species() const;

        virtual wxSize  getSize()                           const   override    {return {1000, 1000};}
        virtual void    animationStepDo(wxDC&, int)                 override    {tick();}
        virtual int     animationMaxStep()                  const   override    {return 100;}
        virtual void    drawAnimation(wxDC& dc, int step)   const   override;
    private:
        void normalize();

        bool addRandomMuscle(int tryCount = 1, int srcNode = -1);
        void removeMuscle(int muscleId);
        void addRandomNode(int nodeCount);
        void removeNode(int nodeId);
        void addNodeConnections(int maxMuscles, int src = -1);
        void addRandomNodeWithConnection();

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
