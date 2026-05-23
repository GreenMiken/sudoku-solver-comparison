#include <iostream>
#include "Benchmark.h"
#include "SolverBacktracking.h"
#include "SolverSimulatedAnnealing.h"
#include "SolverHillClimbWorstBlock.h"
#include "Grid.h"
#include "SolverHillClimbRandom.h"

int main()
{
    Grid grid("grid1.txt");
    SolverHillClimbRandom solver(grid);
    if (solver.solve())
        solver.visualize();
    else
        std::cout << "can't solve\n";

    Benchmark benchmark;
    benchmark.simAnnealTest("grid1.txt", 10);
    benchmark.test("grid2.txt", 10);
    benchmark.SAvsBT("grid4.txt", 5);
}
