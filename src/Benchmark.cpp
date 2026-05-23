#include "Benchmark.h"

#include <chrono>
#include <iostream>

#include "SolverSimulatedAnnealing.h"

std::ostream& operator<<(std::ostream& stream, const TempCoolingPair& pair)
{
    stream << "temp: "
           << pair.temp
           << ", cooling: "
           << pair.cooling;

    return stream;
}

std::ostream& operator<<(std::ostream& stream, const HCRestartPair& pair)
{
    stream << "solver: "
           << Benchmark::solverTypeToString(pair.type)
           << ", restart: "
           << pair.restart;

    return stream;
}

void Benchmark::createAllSolvers(const std::string& fileName)
{
    m_solvers.emplace(
            SolverType::HCWorstBlock,
            createSolver(SolverType::HCWorstBlock, fileName)
    );

    m_solvers.emplace(
            SolverType::HCRandom,
            createSolver(SolverType::HCRandom, fileName)
    );

    m_solvers.emplace(
            SolverType::HCConflictCells,
            createSolver(SolverType::HCConflictCells, fileName)
    );

    m_solvers.emplace(
            SolverType::SA,
            createSolver(SolverType::SA, fileName)
    );

    m_solvers.emplace(
            SolverType::Backtracking,
            createSolver(SolverType::Backtracking, fileName)
    );
}

void Benchmark::calculatePrintResults(
        const std::unordered_map<
                SolverType,
                BenchmarkResult
        >& res,
        int iterations
)
{
    std::vector<SolverType> order =
            {
                    SolverType::HCRandom,
                    SolverType::HCWorstBlock,
                    SolverType::HCConflictCells,
                    SolverType::SA,
                    SolverType::Backtracking
            };

    for (const auto& type : order)
    {
        auto it = res.find(type);

        if (it == res.end())
            continue;

        const auto& result = it->second;

        if (result.times.empty())
            continue;

        long long sum = 0;

        for (const auto& time : result.times)
        {
            sum += time.count();
        }

        double average =
                static_cast<double>(sum) /
                result.times.size();

        double successRate =
                static_cast<double>(result.successCnt) /
                iterations * 100.0;

        std::cout
                << solverTypeToString(type)
                << " average: "
                << average
                << " ms"
                << ", success rate: "
                << successRate
                << "%\n";
    }
}

void Benchmark::calculatePrintResults(
        const std::unordered_map<
                HCRestartPair,
                BenchmarkResult,
                HCRestartPair::HCRestartPairHash
        >& res,
        int iterations
)
{
    std::vector<SolverType> order =
            {
                    SolverType::HCRandom,
                    SolverType::HCWorstBlock,
                    SolverType::HCConflictCells,
                    SolverType::SA
            };

    std::array<int, 3> restarts =
            {
                    100,
                    500,
                    1000
            };

    for (const auto& solverType : order)
    {
        std::cout
                << "\n=== "
                << solverTypeToString(solverType)
                << " ===\n";

        for (const auto& restart : restarts)
        {
            HCRestartPair key(
                    solverType,
                    restart
            );

            auto it = res.find(key);

            if (it == res.end())
                continue;

            const auto& result = it->second;

            if (result.times.empty())
                continue;

            long long sum = 0;

            for (const auto& time : result.times)
            {
                sum += time.count();
            }

            double average =
                    static_cast<double>(sum) /
                    result.times.size();

            double successRate =
                    static_cast<double>(result.successCnt) /
                    iterations * 100.0;

            std::cout
                    << "restart: "
                    << restart
                    << " average: "
                    << average
                    << " ms"
                    << ", success rate: "
                    << successRate
                    << "%\n";
        }
    }
}

void Benchmark::calculatePrintResults(
        const std::unordered_map<
                TempCoolingPair,
                BenchmarkResult,
                TempCoolingPair::TempCoolingPairHash
        >& res,
        int iterations
)
{
    std::array<double, 3> temps =
            {
                    1,
                    5,
                    20
            };

    std::array<double, 4> coolings =
            {
                    0.9,
                    0.99,
                    0.995,
                    0.999
            };

    for (const auto& temp : temps)
    {
        std::cout
                << "\n=== temp: "
                << temp
                << " ===\n";

        for (const auto& cooling : coolings)
        {
            TempCoolingPair key(
                    temp,
                    cooling
            );

            auto it = res.find(key);

            if (it == res.end())
                continue;

            const auto& result = it->second;

            if (result.times.empty())
                continue;

            long long sum = 0;

            for (const auto& time : result.times)
            {
                sum += time.count();
            }

            double average =
                    static_cast<double>(sum) /
                    result.times.size();

            double successRate =
                    static_cast<double>(result.successCnt) /
                    iterations * 100.0;

            std::cout
                    << "cooling: "
                    << cooling
                    << " average: "
                    << average
                    << " ms"
                    << ", success rate: "
                    << successRate
                    << "%\n";
        }
    }
}

void Benchmark::test(
        const std::string& fileName,
        int iterations
)
{
    std::unordered_map<
            SolverType,
            BenchmarkResult
    > res;

    for (int i = 0; i < iterations; ++i)
    {
        m_solvers.clear();
        createAllSolvers(fileName);

        for (const auto& [key, value] : m_solvers)
        {
            auto start =
                    std::chrono::high_resolution_clock::now();

            bool solved = value->solve();

            auto end =
                    std::chrono::high_resolution_clock::now();

            auto duration =
                    std::chrono::duration_cast<
                            std::chrono::milliseconds
                    >(end - start);

            res[key].times.push_back(duration);

            if (solved)
            {
                res[key].successCnt++;
            }
        }
    }

    calculatePrintResults(res, iterations);
}

void Benchmark::testWithoutBT(const std::string& fileName, int iterations)
{
    std::unordered_map<SolverType, BenchmarkResult> res;

    for (int i = 0; i < iterations; ++i)
    {
        m_solvers.clear();
        createAllSolvers(fileName);

        for (const auto& [key, value] : m_solvers)
        {
            if (key == SolverType::Backtracking)
                continue;

            auto start =
                    std::chrono::high_resolution_clock::now();

            bool solved = value->solve();

            auto end =
                    std::chrono::high_resolution_clock::now();

            auto duration =
                    std::chrono::duration_cast<
                            std::chrono::milliseconds
                    >(end - start);

            res[key].times.push_back(duration);

            if (solved)
            {
                res[key].successCnt++;
            }
        }
    }

    calculatePrintResults(res, iterations);
}

void Benchmark::simAnnealTest(
        const std::string& fileName,
        int iterations
)
{
    std::array<double, 3> temps = {1, 5, 20};

    std::array<double, 4> coolings ={0.9, 0.99, 0.995, 0.999};

    std::unordered_map<TempCoolingPair, BenchmarkResult, TempCoolingPair::TempCoolingPairHash> res;

    for (const auto& temp : temps)
    {
        for (const auto& cooling : coolings)
        {
            TempCoolingPair key(temp, cooling);

            for (int i = 0; i < iterations; ++i)
            {
                auto solverBase = createSolver(SolverType::SA, fileName);

                auto* solver = dynamic_cast<SolverSimulatedAnnealing*>(solverBase.get());

                solver->setTemp(temp);
                solver->setCooling(cooling);

                auto start =
                        std::chrono::high_resolution_clock::now();

                bool solved = solver->solve();

                auto end =
                        std::chrono::high_resolution_clock::now();

                auto duration =
                        std::chrono::duration_cast<
                                std::chrono::milliseconds
                        >(end - start);

                res[key].times.push_back(duration);

                if (solved)
                {
                    res[key].successCnt++;
                }
            }
        }
    }

    calculatePrintResults(res, iterations);
}

void Benchmark::LSRestartTest(const std::string& fileName, int iterations)
{
    std::unordered_map<HCRestartPair, BenchmarkResult, HCRestartPair::HCRestartPairHash> res;
    std::array<int, 3> restarts = {100, 500, 1000};
    std::array<SolverType, 4> solvers = {SolverType::HCRandom, SolverType::HCWorstBlock, SolverType::HCConflictCells, SolverType::SA};

    for (const auto& solverType : solvers)
    {
        for (const auto& restart : restarts)
        {
            HCRestartPair key(solverType, restart);

            for (int i = 0; i < iterations; ++i)
            {
                auto solverBase = createSolver(solverType, fileName);
                auto* solver = dynamic_cast<AbstractLocalSearchSolver*>(solverBase.get());

                solver->setRestart(restart);

                auto start = std::chrono::high_resolution_clock::now();

                bool solved = solver->solve();

                auto end = std::chrono::high_resolution_clock::now();
                auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

                res[key].times.push_back(duration);

                if (solved)
                {
                    res[key].successCnt++;
                }
            }
        }
    }

    calculatePrintResults(res, iterations);
}

void Benchmark::SAvsBT(const std::string& fileName, int iterations)
{
    std::unordered_map<SolverType, BenchmarkResult> res;

    for (int i = 0; i < iterations; ++i)
    {
        m_solvers.clear();
        createAllSolvers(fileName);

        for (const auto& [key, value] : m_solvers)
        {
            if (key == SolverType::HCConflictCells || key == SolverType::HCRandom || key == SolverType::HCWorstBlock)
                continue;

            auto start =
                    std::chrono::high_resolution_clock::now();

            bool solved = value->solve();

            auto end =
                    std::chrono::high_resolution_clock::now();

            auto duration =
                    std::chrono::duration_cast<
                            std::chrono::milliseconds
                    >(end - start);

            res[key].times.push_back(duration);

            if (solved)
            {
                res[key].successCnt++;
            }
        }
    }

    calculatePrintResults(res, iterations);
}

std::string Benchmark::solverTypeToString(SolverType type)
{
    switch (type)
    {
        case SolverType::HCRandom:
            return "HillClimbRandom";

        case SolverType::HCWorstBlock:
            return "HillClimbWorstBlock";

        case SolverType::HCConflictCells:
            return "HillClimbConflictCells";

        case SolverType::SA:
            return "SimulatedAnnealing";

        case SolverType::Backtracking:
            return "Backtracking";
    }

    return "Unknown";
}