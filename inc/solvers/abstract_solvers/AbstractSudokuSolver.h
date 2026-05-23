#pragma once
#include "Grid.h"
#include <cstddef>

class AbstractSudokuSolver
{
public:
    AbstractSudokuSolver(const Grid& grid);
    virtual ~AbstractSudokuSolver() = default;
    virtual bool solve() = 0;
    void visualize() const;

protected:

    Grid m_grid;
    size_t m_size;
    size_t m_blockSize;
};