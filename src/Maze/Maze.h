#ifndef THORSANVIL_VALKYRIE_MAZE_H
#define THORSANVIL_VALKYRIE_MAZE_H

#include "MazeDetail.h"
#include <ThorsUtil/Util.h>
#include <ThorsUI/Drawable.h>
#include <wx/wx.h>

namespace ThorsAnvil
{
    namespace ValkyrieMaze
    {

namespace MD = ThorsAnvil::ValkyrieMaze::MazeDetail;

class Maze: public ThorsUI::Drawable
{
    private:
        friend MD::MazeGenerator;

        MD::WallGrid        walls;
        ThorsUtil::Pos      exitPoint;
        ThorsUtil::Pos      entrPoint;

        friend std::istream& operator>>(std::istream& stream, Maze& m)
        {
            m.read(stream);
            return stream;
        }
        friend std::ostream& operator<<(std::ostream& stream, Maze const& m)
        {
            m.write(stream);
            return stream;
        }

        int distanceDiagonal(ThorsUtil::Pos pos, int& exitDir, int exitValue, ThorsUtil::Size size, MD::Direction yd, MD::Direction xd) const;
        int distanceStraight(ThorsUtil::Pos pos, int& exitDir, int exitValue, ThorsUtil::Size size, MD::Direction d) const;
    public:
        Maze();
        Maze(std::istream&& stream);
        void swap(Maze& other) noexcept;
        void read(std::istream& stream);
        void write(std::ostream& stream) const;

        ThorsUtil::Size  size()              const;
        ThorsUtil::Pos   start()             const;
        ThorsUtil::Pos   end()               const;
        MD::Info    info(ThorsUtil::Pos const& pos) const;
        bool        check(ThorsUtil::Pos const& pos, MD::Direction d) const;
        bool        atExit(ThorsUtil::Pos pos) const   {return pos == exitPoint;}
        virtual void    draw(wxDC& dc)  const override;
        virtual wxSize  getSize()       const override;
};

inline void swap(Maze& lhs, Maze& rhs) {lhs.swap(rhs);}

    }
}

#endif
