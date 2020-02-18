#include "MazeDetail.h"
#include "Maze.h"
#include <ThorsUtil/Random.h>
#include <set>
#include <sstream>

using namespace ThorsAnvil::ValkyrieMaze::MazeDetail;

// class MazeGenerator

MazeGenerator::MazeGenerator(int width, int height)
    : width(width)
    , height(height)
{}

void MazeGenerator::operator()(Maze& maze) const
{
    std::default_random_engine& generator = ThorsUtil::Random::getRandomGenerator();

    ThorsUtil::Pos      entrPoint{ThorsUtil::getDist(width)(generator), ThorsUtil::getDist(height)(generator)};
    ThorsUtil::Pos      exitPoint{ThorsUtil::getDist(width)(generator), ThorsUtil::getDist(height)(generator)};
    WallGrid            result(width, height);

    using WallSet   = std::set<WallInfo>;
    using WallIter  = WallSet::iterator;

    std::set<ThorsUtil::Pos>    alreadyUsed;
    WallSet                     wallsFound;

    alreadyUsed.insert(exitPoint);

    wallsFound.emplace(exitPoint.first, exitPoint.second, North);
    wallsFound.emplace(exitPoint.first, exitPoint.second, South);
    wallsFound.emplace(exitPoint.first, exitPoint.second, East);
    wallsFound.emplace(exitPoint.first, exitPoint.second, West);

    while (!wallsFound.empty())
    {
        WallIter        next = ThorsUtil::getRandomContainerIterator(wallsFound, generator);
        WallInfo        wall = *next;
        wallsFound.erase(next);

        ThorsUtil::Pos    src{wall.x, wall.y};
        ThorsUtil::Pos    dst{wall.x, wall.y};
        switch (wall.d)
        {
            case North: --dst.second;break;
            case South: ++dst.second;break;
            case East:  ++dst.first;break;
            case West:  --dst.first;break;
        }
        if (dst.second < 0 || dst.second >= height || dst.first < 0 || dst.first >= width)
        {
            continue;
        }

        auto findSrc = alreadyUsed.find(src);
        if (findSrc == alreadyUsed.end())
        {
            using std::swap;
            swap(src, dst);
        }
        else
        {
            auto findDst = alreadyUsed.find(dst);
            if (findDst != alreadyUsed.end())
            {
                continue;
            }
        }

        result[wall] = false;
        alreadyUsed.insert(dst);

        wallsFound.emplace(dst.first, dst.second, North);
        wallsFound.emplace(dst.first, dst.second, South);
        wallsFound.emplace(dst.first, dst.second, East);
        wallsFound.emplace(dst.first, dst.second, West);
    }

    maze.walls      = result;
    maze.exitPoint  = exitPoint;
    maze.entrPoint  = entrPoint;
}

// class WallGrid

WallGrid::WallGrid()
{}

WallGrid::WallGrid(int width, int height)
    : horzWalls(width, std::vector<bool>(height + 1, true))
    , vertWalls(width + 1, std::vector<bool>(height, true))
{}

void WallGrid::swap(WallGrid& other) noexcept
{
    using std::swap;
    swap(horzWalls, other.horzWalls);
    swap(vertWalls, other.vertWalls);
}

void WallGrid::clear()
{
    horzWalls.clear();
    vertWalls.clear();
}

ThorsAnvil::ThorsUtil::Size WallGrid::size() const
{
    return {horzWalls.size(), vertWalls[0].size()};
}

bool WallGrid::operator[](WallInfo const& info) const
{
    switch (info.d)
    {
        case North: return horzWalls[info.x][info.y];
        case South: return horzWalls[info.x][info.y+1];
        case East:  return vertWalls[info.x+1][info.y];
        case West:  return vertWalls[info.x][info.y];
    }
}

WallGrid::Ref WallGrid::operator[](WallInfo const& info)
{
    switch (info.d)
    {
        case North: return horzWalls[info.x][info.y];
        case South: return horzWalls[info.x][info.y+1];
        case East:  return vertWalls[info.x+1][info.y];
        case West:  return vertWalls[info.x][info.y];
    }
}

void WallGrid::draw(wxDC& dc, wxPoint const& offset, wxPoint const& cellSize) const
{
    // Draw the Maze.
    dc.SetPen(*wxBLACK_PEN);

    ThorsUtil::Size s = size();

    for (int loopX = 0; loopX < s.first; ++loopX)
    {
        for (int loopY = 0; loopY < s.second; ++loopY)
        {
            wxPoint src{offset.x + loopX * cellSize.x, offset.y + loopY * cellSize.y};
            if (horzWalls[loopX][loopY])
            {
                wxPoint dst{src.x + cellSize.x, src.y};
                dc.DrawLine(src, dst);
            }
            if (vertWalls[loopX][loopY])
            {
                wxPoint dst{src.x, src.y + cellSize.y};
                dc.DrawLine(src, dst);
            }
        }
        wxPoint srcV{offset.x + s.first * cellSize.x, offset.y};
        wxPoint dstV{offset.x + s.first * cellSize.x, offset.y + s.second * cellSize.y};
        dc.DrawLine(srcV, dstV);
    }
    wxPoint srcH{offset.x, offset.y + s.second * cellSize.y};
    wxPoint dstH{offset.x + s.first * cellSize.x, offset.y + s.second * cellSize.y};
    dc.DrawLine(srcH, dstH);
}

void WallGrid::read(std::istream& stream)
{
    ThorsUtil::IgnoreUntilNewLine    ignoreNewLine;
    int hSize;
    int vSize;
    if (stream >> hSize >> vSize >> ignoreNewLine)
    {
        WallGrid    tmp;

        int loopH = 0;
        std::string line;
        while (loopH < hSize && std::getline(stream, line))
        {
            std::stringstream   lineStream(line);
            tmp.horzWalls.emplace_back(std::istream_iterator<Bool>(lineStream), std::istream_iterator<Bool>());
            ++loopH;
        }
        int loopV = 0;
        while (loopV < vSize && std::getline(stream, line))
        {
            std::stringstream   lineStream(line);
            tmp.vertWalls.emplace_back(std::istream_iterator<Bool>(lineStream), std::istream_iterator<Bool>());
            ++loopV;
        }
        if (hSize == loopH && vSize == loopV)
        {
            swap(tmp);
        }
        else
        {
            stream.setstate(std::ios::failbit);
        }
    }
}
void WallGrid::write(std::ostream& stream) const
{
    stream << horzWalls.size() << " " << vertWalls.size() << "\n";
    for (auto const& row: horzWalls)
    {
        std::copy(std::begin(row), std::end(row), std::ostream_iterator<bool>(stream));
        stream << "\n";
    }
    for (auto const& row: vertWalls)
    {
        std::copy(std::begin(row), std::end(row), std::ostream_iterator<bool>(stream));
        stream << "\n";
    }
}
