#pragma once
#include <memory>
#include "AbstractSudokuSolver.h"

enum class SolverType
{
    HCConflictCells,
    HCRandom,
    HCWorstBlock,
    SA,
    Backtracking
};

std::unique_ptr<AbstractSudokuSolver> createSolver(SolverType type, const std::string& fileName);