#include "Benchmark.h"

#include <chrono>
#include <cctype>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#include "Exceptions.h"
#include "FileManager.h"
#include "Processor.h"

namespace {
using Clock = std::chrono::steady_clock;

double toMilliseconds(const Clock::duration& duration) {
    return std::chrono::duration<double, std::milli>(duration).count();
}

std::vector<std::string> benchmarkFiles() {
    return {
        "data/students1000.txt",
        "data/students10000.txt",
        "data/students100000.txt",
        "data/students1000000.txt",
        "data/students10000000.txt"
    };
}

std::string extractSizeLabel(const std::string& fileName) {
    std::string digits;
    for (char ch : fileName) {
        if (std::isdigit(static_cast<unsigned char>(ch))) {
            digits.push_back(ch);
        }
    }
    return digits.empty() ? "unknown" : digits;
}
}

BenchmarkResult benchmarkOneFile(const std::string& fileName) {
    const auto totalStart = Clock::now();

    const auto readStart = Clock::now();
    std::vector<Person> students = loadStudentsFromFile(fileName);
    const auto readEnd = Clock::now();

    for (Person& student : students) {
        student.calculateFinalGrade(CalculationMethod::Average);
    }

    const auto sortStart = Clock::now();
    sortStudents(students, SortField::Surname);
    const auto sortEnd = Clock::now();

    std::vector<Person> failed;
    std::vector<Person> passed;

    const auto splitStart = Clock::now();
    splitStudents(students, failed, passed, CalculationMethod::Average);
    const auto splitEnd = Clock::now();

    const std::string sizeLabel = extractSizeLabel(fileName);
    const std::string failedFile = "output/failed_" + sizeLabel + ".txt";
    const std::string passedFile = "output/passed_" + sizeLabel + ".txt";

    const auto writeStart = Clock::now();
    writeStudentsToFile(failedFile, failed, CalculationMethod::Average);
    writeStudentsToFile(passedFile, passed, CalculationMethod::Average);
    const auto writeEnd = Clock::now();

    const auto totalEnd = Clock::now();

    BenchmarkResult result{};
    result.fileName = fileName;
    result.readMs = toMilliseconds(readEnd - readStart);
    result.sortMs = toMilliseconds(sortEnd - sortStart);
    result.splitMs = toMilliseconds(splitEnd - splitStart);
    result.writeMs = toMilliseconds(writeEnd - writeStart);
    result.totalMs = toMilliseconds(totalEnd - totalStart);

    return result;
}

void benchmarkAllFiles() {
    std::filesystem::create_directories("benchmark");

    std::ofstream report("benchmark/vector_benchmark.txt");
    if (!report.is_open()) {
        throw FileOpenException("Could not create benchmark/vector_benchmark.txt");
    }

    report << std::left << std::setw(24) << "File"
           << std::right << std::setw(12) << "Read(ms)"
           << std::setw(12) << "Sort(ms)"
           << std::setw(12) << "Split(ms)"
           << std::setw(12) << "Write(ms)"
           << std::setw(12) << "Total(ms)" << '\n';

    report << std::string(84, '-') << '\n';

    std::cout << '\n';
    std::cout << std::left << std::setw(24) << "File"
              << std::right << std::setw(12) << "Read(ms)"
              << std::setw(12) << "Sort(ms)"
              << std::setw(12) << "Split(ms)"
              << std::setw(12) << "Write(ms)"
              << std::setw(12) << "Total(ms)" << '\n';

    std::cout << std::string(84, '-') << '\n';

    for (const std::string& fileName : benchmarkFiles()) {
        BenchmarkResult result = benchmarkOneFile(fileName);

        report << std::left << std::setw(24) << result.fileName
               << std::right << std::setw(12) << std::fixed << std::setprecision(2) << result.readMs
               << std::setw(12) << result.sortMs
               << std::setw(12) << result.splitMs
               << std::setw(12) << result.writeMs
               << std::setw(12) << result.totalMs << '\n';

        std::cout << std::left << std::setw(24) << result.fileName
                  << std::right << std::setw(12) << std::fixed << std::setprecision(2) << result.readMs
                  << std::setw(12) << result.sortMs
                  << std::setw(12) << result.splitMs
                  << std::setw(12) << result.writeMs
                  << std::setw(12) << result.totalMs << '\n';
    }

    std::cout << "\nBenchmark results saved to benchmark/vector_benchmark.txt\n";
}