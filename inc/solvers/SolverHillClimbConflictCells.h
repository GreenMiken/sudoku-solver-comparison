#pragma once
#include "AbstractLocalSearchSolver.h"

class SolverHillClimbConflictCells : public AbstractLocalSearchSolver
{
public:
    SolverHillClimbConflictCells(const Grid& grid);
    bool solve() override;
private:
    bool hillClimb();
    std::pair<Coord, Coord> getConflictCoords() const;

    static constexpr int RESTART = 1000;
    static constexpr int MAX_ITERS = 500000;
};