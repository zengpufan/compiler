#pragma once

namespace compiler
{
    class location
    {
    private:
        int col;
        int row;

    public:
        location(int row, int col)
            : row(row), col(col) {}
        inline int get_row(){return row;};
        inline int get_col(){return col;};
    };
}