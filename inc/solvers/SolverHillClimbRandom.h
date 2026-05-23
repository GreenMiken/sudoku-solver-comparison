#pragma once
#include "AbstractLocalSearchSolver.h"

class SolverHillClimbRandom : public AbstractLocalSearchSolver
{
public:
    SolverHillClimbRandom(const Grid& grid);
    bool solve() override;
private:
    bool hillClimb();

    static constexpr int RESTART = 1000;
    static constexpr int MAX_ITERS = 500000;
};
