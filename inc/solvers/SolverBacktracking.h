#pragma once
#include "AbstractSudokuSolver.h"

class SolverBacktracking : public AbstractSudokuSolver
{
public:
    SolverBacktracking(const Grid& grid);
    bool solve() override;
private:
    bool isValid(const Coord& coord, int num) const;
};