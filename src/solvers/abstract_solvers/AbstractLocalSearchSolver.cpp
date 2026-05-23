#include "AbstractLocalSearchSolver.h"
#include <algorithm>
#include <random>
#include <iostream>

AbstractLocalSearchSolver::AbstractLocalSearchSolver(const Grid& grid) : AbstractSudokuSolver(grid), m_restart(RESTART_CONST) {}

void AbstractLocalSearchSolver::setRestart(int restart)
{
    m_restart = restart;
}

void AbstractLocalSearchSolver::restart()
{
    const size_t blockSize = m_grid.getBlockSize();

    static std::random_device rd;
    static std::mt19937 gen(rd());

    // projdi všechny bloky
    for (size_t blockRow = 0; blockRow < blockSize; ++blockRow)
    {
        for (size_t blockCol = 0; blockCol < blockSize; ++blockCol)
        {
            std::vector<int> missingNumbers;
            std::vector<Coord> freeCoords;

            // čísla, která už jsou v bloku
            std::vector<bool> used(m_size + 1, false);

            // projdi buňky bloku
            for (size_t r = 0; r < blockSize; ++r)
            {
                for (size_t c = 0; c < blockSize; ++c)
                {
                    size_t globalRow = blockRow * blockSize + r;
                    size_t globalCol = blockCol * blockSize + c;

                    Coord coord(globalRow, globalCol);

                    int number = m_grid.getNumber(coord);

                    if (m_grid.isFixed(coord))
                        used[number] = true;

                    if (!m_grid.isFixed(coord))
                        freeCoords.push_back(coord);
                }
            }

            // zjisti chybějící čísla
            for (size_t num = 1; num <= m_size; ++num)
            {
                if (!used[num])
                    missingNumbers.push_back(num);
            }

            // náhodně promíchej
            std::shuffle(missingNumbers.begin(), missingNumbers.end(), gen);

            // doplň čísla do nefixních buněk
            for (size_t i = 0; i < freeCoords.size(); ++i)
            {
                m_grid.setNumber(freeCoords[i], missingNumbers[i]);
            }
        }
    }

    m_conflictsCnt = countConflicts();
}

int AbstractLocalSearchSolver::countConflicts() const
{
    int conflicts = 0;

    // -------- ŘÁDKY --------
    for (size_t row = 0; row < m_size; ++row)
    {
        std::vector<int> freq(m_size + 1, 0);

        for (size_t col = 0; col < m_size; ++col)
        {
            int value = m_grid.getNumber(Coord(row, col));

            // 0 ignorujeme
            if (value != 0)
                freq[value]++;
        }

        for (size_t num = 1; num <= m_size; ++num)
        {
            if (freq[num] > 1)
                conflicts += freq[num] - 1;
        }
    }

    // -------- SLOUPCE --------
    for (size_t col = 0; col < m_size; ++col)
    {
        std::vector<int> freq(m_size + 1, 0);

        for (size_t row = 0; row < m_size; ++row)
        {
            int value = m_grid.getNumber(Coord(row, col));

            if (value != 0)
                freq[value]++;
        }

        for (size_t num = 1; num <= m_size; ++num)
        {
            if (freq[num] > 1)
                conflicts += freq[num] - 1;
        }
    }

    return conflicts;
}

std::pair<Coord, Coord> AbstractLocalSearchSolver::getRandomCoordsInBlock() const
{
    static std::random_device rd;
    static std::mt19937 gen(rd());

    while (true)
    {
        // náhodný blok
        size_t blockRow = gen() % m_blockSize;
        size_t blockCol = gen() % m_blockSize;

        std::vector<Coord> candidates;

        // projdi blok
        for (size_t r = 0; r < m_blockSize; ++r)
        {
            for (size_t c = 0; c < m_blockSize; ++c)
            {
                size_t row = blockRow * m_blockSize + r;
                size_t col = blockCol * m_blockSize + c;

                Coord coord(row, col);

                if (!m_grid.isFixed(coord))
                    candidates.push_back(coord);
            }
        }

        // musí být alespoň 2 nefixní
        if (candidates.size() < 2)
            continue;

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
}

void AbstractLocalSearchSolver::swapCoords(const Coord& a, const Coord& b)
{
    int temp = m_grid.getNumber(a);

    m_grid.setNumber(a, m_grid.getNumber(b));
    m_grid.setNumber(b, temp);
}

void AbstractLocalSearchSolver::swapCoords(const std::pair<Coord, Coord>& coords)
{
    int temp = m_grid.getNumber(coords.first);

    m_grid.setNumber(coords.first, m_grid.getNumber(coords.second));
    m_grid.setNumber(coords.second, temp);
}

int AbstractLocalSearchSolver::countConflictsForSwap(const Coord& a, const Coord& b) const
{
    //TODO
    return 1;
}