#pragma once
#include "AbstractLocalSearchSolver.h"

class SolverSimulatedAnnealing : public AbstractLocalSearchSolver
{
public:
    SolverSimulatedAnnealing(const Grid& grid);
    bool solve() override;
    void setTemp(double temp);
    void setCooling(double cooling);
private:
    bool simAnneal();
    double probability(int delta) const;
    double getRandomProb() const;

    double m_temp;
    double m_coolRate;
    double m_startTemp;
    static constexpr double START_TEMP = 1.0;
    static constexpr double COOL_RATE = 0.99;
    static constexpr int MAX_ITERS = 200000;
    static constexpr int RESTART = 500;
};