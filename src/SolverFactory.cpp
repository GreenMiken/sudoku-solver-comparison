#include "SolverFactory.h"
#include "Grid.h"
#include "SolverSimulatedAnnealing.h"
#include "SolverHillClimbConflictCells.h"
#include "SolverHillClimbRandom.h"
#include "SolverHillClimbWorstBlock.h"
#include "SolverBacktracking.h"

std::unique_ptr<AbstractSudokuSolver> createSolver(SolverType type, const std::string& fileName)
{
    switch (type)
    {
        case SolverType::HCConflictCells:
            return std::make_unique<SolverHillClimbConflictCells>(Grid(fileName));
        case SolverType::HCRandom:
            return std::make_unique<SolverHillClimbRandom>(Grid(fileName));
        case SolverType::HCWorstBlock:
            return std::make_unique<SolverHillClimbWorstBlock>(Grid(fileName));
        case SolverType::SA:
            return std::make_unique<SolverSimulatedAnnealing>(Grid(fileName));
        case SolverType::Backtracking:
            return std::make_unique<SolverBacktracking>(Grid(fileName));

    }
}