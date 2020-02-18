#ifndef THORSANVIL_VALKYRIE_MAZE_DETAIL_H
#define THORSANVIL_VALKYRIE_MAZE_DETAIL_H

#include <ThorsUtil/Util.h>
#include <wx/wx.h>
#include <vector>
#include <iostream>

namespace ThorsAnvil
{
    namespace ValkyrieMaze
    {

class Maze;

        namespace MazeDetail
        {

using Info = std::vector<int>;
enum Direction {North, South, East, West};
inline std::ostream& operator<<(std::ostream& stream, Direction const& d)
{
    switch (d)
    {
        case North: stream << "North ";break;
        case South: stream << "South ";break;
        case East:  stream << "East  ";break;
        case West:  stream << "West  ";break;
    }
    return stream;
}

class MazeGenerator
{
    int width;
    int height;
    public:
        MazeGenerator(int width, int height);
        void operator()(Maze&) const;
};

struct WallInfo
{
    int         x;
    int         y;
    Direction   d;
    WallInfo(int x, int y, Direction d): x(x), y(y), d(d) {}
    bool operator<(WallInfo const& rhs) const
    {
        return std::forward_as_tuple(x, y, d) < std::forward_as_tuple(rhs.x, rhs.y, rhs.d);
    }
};

struct Bool
{
    bool value;
    operator bool() const {return value;}
    friend std::istream& operator>>(std::istream& stream, Bool& data)
    {
        char x = stream.get();
        data.value = (x == '0') ? false : true;
        return stream;
    }
};

class WallGrid
{
    std::vector<std::vector<bool>>      horzWalls;
    std::vector<std::vector<bool>>      vertWalls;
    using Ref = std::vector<bool>::reference;

    public:
        WallGrid();
        WallGrid(int width, int height);
        void        swap(WallGrid& other) noexcept;
        void        clear();
        ThorsUtil::Size  size() const;
        Ref         operator[](WallInfo const& info);
        bool        operator[](WallInfo const& info) const;

        void draw(wxDC& dc, wxPoint const& offset, wxPoint const& cellSize) const;
        void read(std::istream& stream);
        void write(std::ostream& stream) const;

        friend std::istream& operator>>(std::istream& stream, WallGrid& data)
        {
            data.read(stream);
            return stream;
        }
        friend std::ostream& operator<<(std::ostream& stream, WallGrid const& data)
        {
            data.write(stream);
            return stream;
        }
};

inline void swap(WallGrid& lhs, WallGrid& rhs) {lhs.swap(rhs);}

        }
    }
}

#endif
