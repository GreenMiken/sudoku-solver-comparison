cmake_minimum_required(VERSION 3.10)
project(final_project)

set(CMAKE_CXX_STANDARD 20)

add_executable(final_project main.cpp
        src/Grid.cpp
        inc/Grid.h
        src/Benchmark.cpp
        inc/Benchmark.h
        inc/solvers/abstract_solvers/AbstractSudokuSolver.h
        inc/solvers/SolverHillClimbRandom.h
        src/solvers/abstract_solvers/AbstractSudokuSolver.cpp
        src/solvers/SolverHillClimbRandom.cpp
        inc/solvers/SolverHillClimbWorstBlock.h
        src/solvers/SolverHillClimbWorstBlock.cpp
        inc/solvers/SolverHillClimbConflictCells.h
        src/solvers/SolverHillClimbConflictCells.cpp
        inc/solvers/SolverSimulatedAnnealing.h
        src/solvers/SolverSimulatedAnnealing.cpp
        inc/SolverFactory.h
        src/SolverFactory.cpp
        inc/solvers/SolverBacktracking.h
        src/solvers/SolverBacktracking.cpp
        inc/solvers/abstract_solvers/AbstractLocalSearchSolver.h
        src/solvers/abstract_solvers/AbstractLocalSearchSolver.cpp)

target_include_directories(final_project PRIVATE inc inc/solvers inc/solvers/abstract_solvers)
