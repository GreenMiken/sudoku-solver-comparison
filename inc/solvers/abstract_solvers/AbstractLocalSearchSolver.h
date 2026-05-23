#pragma once

#include "AbstractSudokuSolver.h"

class AbstractLocalSearchSolver : public AbstractSudokuSolver
{
public:
    AbstractLocalSearchSolver(const Grid& grid);
    void setRestart(int restart);

protected:
    void restart();
    int countConflicts() const;
    std::pair<Coord, Coord> getRandomCoordsInBlock() const;
    void swapCoords(const Coord& a, const Coord& b);
    void swapCoords(const std::pair<Coord, Coord>& coords);
    int countConflictsForSwap(const Coord& a, const Coord& b) const;

    static constexpr int RESTART_CONST = 500;

    int m_restart;
    int m_conflictsCnt;
};