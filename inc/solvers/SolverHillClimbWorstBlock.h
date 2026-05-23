#pragma once
#include "AbstractLocalSearchSolver.h"

class SolverHillClimbWorstBlock : public AbstractLocalSearchSolver
{
public:
    SolverHillClimbWorstBlock(const Grid& grid);
    bool solve() override;
private:
    bool hillClimb();
    std::pair<Coord, Coord> getConflictCoords() const;

    static constexpr int RESTART = 100;
    static constexpr int MAX_ITERS = 500000;
};