#include <random>
#include <iostream>
#include "SolverHillClimbConflictCells.h"

SolverHillClimbConflictCells::SolverHillClimbConflictCells(const Grid& grid) : AbstractLocalSearchSolver(grid)
{
    m_restart = RESTART;
}

std::pair<Coord, Coord> SolverHillClimbConflictCells::getConflictCoords() const
{
    static std::random_device rd;
    static std::mt19937 gen(rd());

    std::vector<Coord> conflictCoords;

    // -------- najdi všechny konfliktní nefixní buňky --------
    for (size_t row = 0; row < m_size; ++row)
    {
        for (size_t col = 0; col < m_size; ++col)
        {
            Coord coord(row, col);

            if (m_grid.isFixed(coord))
                continue;

            int value = m_grid.getNumber(coord);

            bool hasConflict = false;

            // řádek
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

            // sloupec
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
                conflictCoords.push_back(coord);
        }
    }

    // fallback
    if (conflictCoords.empty())
        return getRandomCoordsInBlock();

    // -------- vyber první konfliktní buňku --------
    std::uniform_int_distribution<size_t> dist(0, conflictCoords.size() - 1);

    Coord first = conflictCoords[dist(gen)];

    // -------- najdi konfliktní buňky ve stejném bloku --------
    size_t blockRow = first.row / m_blockSize;
    size_t blockCol = first.col / m_blockSize;

    std::vector<Coord> sameBlockConflicts;

    for (size_t r = 0; r < m_blockSize; ++r)
    {
        for (size_t c = 0; c < m_blockSize; ++c)
        {
            size_t row = blockRow * m_blockSize + r;
            size_t col = blockCol * m_blockSize + c;

            Coord coord(row, col);

            if (coord.row == first.row && coord.col == first.col)
                continue;

            if (m_grid.isFixed(coord))
                continue;

            int value = m_grid.getNumber(coord);

            bool hasConflict = false;

            // řádek
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

            // sloupec
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
                sameBlockConflicts.push_back(coord);
        }
    }

    // pokud není druhá konfliktní, fallback
    if (sameBlockConflicts.empty())
        return getRandomCoordsInBlock();

    // -------- vyber druhou --------
    std::uniform_int_distribution<size_t> dist2(0, sameBlockConflicts.size() - 1);

    Coord second = sameBlockConflicts[dist2(gen)];

    return { first, second };
}

bool SolverHillClimbConflictCells::hillClimb()
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

bool SolverHillClimbConflictCells::solve()
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
            //std::cout << "resatrt\n";
            restart();
            noImprovementCnt = 0;
        }

        iterCnt++;
        if (iterCnt > MAX_ITERS)
            return false;
    }
    return true;
}
