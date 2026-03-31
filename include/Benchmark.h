#pragma once

#include <string>

struct BenchmarkResult {
    std::string fileName;
    double readMs;
    double sortMs;
    double splitMs;
    double writeMs;
    double totalMs;
};

BenchmarkResult benchmarkOneFile(const std::string& fileName);
void benchmarkAllFiles();