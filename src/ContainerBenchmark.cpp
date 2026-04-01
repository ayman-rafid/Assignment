#include "ContainerBenchmark.h"

#include <algorithm>
#include <chrono>
#include <cctype>
#include <deque>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <list>
#include <sstream>
#include <string>
#include <vector>

#include "Benchmark.h"
#include "Exceptions.h"
#include "Person.h"
#include "Types.h"

namespace {
using Clock = std::chrono::steady_clock;

double toMilliseconds(const Clock::duration& duration) {
    return std::chrono::duration<double, std::milli>(duration).count();
}

bool compareBySurname(const Person& a, const Person& b) {
    if (a.surname() == b.surname()) {
        return a.firstName() < b.firstName();
    }
    return a.surname() < b.surname();
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

std::string strategyToString(SplitStrategy strategy) {
    if (strategy == SplitStrategy::Strategy1) {
        return "Strategy 1";
    }
    return "Strategy 2";
}

template <typename Container>
Container loadContainerFromFile(const std::string& fileName) {
    std::ifstream file(fileName);
    Container students;

    if (!file.is_open()) {
        throw FileOpenException("Could not open file: " + fileName);
    }

    std::string line;
    if (!std::getline(file, line)) {
        throw ValidationException("File is empty: " + fileName);
    }

    int lineNumber = 1;

    while (std::getline(file, line)) {
        ++lineNumber;

        if (line.empty()) {
            continue;
        }

        std::istringstream row(line);
        std::string name;
        std::string surname;

        if (!(row >> name >> surname)) {
            throw ValidationException("Invalid name/surname at line " + std::to_string(lineNumber));
        }

        std::vector<int> scores;
        int score = 0;

        while (row >> score) {
            if (score < 1 || score > 10) {
                throw ValidationException(
                    "Score out of range at line " + std::to_string(lineNumber) +
                    ". Allowed values are 1..10."
                );
            }
            scores.push_back(score);
        }

        if (scores.size() < 2) {
            throw ValidationException(
                "Not enough grade data at line " + std::to_string(lineNumber) +
                ". Need at least one homework score and one exam score."
            );
        }

        const int exam = scores.back();
        scores.pop_back();

        students.emplace_back(name, surname, scores, exam);
    }

    return students;
}

template <typename Container>
void calculateFinalGrades(Container& students, CalculationMethod method) {
    for (auto& student : students) {
        student.calculateFinalGrade(method);
    }
}

template <typename Container>
void splitContainer(const Container& all, Container& failed, Container& passed, CalculationMethod method) {
    failed.clear();
    passed.clear();

    for (const auto& original : all) {
        Person student = original;
        student.calculateFinalGrade(method);

        if (student.finalGrade() < 5.0) {
            failed.emplace_back(student);
        }
        else {
            passed.emplace_back(student);
        }
    }
}

template <typename Container>
void writeContainerToFile(const std::string& fileName, const Container& students, CalculationMethod method) {
    std::filesystem::path path(fileName);
    if (path.has_parent_path()) {
        std::filesystem::create_directories(path.parent_path());
    }

    std::ofstream file(fileName);
    if (!file.is_open()) {
        throw FileOpenException("Could not create file: " + fileName);
    }

    const std::string title = (method == CalculationMethod::Average) ? "Final (Avg.)" : "Final (Med.)";

    file << std::left << std::setw(20) << "Name"
         << std::setw(20) << "Surname"
         << std::right << std::setw(15) << title << '\n';

    file << std::string(55, '-') << '\n';

    for (const auto& student : students) {
        const double finalValue =
            (method == CalculationMethod::Average) ? student.finalByAverage() : student.finalByMedian();

        file << std::left << std::setw(20) << student.firstName()
             << std::setw(20) << student.surname()
             << std::right << std::setw(15) << std::fixed << std::setprecision(2) << finalValue << '\n';
    }
}

BenchmarkResult benchmarkDequeFile(const std::string& fileName) {
    const auto totalStart = Clock::now();

    const auto readStart = Clock::now();
    std::deque<Person> students = loadContainerFromFile<std::deque<Person>>(fileName);
    const auto readEnd = Clock::now();

    calculateFinalGrades(students, CalculationMethod::Average);

    const auto sortStart = Clock::now();
    std::sort(students.begin(), students.end(), compareBySurname);
    const auto sortEnd = Clock::now();

    std::deque<Person> failed;
    std::deque<Person> passed;

    const auto splitStart = Clock::now();
    splitContainer(students, failed, passed, CalculationMethod::Average);
    const auto splitEnd = Clock::now();

    const std::string sizeLabel = extractSizeLabel(fileName);
    const std::string failedFile = "output/deque_failed_" + sizeLabel + ".txt";
    const std::string passedFile = "output/deque_passed_" + sizeLabel + ".txt";

    const auto writeStart = Clock::now();
    writeContainerToFile(failedFile, failed, CalculationMethod::Average);
    writeContainerToFile(passedFile, passed, CalculationMethod::Average);
    const auto writeEnd = Clock::now();

    const auto totalEnd = Clock::now();

    return {
        fileName,
        toMilliseconds(readEnd - readStart),
        toMilliseconds(sortEnd - sortStart),
        toMilliseconds(splitEnd - splitStart),
        toMilliseconds(writeEnd - writeStart),
        toMilliseconds(totalEnd - totalStart)
    };
}

BenchmarkResult benchmarkListFile(const std::string& fileName) {
    const auto totalStart = Clock::now();

    const auto readStart = Clock::now();
    std::list<Person> students = loadContainerFromFile<std::list<Person>>(fileName);
    const auto readEnd = Clock::now();

    calculateFinalGrades(students, CalculationMethod::Average);

    const auto sortStart = Clock::now();
    students.sort(compareBySurname);
    const auto sortEnd = Clock::now();

    std::list<Person> failed;
    std::list<Person> passed;

    const auto splitStart = Clock::now();
    splitContainer(students, failed, passed, CalculationMethod::Average);
    const auto splitEnd = Clock::now();

    const std::string sizeLabel = extractSizeLabel(fileName);
    const std::string failedFile = "output/list_failed_" + sizeLabel + ".txt";
    const std::string passedFile = "output/list_passed_" + sizeLabel + ".txt";

    const auto writeStart = Clock::now();
    writeContainerToFile(failedFile, failed, CalculationMethod::Average);
    writeContainerToFile(passedFile, passed, CalculationMethod::Average);
    const auto writeEnd = Clock::now();

    const auto totalEnd = Clock::now();

    return {
        fileName,
        toMilliseconds(readEnd - readStart),
        toMilliseconds(sortEnd - sortStart),
        toMilliseconds(splitEnd - splitStart),
        toMilliseconds(writeEnd - writeStart),
        toMilliseconds(totalEnd - totalStart)
    };
}

void writeRow(std::ostream& out, const std::string& containerName, const BenchmarkResult& result) {
    out << std::left << std::setw(10) << containerName
        << std::setw(24) << result.fileName
        << std::right << std::setw(12) << std::fixed << std::setprecision(2) << result.readMs
        << std::setw(12) << result.sortMs
        << std::setw(12) << result.splitMs
        << std::setw(12) << result.writeMs
        << std::setw(12) << result.totalMs << '\n';
}
}

void benchmarkAllContainers(SplitStrategy strategy) {
    std::filesystem::create_directories("benchmark");

    std::ofstream report("benchmark/all_containers_benchmark.txt");
    if (!report.is_open()) {
        throw FileOpenException("Could not create benchmark/all_containers_benchmark.txt");
    }

    report << "Selected split strategy: " << strategyToString(strategy) << "\n\n";

    report << std::left << std::setw(10) << "Container"
           << std::setw(24) << "File"
           << std::right << std::setw(12) << "Read(ms)"
           << std::setw(12) << "Sort(ms)"
           << std::setw(12) << "Split(ms)"
           << std::setw(12) << "Write(ms)"
           << std::setw(12) << "Total(ms)" << '\n';

    report << std::string(94, '-') << '\n';

    std::cout << "\nSelected split strategy: " << strategyToString(strategy) << "\n\n";

    std::cout << std::left << std::setw(10) << "Container"
              << std::setw(24) << "File"
              << std::right << std::setw(12) << "Read(ms)"
              << std::setw(12) << "Sort(ms)"
              << std::setw(12) << "Split(ms)"
              << std::setw(12) << "Write(ms)"
              << std::setw(12) << "Total(ms)" << '\n';

    std::cout << std::string(94, '-') << '\n';

    for (const std::string& fileName : benchmarkFiles()) {
        BenchmarkResult vectorResult = benchmarkOneFile(fileName);
        BenchmarkResult dequeResult = benchmarkDequeFile(fileName);
        BenchmarkResult listResult = benchmarkListFile(fileName);

        writeRow(report, "vector", vectorResult);
        writeRow(report, "deque", dequeResult);
        writeRow(report, "list", listResult);

        writeRow(std::cout, "vector", vectorResult);
        writeRow(std::cout, "deque", dequeResult);
        writeRow(std::cout, "list", listResult);
    }

    std::cout << "\nAll container benchmark results saved to benchmark/all_containers_benchmark.txt\n";
}