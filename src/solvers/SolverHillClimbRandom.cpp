#include "SolverHillClimbRandom.h"
#include "iostream"

SolverHillClimbRandom::SolverHillClimbRandom(const Grid& grid) : AbstractLocalSearchSolver(grid)
{
    m_restart = RESTART;
}

bool SolverHillClimbRandom::hillClimb()
{
    std::pair<Coord, Coord> coords = getRandomCoordsInBlock();
    swapCoords(coords);
    int newCnt = countConflicts();
    if (m_conflictsCnt > newCnt)
    {
        m_conflictsCnt = newCnt;
        return true;
    }
    else if (m_conflictsCnt == newCnt)
        return false;
    
    swapCoords(coords);
    return false;
}

bool SolverHillClimbRandom::solve()
{
    restart();
    int noImprovementCnt = 0;
    int iterCnt = 0;

    while(m_conflictsCnt != 0)
    {
        bool res = hillClimb();
        if (!res)
            noImprovementCnt++;
        else
            noImprovementCnt = 0;

        if (noImprovementCnt > m_restart)
        {
            restart();
            noImprovementCnt = 0;
        }

        iterCnt++;
        if (iterCnt > MAX_ITERS)
            return false;
    }
    return true;
}