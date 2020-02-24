#include "Walker.h"
#include <cmath>

using namespace ThorsAnvil::ValkyrieWalker;

Clock::Clock()
    : time(0)
{}

int Clock::tick()
{
    return ++time;
}

Node::Node()
{
    std::default_random_engine&             generator   = ThorsUtil::Random::getRandomGenerator();
    std::uniform_int_distribution<int>      positionDist(0, 1000);
    std::uniform_int_distribution<int>      massDist(1, 10);

    position    = {positionDist(generator), positionDist(generator)};
    mass        = massDist(generator);
}

Pos const& Node::getPos() const
{
    return position;
}

int Node::getMass() const
{
    return mass;
}

void Node::updatePos(Force const& force)
{
    position.first  += std::trunc(force.first);
    position.second += std::trunc(force.second);
}

Muscle::Muscle()
    : currentTick(0)
{
    std::default_random_engine&             generator   = ThorsUtil::Random::getRandomGenerator();
    std::uniform_int_distribution<int>      timerDist(1, 10);
    std::uniform_real_distribution<float>   lenDist(0.1, 1.0);
    std::uniform_real_distribution<float>   strenDist(0.1, 1.0);

    extendedLen     = lenDist(generator);
    contractLen     = lenDist(generator);
    strength        = strenDist(generator);
    extendedTime    = timerDist(generator);
    contractTime    = timerDist(generator);

    currentSize = contractLen;
}

void Muscle::tick(int tick)
{
    currentTick = tick % (extendedTime + contractTime);

    int     timeLeft = (currentTick < extendedTime) ? extendedTime - currentTick : contractLen - (currentTick - extendedTime);
    float   distance = (contractLen - currentSize) / timeLeft;

    currentSize += distance;
}

float Muscle::getLen() const
{
    return currentSize;
}

Walker::Walker()
{
    std::default_random_engine&         generator   = ThorsUtil::Random::getRandomGenerator();

    // Add the base node (at the origin.
    nodes.emplace_back();

    // Add a set of random nodes that are randomly connected to one other node.
    std::uniform_int_distribution<int>  extraNodes(1,7);
    int nodeCount = extraNodes(generator);
    for (int loop = 0; loop < nodeCount; ++loop)
    {
        auto    prevNode = ThorsUtil::getRandomContainerIterator(nodes, generator);
        int     srcIndex = std::distance(std::begin(nodes), prevNode);
        int     dstIndex = nodes.size();
        int     musIndex = muscles.size();

        Muscle  newMuscle;
        Node    newNode;

        nodes.emplace_back();
        muscles.emplace_back();
        connections[musIndex] = Con(srcIndex, dstIndex);
    }

    // If every node is connected to every other node
    // Then we would have "maxMuscles"
    int maxMuscles = nodes.size() * (nodes.size() - 1) / 2;

    // Must reduce this count by the number of muscles we have
    // already assigned.
    maxMuscles -= (nodes.size() - 1);

    // Generate a set of new muscles.
    std::uniform_int_distribution<int>  extraMuscles(0, maxMuscles);
    std::uniform_int_distribution<int>  nodeGesser(0, nodes.size() - 1);
    int muscleCount = extraMuscles(generator);
    for (int loop = 0; loop < muscleCount; ++loop)
    {
        int srcNode = nodeGesser(generator);
        int dstNode;
        do
        {
            dstNode = nodeGesser(generator);
        }
        while (dstNode == srcNode);

        Con bestCon = std::minmax(srcNode, dstNode);
        auto find = std::find_if(std::begin(connections), std::end(connections), [&bestCon](auto const& value){return value.second == bestCon;});
        if (find != std::end(connections))
        {
            // don't allow new muscles between nodes that already have a connection.
            continue;
        }

        // Add a new muscle and normalize the distance between nodes.
        int musIndex = muscles.size();
        muscles.emplace_back();
        connections[musIndex] = bestCon;
        //normalize();
    }
}

void Walker::tick()
{
    int nextTick = clock.tick();
    for (auto m: muscles)
    {
        m.tick(nextTick);
    }
    normalize();
}

void Walker::normalize(int maxRep)
{
    // Try and minimize the stress on each muscle.
    // This is done by reducing the current size of the muscle (which is the distance between
    // its two nodes) and the desired length of the muscle (retrieved by getLen()).

    float previosTotalStress = 0;
    float currentTotalStress = 1000;
    /*
     * Repeat for maxRep iterations or until the change in stress between iterations is 0
     */
    for (int repeat = 0; repeat < maxRep && std::abs(previosTotalStress - currentTotalStress) > 0; ++repeat)
    {
        previosTotalStress  = currentTotalStress;
        currentTotalStress  = 0;
        std::vector<Force>   forces(nodes.size(), {0,0});;
        for (auto const& con: connections)
        {
            Pos     srcPos      = nodes[con.second.first].getPos();
            Pos     dstPos      = nodes[con.second.second].getPos();

            int     srcMass     = nodes[con.second.first].getMass();
            int     dstMass     = nodes[con.second.second].getMass();
            float   srcInertia  = 1.0 * srcMass / (srcMass + dstMass);
            float   dstInertia  = 1.0 * dstMass / (srcMass + dstMass);

            if (srcPos != dstPos)
            {
                int   xDist         = srcPos.first - dstPos.first;
                int   yDist         = srcPos.second - dstPos.second;
                float distSqr       = (xDist * xDist) + (yDist * yDist);
                float dist          = std::sqrt(distSqr);

                float wantedDist    = muscles[con.first].getLen() * 1000;

                // Negative force => away from each other.
                // Positive force => towards each other.
                float forceMag      = dist - wantedDist;
                bool  towards       = forceMag > 0;
                float force         = std::abs(forceMag);
                float alpha         = 1;
                float xDir          = ((srcPos.first < dstPos.first) == towards)   ? 1.0 : -1.0;
                float yDir          = ((srcPos.second < dstPos.second) == towards) ? 1.0 : -1.0;
                float xForce        = alpha * xDir * force * std::abs(xDist) / dist;
                float yForce        = alpha * yDir * force * std::abs(yDist) / dist;

                forces[con.second.first].first      += dstInertia * xForce;
                forces[con.second.second].first     -= srcInertia * xForce;

                forces[con.second.first].second     += dstInertia * yForce;
                forces[con.second.second].second    -= srcInertia * yForce;

                currentTotalStress           += force;
            }
        }
        for (std::size_t loop = 0; loop < nodes.size(); ++loop)
        {
            nodes[loop].updatePos(forces[loop]);
        }
    }
}

void Walker::applyGravity()
{
}

wxPoint toScreen(Pos const& p, int xOffset = 0, int yOffset = 0)
{
    // Y cordinate is from bottom up (Y 0 is 20 pixels off the bottom)
    // X cordinate 0 is the middle of the screen.
    return wxPoint{750 + xOffset + p.first, 1000 - 20 - yOffset - p.second};
}

void Walker::drawAnimation(wxDC& dc, int /*step*/) const
{
    for (auto const& con: connections)
    {
        dc.DrawLine(toScreen(nodes[con.second.first].getPos()), toScreen(nodes[con.second.second].getPos()));
    }
    for (std::size_t loop = 0; loop < nodes.size(); ++loop)
    {
        dc.DrawCircle(toScreen(nodes[loop].getPos()), 10);
        dc.DrawText(std::to_string(loop).c_str(), toScreen(nodes[loop].getPos(), -3, +7));
    }
}

void Walker::animationStepDo(wxDC& /*dc*/, int /*step*/)
{
    normalize();
    applyGravity();
}

int Walker::animationMaxStep() const
{
    return 100;
}

wxSize Walker::getSize() const
{
    return {1500, 1000};
}
