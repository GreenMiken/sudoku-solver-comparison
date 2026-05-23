#include "solvers/abstract_solvers/AbstractSudokuSolver.h"
#include <algorithm>
#include <random>
#include <iostream>
#include <iomanip>

AbstractSudokuSolver::AbstractSudokuSolver(const Grid& grid) : m_grid(grid), m_blockSize(m_grid.getBlockSize()), m_size(m_grid.getSize())
{}

void AbstractSudokuSolver::visualize() const
{
    for (size_t row = 0; row < m_size; ++row)
    {
        // horizontální oddělovač bloků
        if (row % m_blockSize == 0)
        {
            for (size_t i = 0; i < m_size * 2 + m_blockSize + 1; ++i)
                std::cout << "-";

            std::cout << "\n";
        }

        for (size_t col = 0; col < m_size; ++col)
        {
            // vertikální oddělovač bloků
            if (col % m_blockSize == 0)
                std::cout << "| ";

            int value = m_grid.getNumber(Coord(row, col));

            if (value == 0)
                std::cout << ". ";
            else
                std::cout << value << " ";
        }

        std::cout << "|\n";
    }

    // spodní čára
    for (size_t i = 0; i < m_size * 2 + m_blockSize + 1; ++i)
        std::cout << "-";

    std::cout << "\n";
}