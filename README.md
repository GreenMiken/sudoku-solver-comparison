# Sudoku Solver Comparison

Comparison of systematic and non-systematic search algorithms for solving Sudoku puzzles.

This project implements and benchmarks multiple Sudoku solving approaches, including local search heuristics, simulated annealing, and backtracking.

The goal was to compare algorithm performance, success rate, and behavior on Sudoku grids with different densities.

---

# Implemented Algorithms

## 1. Hill Climbing — Random Swap

A hill climbing algorithm that randomly selects two mutable cells within the same block and swaps them.

The objective function is the total number of conflicts in the Sudoku grid.

---

## 2. Hill Climbing — Conflict Cells

A modified hill climbing approach that selects conflicting cells within the same block before performing a swap.

The objective function remains the total number of conflicts.

---

## 3. Hill Climbing — Worst Block

This heuristic selects the block containing the highest number of conflicts and swaps two random cells inside that block.

---

## 4. Simulated Annealing

A simulated annealing solver based on random swaps.

Unlike standard hill climbing, the algorithm occasionally accepts worse states to escape local minima.

Features:
- configurable temperature
- cooling rate
- reheating mechanism after stagnation

The objective function is again the total number of conflicts.

---

## 5. Backtracking

A classic systematic search algorithm.

The solver recursively tries all possible values and backtracks whenever a conflict occurs.

Backtracking performs extremely well on easier Sudoku instances but becomes significantly slower on sparse grids with many empty cells.

---

# Benchmarking

The project contains several benchmark tests for evaluating solver performance.

Implemented benchmarks include:
- average solving time
- success rate
- simulated annealing parameter tuning
- local search restart optimization
- simulated annealing vs backtracking comparison

---

# Sudoku Grids

The `grids` directory contains four Sudoku instances with different difficulty levels.

Backtracking dominates on easier grids (`grid1`–`grid3`), while simulated annealing performs significantly better on sparse grids such as `grid4`.

---

# Results

One of the most interesting observations was that the simple random hill climbing approach consistently outperformed the more complex heuristics.

Findings:
- `HillClimbRandom` achieved the best performance among hill climbing variants
- `HillClimbWorstBlock` was consistently the slowest heuristic
- simulated annealing outperformed other local search approaches
- backtracking dominated on easier grids
- simulated annealing performed significantly better on sparse grids

Benchmark outputs and graphs are available in the `img` directory.

---

# Best Parameters

## Simulated Annealing

Best observed parameters:
- temperature: `1.0`
- cooling rate: `0.99`

---

## Local Search Restart

Optimal restart interval was approximately:
- `500–1000` unsuccessful iterations

The `WorstBlock` heuristic preferred more frequent restarts around `100` iterations.

---

# Architecture

All solvers inherit from `AbstractSudokuSolver`, which defines the abstract `solve()` interface.

Local search solvers inherit from `AbstractLocalSearchSolver`.

The `Benchmark` class manages all tests and stores solver instances in a hash map. Solvers are created using a factory function (`createSolver`).

The project heavily utilizes:
- polymorphism
- inheritance
- abstract interfaces
- benchmarking utilities

---

# Technologies

- C++
- Object-Oriented Programming
- Search Algorithms
- Benchmarking
- Local Search
- Simulated Annealing
