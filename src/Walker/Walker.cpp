#include "Walker.h"
#include <cmath>
#include <sstream>
#include <iterator>
#include <string>


using namespace ThorsAnvil::ValkyrieWalker;

Node::Node()
{
    std::default_random_engine&             generator   = ThorsUtil::Random::getRandomGenerator();
    std::uniform_int_distribution<int>      positionDist(0, 1000);

    position    = {positionDist(generator), positionDist(generator)};
    startState  = position;

    mutateMass();
}

void Node::mutateMass()
{
    std::default_random_engine&             generator   = ThorsUtil::Random::getRandomGenerator();
    std::uniform_int_distribution<int>      massDist(1, 10);
    mass        = massDist(generator);
}

void Node::updatePos(Force const& force)
{
    position.first  += std::trunc(force.first);
    position.second += std::trunc(force.second);
}

void Node::load(std::istream& stream)
{
    stream  >> startState.first
            >> startState.second
            >> mass;
}

void Node::save(std::ostream& stream) const
{
    stream  << startState.first  << " "
            << startState.second << " "
            << mass              << " ";
}

Muscle::Muscle()
    : currentTick(0)
{
    std::default_random_engine&             generator   = ThorsUtil::Random::getRandomGenerator();
    std::uniform_int_distribution<int>      timerDist(1, 10);
    std::uniform_real_distribution<float>   lenDist(0.1, 1.0);
    std::uniform_real_distribution<float>   strenDist(0.1, 1.0);

    std::tie(contractLen, extendedLen) = std::minmax(lenDist(generator), lenDist(generator));
    strength        = strenDist(generator);
    mutateExtendedTime();
    mutateContractTime();

    currentSize = contractLen;
    startState  = currentSize;
}

void Muscle::mutateExtendedLen()
{
    std::default_random_engine&             generator   = ThorsUtil::Random::getRandomGenerator();
    std::uniform_real_distribution<float>   lenDist(0.1, 1.0);

    float tmpContractLen      = contractLen;
    float tmpExtendedLen      = lenDist(generator);
    std::tie(contractLen, extendedLen) = std::minmax(tmpContractLen, tmpExtendedLen);
}

void Muscle::mutateContractLen()
{
    std::default_random_engine&             generator   = ThorsUtil::Random::getRandomGenerator();
    std::uniform_real_distribution<float>   lenDist(0.1, 1.0);

    float tmpContractLen      = lenDist(generator);
    float tmpExtendedLen      = extendedLen;
    std::tie(contractLen, extendedLen) = std::minmax(tmpContractLen, tmpExtendedLen);
}

void Muscle::mutateExtendedTime()
{
    std::default_random_engine&             generator   = ThorsUtil::Random::getRandomGenerator();
    std::uniform_int_distribution<int>      timerDist(1, 10);

    extendedTime    = timerDist(generator) * 10;
}

void Muscle::mutateContractTime()
{
    std::default_random_engine&             generator   = ThorsUtil::Random::getRandomGenerator();
    std::uniform_int_distribution<int>      timerDist(1, 10);

    contractTime    = timerDist(generator) * 10;
}

void Muscle::tick(int tick)
{
    currentTick = tick % (extendedTime + contractTime);
    if (currentTick < extendedTime)
    {
        // Expand phase
        currentSize = contractLen + (1.0 * currentTick / extendedTime * (extendedLen - contractLen));
    }
    else
    {
        // Contract phase
        currentTick -= extendedTime;
        currentSize = extendedLen - (1.0 * currentTick / contractTime * (extendedLen - contractLen));
    }
}

void Muscle::load(std::istream& stream)
{
    stream  >> startState
            >> extendedLen
            >> contractLen
            >> strength
            >> extendedTime
            >> contractTime;
}

void Muscle::save(std::ostream& stream) const
{
    stream  << startState   << " "
            << extendedLen  << " "
            << contractLen  << " "
            << strength     << " "
            << extendedTime << " "
            << contractTime << " ";
}

Walker::Walker()
    : currentScore(0)
    , invalidScore(true)
{
    std::default_random_engine&         generator   = ThorsUtil::Random::getRandomGenerator();

    // Add the base node (at the origin.
    nodes.emplace_back();

    // Add a set of random nodes that are randomly connected to one other node.
    std::uniform_int_distribution<int>  extraNodes(1,7);
    int nodeCount = extraNodes(generator);

    addRandomNode(nodeCount);

    // If every node is connected to every other node
    // Then we would have "maxMuscles"
    int maxMuscles = nodes.size() * (nodes.size() - 1) / 2;

    // Must reduce this count by the number of muscles we have
    // already assigned.
    maxMuscles -= (muscles.size() - 1);
    addNodeConnections(maxMuscles);

    normalize();
}

void Walker::addRandomNode(int nodeCount)
{
    std::default_random_engine&         generator   = ThorsUtil::Random::getRandomGenerator();

    for (int loop = 0; loop < nodeCount; ++loop)
    {
        auto    prevNode = ThorsUtil::getRandomContainerIterator(nodes, generator);
        int     srcIndex = std::distance(std::begin(nodes), prevNode);
        int     dstIndex = nodes.size();
        int     musIndex = muscles.size();

        nodes.emplace_back();
        muscles.emplace_back();
        connections[musIndex] = Con(srcIndex, dstIndex);
    }
}

void Walker::addRandomNodeWithConnection()
{
    // When adding a muscle the most muscles we can add is
    // one muscle from each other node to this one.
    // So 'nodes.size()' is the maximum number of muscles we can add.
    //
    // Subtract one from this number because adding the node
    // automatically one muscle.
    int maxMusclesToAdd = nodes.size() - 1;
    int newNodeId       = nodes.size();

    addRandomNode(1);
    addNodeConnections(maxMusclesToAdd, newNodeId);
}

void Walker::addNodeConnections(int maxMuscles, int src)
{
    std::default_random_engine&         generator   = ThorsUtil::Random::getRandomGenerator();

    // Generate a set of new muscles.
    std::uniform_int_distribution<int>  extraMuscles(0, maxMuscles);
    int muscleCount = extraMuscles(generator);
    for (int loop = 0; loop < muscleCount; ++loop)
    {
        if (!addRandomMuscle(1, src))
        {
            continue;
        }
    }
}

bool Walker::addRandomMuscle(int tryCount, int srcNode)
{
    if (nodes.size() == 1)
    {
        return false;
    }
    std::default_random_engine&         generator   = ThorsUtil::Random::getRandomGenerator();
    std::uniform_int_distribution<int>  nodeGesser(0, nodes.size() - 1);

    if (srcNode == -1)
    {
        srcNode = nodeGesser(generator);
    }

    for (int loop = 0; loop < tryCount; ++loop)
    {
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

        int musIndex = muscles.size();
        muscles.emplace_back();
        connections[musIndex] = bestCon;
        return true;
    }
    return false;
}

void Walker::normalize()
{
    reset();
    minimizeStress();
    dropAndFindRestingPoint();
}

void Walker::run()
{
    if (invalidScore)
    {
        float score = 0;
        for (int loop = 0; loop < 3000; ++loop)
        {
            score = tick();
        }
        normalize();
        currentScore = score;
        invalidScore = false;
    }
}

int Walker::tick()
{
    int nextTick = clock.tick();
    for (auto& m: muscles)
    {
        m.tick(nextTick);
    }
    minimizeStress();
    return applyGravity();
}

void Walker::reset()
{
    clock.reset();
    for (auto& node: nodes)
    {
        node.reset();
    }
    for (auto& muscle: muscles)
    {
        muscle.reset();
    }
}

void Walker::minimizeStress(int maxRep)
{
    // Try and minimize the stress on each muscle.
    // This is done by reducing the current size of the muscle (which is the distance between
    // its two nodes) and the desired length of the muscle (retrieved by getLen()).

    float previosTotalStress = 0;
    float currentTotalStress = 1000;
    /*
     * Repeat for maxRep iterations or until the change in stress between iterations is 0
     */
    bool broken = false;
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
                if (dist > 5000)
                {
                    broken = true;
                    break;
                }

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
    if (broken)
    {
        kill();
    }
}

void Walker::kill()
{
    // Clear out all the state.
    nodes.clear();
    muscles.clear();
    connections.clear();

    // Add one node so it will be drawn but can't move
    nodes.emplace_back();
    normalize();
}

void Walker::removeMuscle(int muscleId)
{
    std::map<int, Con>      newConnections;
    for (auto const& item: connections)
    {
        if (item.first == muscleId)
        {
            continue;
        }
        int index = item.first;
        if (item.first > muscleId)
        {
            --index;
        }
        newConnections[index] = item.second;
    }
    muscles.erase(muscles.begin() + muscleId);
    connections.clear();
    connections = newConnections;
}

void Walker::removeNode(int nodeId)
{
    bool foundAMuscle = false;
    do
    {
        foundAMuscle = false;
        // Iterate over the connections looking for a muscle
        // That is connected to the node we are deleting.
        // Note. When we delete the muscle the iterators are
        // invalidated so we must start from the beginning
        // again.
        for (auto const& item: connections)
        {
            if (item.second.first == nodeId || item.second.second == nodeId)
            {
                removeMuscle(item.first);
                foundAMuscle = true;
                break;
            }
        }
    }
    while (foundAMuscle);

    // Remove the node.
    nodes.erase(std::begin(nodes) + nodeId);
    // Adjust all muscles to make sure they
    // are correct to the correct node.
    for (auto& item: connections)
    {
        if (item.second.first > nodeId)
        {
            --item.second.first;
        }
        if (item.second.second > nodeId)
        {
            --item.second.second;
        }
    }
}

void Walker::mutate()
{
    const int onePercent            = (nodes.size() + muscles.size() * 4);
    const int mutate                = onePercent * 94;
    // If we have very few muscles then deleting them is not an option.
    // So increase the chance to add Muscle.
    const int delMuscle             = onePercent * (muscles.size() < nodes.size() ? 0 : 1);
    const int addMuscle             = onePercent * (muscles.size() < nodes.size() ? 2 : 1);
    // If we have very few nodes then deleting them is not an option.
    // So increase the chance to add Nodes.
    const int delNode               = onePercent * (nodes.size() < 3 ? 0 : 1);
    const int addNode               = onePercent * (nodes.size() < 3 ? 2 : 1);

    // Note there is a chance that we have a chance there are zero muscles.
    // This happens when the node blows up due to stress between muscles.
    const int mutateNodePart        = mutate * (nodes.size() * 1.0 / onePercent);
    const int mutateMusclePart      = (mutate - mutateNodePart) / 4;

    // The max for each range.
    const int part1_MutateNode      = mutateNodePart;
    const int part2_DelNode         = part1_MutateNode + delNode;
    const int part3_AddNode         = part2_DelNode    + addNode;
    const int sectionA_Nodes        = part3_AddNode;

    // Muscles
    const int part4_ExtendedLen     = part3_AddNode      + mutateMusclePart;
    const int part5_ContractLen     = part4_ExtendedLen  + mutateMusclePart;
    const int part6_ExtendedTime    = part5_ContractLen  + mutateMusclePart;
    const int part7_ContractTime    = part6_ExtendedTime + mutateMusclePart;
    const int part8_DelMuscle       = part7_ContractTime + delMuscle;
    const int part9_AddMuscle       = part8_DelMuscle    + addMuscle;
    const int SectionB_Muscles      = part9_AddMuscle;
    const int allPosabilities       = SectionB_Muscles;

    std::default_random_engine&             generator   = ThorsUtil::Random::getRandomGenerator();
    std::uniform_int_distribution<int>      dist(0, allPosabilities - 1);
    int  random = dist(generator);

    if (random < sectionA_Nodes)
    {
        std::uniform_int_distribution<int>      node(0, nodes.size() - 1);
        int nodeId = node(generator);

        if (random < part1_MutateNode)          {nodes[nodeId].mutateMass();}
        else if (random < part2_DelNode)        {removeNode(nodeId);}
        else if (random < part3_AddNode)        {addRandomNodeWithConnection();}
        else                                    {/* ERROR */std::cerr << "Error: Node\n";}
    }
    else if (random < SectionB_Muscles)
    {
        std::uniform_int_distribution<int>      muscle(0, muscles.size() - 1);
        int muscleId = muscle(generator);

        if (random < part4_ExtendedLen)         {muscles[muscleId].mutateContractLen();}
        else if (random < part5_ContractLen)    {muscles[muscleId].mutateExtendedLen();}
        else if (random < part6_ExtendedTime)   {muscles[muscleId].mutateExtendedTime();}
        else if (random < part7_ContractTime)   {muscles[muscleId].mutateContractTime();}
        else if (random < part8_DelMuscle)      {removeMuscle(muscleId);}
        else if (random < part9_AddMuscle)      {addRandomMuscle(5);}
        else                                    {/* ERROR */std::cerr << "Error: Muscle\n";}
    }
    else                                        {/* ERROR */std::cerr << "Error: Unknown\n";}

    normalize();
    currentScore    = 0;
    invalidScore    = true;
}

void Walker::spawn(Walker const& parent)
{
    nodes       = parent.nodes;
    muscles     = parent.muscles;
    connections = parent.connections;

    mutate();
}

std::string Walker::species() const
{
#pragma vera-pushoff#
    using namespace std::string_literals;
#pragma vera-pop
    return "N"s + std::to_string(nodes.size()) + "M"s + std::to_string(muscles.size());
}

void Walker::dropAndFindRestingPoint()
{
    Node const&    lowestNode          = dropToGround();

    float   centerGravity       = rotateAroundLowestPoint(lowestNode, 2 * M_PI);
    translateNodes({-centerGravity, 0});
}

float Walker::applyGravity()
{
    Node const&    lowestNode          = dropToGround();
    return rotateAroundLowestPoint(lowestNode, 2 * M_PI * 15 / 360);
}

Node const& Walker::dropToGround()
{
    Node const&    lowestNode = findLowestNode();
    translateNodes({0, -lowestNode.getPos().second});

    return lowestNode;
}

float Walker::rotateAroundLowestPoint(Node const& lowestNode, float maxTurn)
{
    auto    base                = getBaseOfObject(lowestNode);
    int     minXPointOnGround   = std::get<0>(base).getPos().first;
    int     maxXPointOnGround   = std::get<1>(base).getPos().first;


    float   totalRotation = 0;
    float   centerGravity;
    bool    rotation;
    do
    {
        centerGravity       = calculateCenterOfGravity().first;
        rotation            = false;

        if (centerGravity < minXPointOnGround)
        {
            float alpha = findSmallestAngleFrom([](int pos, int point){return pos < point;}, minXPointOnGround);
            alpha = std::min(alpha, +1.0f * maxTurn);

            rotateNodesAround(std::get<0>(base), alpha);
            totalRotation += std::abs(alpha);
            rotation = alpha != 0;
        }
        if (centerGravity > maxXPointOnGround)
        {
            float alpha = findSmallestAngleFrom([](int pos, int point){return pos > point;}, maxXPointOnGround);
            alpha = std::max(alpha, -1.0f * maxTurn);

            rotateNodesAround(std::get<1>(base), alpha);
            totalRotation += std::abs(alpha);
            rotation = alpha != 0;
        }
    }
    while (totalRotation < std::abs(maxTurn) && rotation);
    return centerGravity;
}

Node const& Walker::findLowestNode() const
{
    int     lowestNode      = 0;
    int     minHeight       = nodes[lowestNode].getPos().second;
    for (std::size_t loop = 1; loop < nodes.size(); ++loop)
    {
        if (minHeight > nodes[loop].getPos().second)
        {
            minHeight = nodes[loop].getPos().second;
            lowestNode = loop;
        }
    }
    return nodes[lowestNode];
}

Walker::CM Walker::calculateCenterOfGravity() const
{
    int     totalMass       = 0;
    int     momentX         = 0;
    int     momentY         = 0;
    for (std::size_t loop = 0; loop < nodes.size(); ++loop)
    {
        int currentMass = nodes[loop].getMass();
        totalMass       += currentMass;
        momentX         += nodes[loop].getPos().first * currentMass;
        momentY         += nodes[loop].getPos().second * currentMass;
    }
    CM   centerGravity   {1.0 * momentX / totalMass, 1.0 * momentY / totalMass};

    return centerGravity;
}

void Walker::rotateNodesAround(Node const& cent, float alpha)
{
    Pos const& centerOfRotation = cent.getPos();

    for (auto& node: nodes)
    {
        Pos pos = node.getPos();
        pos.first  -= centerOfRotation.first;
        pos.second -= centerOfRotation.second;

        float s = sin(alpha);
        float c = cos(alpha);

        float xNew = (pos.first * c - pos.second * s) + centerOfRotation.first;
        float yNew = (pos.first * s + pos.second * c) + centerOfRotation.second;

        node.setPos({xNew, yNew});
    }
}

void Walker::translateNodes(Pos const& relative)
{
    for (std::size_t loop = 0; loop < nodes.size(); ++loop)
    {
        nodes[loop].updatePos(relative);
    }
}

Walker::Bound Walker::getBaseOfObject(Node const& lowestNode) const
{
    int minXNodeOnGround    = -1;
    int maxXNodeOnGround    = -1;
    int minXPointOnGround   = lowestNode.getPos().first;
    int maxXPointOnGround   = lowestNode.getPos().first;

    for (std::size_t loop = 0; loop < nodes.size(); ++loop)
    {
        auto const& pos = nodes[loop].getPos();

        // Check to see if there are other points on the ground.
        // Keep note of the left and right-most points.
        if (pos.second == 0)
        {
            if (pos.first <= minXPointOnGround)
            {
                minXPointOnGround   = pos.first;
                minXNodeOnGround    = loop;
            }
            if (maxXPointOnGround <= pos.first)
            {
                maxXPointOnGround   = pos.first;
                maxXNodeOnGround    = loop;
            }
        }
    }
    if (minXNodeOnGround == -1 || maxXNodeOnGround == -1)
    {
        std::cerr << "Failed in getBaseOfObject\n";
        //std::cerr << (*this) << "\n";
        throw std::runtime_error("Bad Node\n");
    }

    return std::tie(nodes[minXNodeOnGround], nodes[maxXNodeOnGround]);
}

template<typename F>
float Walker::findSmallestAngleFrom(F const& test, int point) const
{
    int     nodeWithSmallestAngle    = -1;
    float   smallestAngle;
    float   smallestAngleAbsolute;
    for (std::size_t loop = 0; loop < nodes.size(); ++loop)
    {
        auto const& pos = nodes[loop].getPos();
        float thisAngle = 1.0 * pos.second / (point - pos.first);
        float thisAngleAbsolute = std::abs(thisAngle);
        if (test(pos.first, point) && (nodeWithSmallestAngle == -1 || thisAngleAbsolute < smallestAngleAbsolute))
        {
            nodeWithSmallestAngle   = loop;
            smallestAngle           = thisAngle;
            smallestAngleAbsolute   = thisAngleAbsolute;
        }
    }

    float alpha = std::atan(smallestAngle);
    return alpha;
}

wxPoint toScreen(Pos const& p, int xOffset = 0, int yOffset = 0)
{
    // Y cordinate is from bottom up (Y 0 is 20 pixels off the bottom)
    // X cordinate 0 is the middle of the screen.
    return wxPoint{xOffset + p.first, 1000 - yOffset - p.second};
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
    dc.SetBrush(*wxRED_BRUSH);
    dc.DrawCircle(toScreen(calculateCenterOfGravity()), 30);
}

namespace ThorsAnvil::ValkyrieWalker
{
    using InValue = std::pair<int, std::pair<int, int>>;

    std::istream& operator>>(std::istream& stream, InValue& data)
    {
        return stream >> data.first >> data.second.first >> data.second.second;
    }
    std::ostream& operator<<(std::ostream& stream, InValue const& data)
    {
        return stream << data.first << " " <<  data.second.first << " " <<  data.second.second << " ";
    }
}

void Walker::load(std::istream& stream)
{
    std::vector<Node>       tmpNode;
    std::vector<Muscle>     tmpMuscle;
    std::map<int, Con>      tmpConection;
    int count = 0;

    std::string line;
    if (std::getline(stream, line))
    {
        std::stringstream   lineStream(line);
        tmpNode.insert(tmpNode.end(), std::istream_iterator<Node>(lineStream), std::istream_iterator<Node>());
        ++count;
    }
    if (std::getline(stream, line))
    {
        std::stringstream   lineStream(line);
        tmpMuscle.insert(tmpMuscle.end(), std::istream_iterator<Muscle>(lineStream), std::istream_iterator<Muscle>());
        ++count;
    }
    if (std::getline(stream, line))
    {
        std::stringstream   lineStream(line);
        InValue             tmp;
        while (lineStream >> tmp)
        {
            tmpConection[tmp.first] = tmp.second;
        }
        ++count;
    }
    if (count == 3)
    {
        using std::swap;
        swap(nodes,      tmpNode);
        swap(muscles,    tmpMuscle);
        swap(connections,tmpConection);

        normalize();
        currentScore    = 0;
        invalidScore    = true;
    }
}

void Walker::save(std::ostream& stream) const
{
    std::copy(std::begin(nodes), std::end(nodes), std::ostream_iterator<Node>(stream));
    stream << "\n";

    std::copy(std::begin(muscles), std::end(muscles), std::ostream_iterator<Muscle>(stream));
    stream << "\n";

    for (auto const& connect: connections)
    {
        stream << connect;
    }
    stream << "\n";
}
