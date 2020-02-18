#include "Maze.h"

using namespace ThorsAnvil::ValkyrieMaze;

Maze::Maze()
    : walls(2,2)
    , exitPoint(0, 0)
    , entrPoint(1, 1)
{}

Maze::Maze(std::istream&& stream)
    : Maze()
{
    stream >> (*this);
}

void Maze::swap(Maze& other) noexcept
{
    using std::swap;
    swap(walls,     other.walls);
    swap(exitPoint, other.exitPoint);
    swap(entrPoint, other.entrPoint);
}

void Maze::read(std::istream& stream)
{
    ThorsUtil::IgnoreUntilNewLine  ignoreNewLine;

    Maze tmp;
    tmp.walls.clear();
    if (stream >> tmp.exitPoint.first >> tmp.exitPoint.second >> ignoreNewLine
               >> tmp.entrPoint.first >> tmp.entrPoint.second >> ignoreNewLine
               >> tmp.walls)
    {
        ThorsUtil::Size size = tmp.walls.size();
        if (tmp.exitPoint.first < size.first && tmp.exitPoint.second < size.second
         && tmp.entrPoint.first < size.first && tmp.entrPoint.second < size.second)
        {
            swap(tmp);
        }
        else
        {
            stream.setstate(std::ios::failbit);
        }
    }
}

void Maze::write(std::ostream& stream) const
{
    stream << exitPoint.first << " " << exitPoint.second << "\n"
           << entrPoint.first << " " << entrPoint.second << "\n"
           << walls;
}

ThorsAnvil::ThorsUtil::Size Maze::size() const
{
    return walls.size();
}

ThorsAnvil::ThorsUtil::Pos Maze::start() const
{
    return entrPoint;
}

ThorsAnvil::ThorsUtil::Pos Maze::end() const
{
    return exitPoint;
}

int Maze::distanceDiagonal(ThorsUtil::Pos pos, int& exitDir, int exitValue, ThorsUtil::Size size, MD::Direction yd, MD::Direction xd) const
{
    int yOffset = (yd == MD::North) ? -1 : +1;
    int xOffset = (xd == MD::West)  ? -1 : +1;

    if (pos == exitPoint)
    {
        exitDir = exitValue;
    }

    if (pos.first + xOffset < 0 || pos.first + xOffset >= size.first || pos.second + yOffset< 0 || pos.second + yOffset >= size.second)
    {
        return 0;
    }

    if (    (walls[MD::WallInfo(pos.first, pos.second, xd)]  && walls[MD::WallInfo(pos.first, pos.second, yd)])
        ||  (walls[MD::WallInfo(pos.first, pos.second, xd)]  && walls[MD::WallInfo(pos.first, pos.second + yOffset, xd)])
        ||  (walls[MD::WallInfo(pos.first, pos.second, yd)] && walls[MD::WallInfo(pos.first + xOffset, pos.second, yd)])
        ||  (walls[MD::WallInfo(pos.first, pos.second + yOffset, xd)] && walls[MD::WallInfo(pos.first + xOffset, pos.second, yd)])
       )
    {
        return 0;
    }
    else
    {
        pos.first += xOffset;
        pos.second += yOffset;
        return 1 + distanceDiagonal(pos, exitDir, exitValue, size, yd, xd);
    }
}

int Maze::distanceStraight(ThorsUtil::Pos pos, int& exitDir, int exitValue, ThorsUtil::Size size, MD::Direction d) const
{
    if (pos == exitPoint)
    {
        exitDir = exitValue;
    }

    if (pos.first < 0 || pos.first >= size.first || pos.second < 0 || pos.second >= size.second)
    {
        return 0;
    }
    if (walls[MD::WallInfo(pos.first, pos.second, d)])
    {
        return 0;
    }
    else
    {
        int yOffset = (d == MD::North) ? -1 : (d == MD::South) ? +1 : 0;
        int xOffset = (d == MD::West)  ? -1 : (d == MD::East)  ? +1 : 0;

        pos.first += xOffset;
        pos.second += yOffset;
        return 1 + distanceStraight(pos, exitDir, exitValue, size, d);
    }
}

MD::Info Maze::info(ThorsUtil::Pos const& pos) const
{
    ThorsUtil::Size currentSize = size();
    MD::Info        result(9, 0);
    int&            exitDir = result[0];
    result[1]   = distanceDiagonal(pos, exitDir, 1, currentSize, MD::North, MD::West);
    result[2]   = distanceStraight(pos, exitDir, 2, currentSize, MD::North);
    result[3]   = distanceDiagonal(pos, exitDir, 3, currentSize, MD::North, MD::East);
    result[4]   = distanceStraight(pos, exitDir, 4, currentSize, MD::West);
    result[5]   = distanceStraight(pos, exitDir, 5, currentSize, MD::East);
    result[6]   = distanceDiagonal(pos, exitDir, 6, currentSize, MD::South, MD::West);
    result[7]   = distanceStraight(pos, exitDir, 7, currentSize, MD::South);
    result[8]   = distanceDiagonal(pos, exitDir, 8, currentSize, MD::South, MD::East);
    return result;
}

bool Maze::check(ThorsUtil::Pos const& pos, MD::Direction d) const
{
    return walls[MD::WallInfo{pos.first, pos.second, d}];
}

void Maze::draw(wxDC& dc) const
{
    static const wxPoint offset{16, 16};
    static const wxPoint cellSize{8, 8};

    walls.draw(dc, offset, cellSize);

    // Draw the destination.
    dc.SetPen(*wxRED_PEN);
    wxPoint tlexit{offset.x + exitPoint.first * cellSize.x + 0, offset.y + exitPoint.second * cellSize.y + 0};
    wxPoint brexit{offset.x + exitPoint.first * cellSize.x + 8, offset.y + exitPoint.second * cellSize.y + 8};
    dc.DrawLine(tlexit, brexit);
    wxPoint blexit{offset.x + exitPoint.first * cellSize.x + 0, offset.y + exitPoint.second * cellSize.y + 8};
    wxPoint trexit{offset.x + exitPoint.first * cellSize.x + 8, offset.y + exitPoint.second * cellSize.y + 0};
    dc.DrawLine(blexit, trexit);

    // Draw the start.
    dc.SetPen(*wxBLUE_PEN);
    wxPoint tlentr{offset.x + entrPoint.first * cellSize.x + 0, offset.y + entrPoint.second * cellSize.y + 0};
    wxPoint brentr{offset.x + entrPoint.first * cellSize.x + 8, offset.y + entrPoint.second * cellSize.y + 8};
    dc.DrawLine(tlentr, brentr);
    wxPoint blentr{offset.x + entrPoint.first * cellSize.x + 0, offset.y + entrPoint.second * cellSize.y + 8};
    wxPoint trentr{offset.x + entrPoint.first * cellSize.x + 8, offset.y + entrPoint.second * cellSize.y + 0};
    dc.DrawLine(blentr, trentr);
}

wxSize Maze::getSize() const
{
    ThorsUtil::Size size = walls.size();
    return wxSize{size.first * 8 + 32, size.second * 8 + 32};
}
