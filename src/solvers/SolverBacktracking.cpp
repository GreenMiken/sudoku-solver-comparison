#include "SolverBacktracking.h"

SolverBacktracking::SolverBacktracking(const Grid& grid) : AbstractSudokuSolver(grid)
{}

bool SolverBacktracking::solve()
{
    for (size_t row = 0; row < m_size; ++row)
    {
        for (size_t col = 0; col < m_size; ++col)
        {
            Coord coord(row, col);

            if (m_grid.getNumber(coord) == 0)
            {
                for (int num = 1; num <= static_cast<int>(m_size); ++num)
                {
                    if (isValid(coord, num))
                    {
                        m_grid.setNumber(coord, num);

                        if (solve())
                            return true;

                        // backtrack
                        m_grid.setNumber(coord, 0);
                    }
                }
                return false;
            }
        }
    }
    return true;
}

bool SolverBacktracking::isValid(const Coord& coord, int num) const
{
    // row
    for (size_t col = 0; col < m_size; ++col)
    {
        if (col == coord.col)
            continue;

        if (m_grid.getNumber(Coord(coord.row, col)) == num)
            return false;
    }

    // col
    for (size_t row = 0; row < m_size; ++row)
    {
        if (row == coord.row)
            continue;

        if (m_grid.getNumber(Coord(row, coord.col)) == num)
            return false;
    }

    size_t startRow =
            (coord.row / m_blockSize) * m_blockSize;

    size_t startCol =
            (coord.col / m_blockSize) * m_blockSize;

    for (size_t r = 0; r < m_blockSize; ++r)
    {
        for (size_t c = 0; c < m_blockSize; ++c)
        {
            size_t row = startRow + r;
            size_t col = startCol + c;

            if (row == coord.row &&
                col == coord.col)
            {
                continue;
            }

            if (m_grid.getNumber(Coord(row, col)) == num)
                return false;
        }
    }

    return true;
}