#pragma once

#include <vector>
#include <memory>
#include <chrono>
#include <iostream>
#include "AbstractSudokuSolver.h"
#include "unordered_map"
#include "SolverFactory.h"

struct BenchmarkResult
{
    std::vector<std::chrono::milliseconds> times;
    int successCnt = 0;
};

struct TempCoolingPair
{
    TempCoolingPair(double temp, double cooling) : temp(temp), cooling(cooling) {}
    bool operator==(const TempCoolingPair& other) const
    {
        return temp == other.temp &&
               cooling == other.cooling;
    }
    struct TempCoolingPairHash
    {
        std::size_t operator()(const TempCoolingPair& pair) const
        {
            std::size_t h1 =
                    std::hash<double>{}(pair.temp);

            std::size_t h2 =
                    std::hash<double>{}(pair.cooling);

            return h1 ^ (h2 << 1);
        }
    };
    double temp;
    double cooling;
};

std::ostream& operator<<(std::ostream& stream, const TempCoolingPair& pair);

struct HCRestartPair
{
    HCRestartPair(SolverType type, int restart) : type(type), restart(restart) {}
    bool operator==(const HCRestartPair& other) const
    {
        return type == other.type &&
               restart == other.restart;
    }
    struct HCRestartPairHash
    {
        std::size_t operator()(
                const HCRestartPair& pair
        ) const
        {
            std::size_t h1 =
                    std::hash<int>{}(
                            static_cast<int>(pair.type)
                    );

            std::size_t h2 =
                    std::hash<int>{}(pair.restart);

            return h1 ^ (h2 << 1);
        }
    };
    SolverType type;
    int restart;
};

std::ostream& operator<<(std::ostream& stream, const HCRestartPair& pair);

class Benchmark
{
public:
    void test(const std::string& fileName, int iterations);
    void testWithoutBT(const std::string& fileName, int iterations);
    void simAnnealTest(const std::string& fileName, int iterations);
    void LSRestartTest(const std::string& fileName, int iterations);
    void SAvsBT(const std::string& fileName, int iterations);
    static std::string solverTypeToString(SolverType type);
private:
    void createAllSolvers(const std::string& fileName);
    void calculatePrintResults(const std::unordered_map<SolverType, BenchmarkResult>& res, int iterations);
    void calculatePrintResults(const std::unordered_map<TempCoolingPair, BenchmarkResult, TempCoolingPair::TempCoolingPairHash>& res, int iterations);
    void calculatePrintResults(const std::unordered_map<HCRestartPair, BenchmarkResult, HCRestartPair::HCRestartPairHash>& res, int iterations);
    std::unordered_map<SolverType, std::unique_ptr<AbstractSudokuSolver>> m_solvers;
};