#include <random>
#include <iostream>
#include "SolverSimulatedAnnealing.h"

SolverSimulatedAnnealing::SolverSimulatedAnnealing(
    const Grid& grid
)
    : AbstractLocalSearchSolver(grid),
      m_temp(START_TEMP),
      m_coolRate(COOL_RATE),
      m_startTemp(START_TEMP)
{
    m_restart = RESTART;
}

bool SolverSimulatedAnnealing::solve()
{
    restart();
    int iterCnt = 0;
    int noImprovementCnt = 0;
    while (m_conflictsCnt != 0)
    {
        if (noImprovementCnt > m_restart)
        {
            noImprovementCnt = 0;
            m_temp = m_startTemp; // reheat
        }
        std::pair<Coord, Coord> coords = getRandomCoordsInBlock();
        swapCoords(coords);
        int newCnt = countConflicts();
        if (m_conflictsCnt > newCnt)
        {
            m_conflictsCnt = newCnt;
            noImprovementCnt = 0;
        }
        else
        {
            noImprovementCnt++;
            if (probability(newCnt - m_conflictsCnt) >= getRandomProb()) // take worse state
            {
                m_conflictsCnt = newCnt;
            }
            else // don't take worse state
            {
                swapCoords(coords);
            }
        }
        iterCnt++;
        if (iterCnt > MAX_ITERS)
            return false;
        m_temp *= m_coolRate; // decrease temp
    }
    return true;
}

void SolverSimulatedAnnealing::setTemp(double temp)
{
    m_startTemp = temp;
    m_temp = temp;
}

void SolverSimulatedAnnealing::setCooling(double cooling)
{
    m_coolRate = cooling;
}

double SolverSimulatedAnnealing::probability(int delta) const
{
    return std::exp(-(double)delta / m_temp);
}

double SolverSimulatedAnnealing::getRandomProb() const
{
    static std::random_device rd;
    static std::mt19937 gen(rd());

    static std::uniform_real_distribution<double> dist(0.0, 1.0);

    return dist(gen);
}
