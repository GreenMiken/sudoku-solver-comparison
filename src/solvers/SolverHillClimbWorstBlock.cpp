#include <random>
#include <iostream>
#include "SolverHillClimbWorstBlock.h"

SolverHillClimbWorstBlock::SolverHillClimbWorstBlock(const Grid& grid) : AbstractLocalSearchSolver(grid)
{
    m_restart = RESTART;
}

std::pair<Coord, Coord> SolverHillClimbWorstBlock::getConflictCoords() const
{
    static std::random_device rd;
    static std::mt19937 gen(rd());

    int maxConflicts = -1;

    size_t bestBlockRow = 0;
    size_t bestBlockCol = 0;

    for (size_t blockRow = 0; blockRow < m_blockSize; ++blockRow)
    {
        for (size_t blockCol = 0; blockCol < m_blockSize; ++blockCol)
        {
            int blockConflicts = 0;

            for (size_t r = 0; r < m_blockSize; ++r)
            {
                for (size_t c = 0; c < m_blockSize; ++c)
                {
                    size_t row = blockRow * m_blockSize + r;
                    size_t col = blockCol * m_blockSize + c;

                    Coord coord(row, col);

                    if (m_grid.isFixed(coord))
                        continue;

                    int value = m_grid.getNumber(coord);

                    bool hasConflict = false;

                    for (size_t i = 0; i < m_size; ++i)
                    {
                        if (i == col)
                            continue;

                        if (m_grid.getNumber(Coord(row, i)) == value)
                        {
                            hasConflict = true;
                            break;
                        }
                    }

                    if (!hasConflict)
                    {
                        for (size_t i = 0; i < m_size; ++i)
                        {
                            if (i == row)
                                continue;

                            if (m_grid.getNumber(Coord(i, col)) == value)
                            {
                                hasConflict = true;
                                break;
                            }
                        }
                    }

                    if (hasConflict)
                        blockConflicts++;
                }
            }

            if (blockConflicts > maxConflicts)
            {
                maxConflicts = blockConflicts;
                bestBlockRow = blockRow;
                bestBlockCol = blockCol;
            }
        }
    }

    std::vector<Coord> candidates;

    for (size_t r = 0; r < m_blockSize; ++r)
    {
        for (size_t c = 0; c < m_blockSize; ++c)
        {
            size_t row = bestBlockRow * m_blockSize + r;
            size_t col = bestBlockCol * m_blockSize + c;

            Coord coord(row, col);

            if (!m_grid.isFixed(coord))
                candidates.push_back(coord);
        }
    }

    // fallback
    if (candidates.size() < 2)
        return getRandomCoordsInBlock();

    std::uniform_int_distribution<size_t> dist(0, candidates.size() - 1);

    size_t i = dist(gen);
    size_t j;

    do
    {
        j = dist(gen);
    }
    while (i == j);

    return { candidates[i], candidates[j] };
}

bool SolverHillClimbWorstBlock::hillClimb()
{
    std::pair<Coord, Coord> coords = getConflictCoords();
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

bool SolverHillClimbWorstBlock::solve()
{
    restart();
    int noImprovementCnt = 0;
    int iterCnt = 0;

    while(m_conflictsCnt != 0)
    {
        bool res = hillClimb(); // get random neighbour
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
