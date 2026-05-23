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

            // najdi prázdnou buňku
            if (m_grid.getNumber(coord) == 0)
            {
                // zkus všechna čísla
                for (int num = 1; num <= static_cast<int>(m_size); ++num)
                {
                    if (isValid(coord, num))
                    {
                        m_grid.setNumber(coord, num);

                        // rekurze
                        if (solve())
                            return true;

                        // backtrack
                        m_grid.setNumber(coord, 0);
                    }
                }
                // žádné číslo nefungovalo
                return false;
            }
        }
    }
    // žádná prázdná buňka -> vyřešeno
    return true;
}

bool SolverBacktracking::isValid(const Coord& coord, int num) const
{
    // -------- řádek --------
    for (size_t col = 0; col < m_size; ++col)
    {
        if (col == coord.col)
            continue;

        if (m_grid.getNumber(Coord(coord.row, col)) == num)
            return false;
    }

    // -------- sloupec --------
    for (size_t row = 0; row < m_size; ++row)
    {
        if (row == coord.row)
            continue;

        if (m_grid.getNumber(Coord(row, coord.col)) == num)
            return false;
    }

    // -------- blok --------
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