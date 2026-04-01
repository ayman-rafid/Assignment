#include "ContainerBenchmark.h"

#include <algorithm>
#include <chrono>
#include <cctype>
#include <deque>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
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

std::string strategyFileTag(SplitStrategy strategy) {
    if (strategy == SplitStrategy::Strategy1) {
        return "s1";
    }
    return "s2";
}

std::string buildOutputFileName(
    const std::string& containerName,
    const std::string& groupName,
    const std::string& sizeLabel,
    SplitStrategy strategy
) {
    return "output/" + containerName + "_" + strategyFileTag(strategy) + "_" + groupName + "_" + sizeLabel + ".txt";
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

void sortContainer(std::vector<Person>& students) {
    std::sort(students.begin(), students.end(), compareBySurname);
}

void sortContainer(std::deque<Person>& students) {
    std::sort(students.begin(), students.end(), compareBySurname);
}

void sortContainer(std::list<Person>& students) {
    students.sort(compareBySurname);
}

void shrinkContainer(std::vector<Person>& students) {
    students.shrink_to_fit();
}

void shrinkContainer(std::deque<Person>& students) {
    students.shrink_to_fit();
}

template <typename Container>
void splitStrategy1(const Container& students, Container& failed, Container& passed) {
    failed.clear();
    passed.clear();

    for (const auto& student : students) {
        if (student.finalGrade() < 5.0) {
            failed.emplace_back(student);
        }
        else {
            passed.emplace_back(student);
        }
    }
}

void splitStrategy2(std::vector<Person>& students, std::vector<Person>& failed) {
    failed.clear();

    auto boundary = std::stable_partition(
        students.begin(),
        students.end(),
        [](const Person& student) {
            return student.finalGrade() >= 5.0;
        }
    );

    std::copy(boundary, students.end(), std::back_inserter(failed));
    students.erase(boundary, students.end());
    shrinkContainer(students);
}

void splitStrategy2(std::deque<Person>& students, std::deque<Person>& failed) {
    failed.clear();

    auto boundary = std::stable_partition(
        students.begin(),
        students.end(),
        [](const Person& student) {
            return student.finalGrade() >= 5.0;
        }
    );

    std::copy(boundary, students.end(), std::back_inserter(failed));
    students.erase(boundary, students.end());
    shrinkContainer(students);
}

void splitStrategy2(std::list<Person>& students, std::list<Person>& failed) {
    failed.clear();

    auto boundary = std::stable_partition(
        students.begin(),
        students.end(),
        [](const Person& student) {
            return student.finalGrade() >= 5.0;
        }
    );

    failed.splice(failed.end(), students, boundary, students.end());
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

template <typename Container>
BenchmarkResult benchmarkContainerFile(
    const std::string& fileName,
    const std::string& containerName,
    SplitStrategy strategy
) {
    const auto totalStart = Clock::now();

    const auto readStart = Clock::now();
    Container students = loadContainerFromFile<Container>(fileName);
    const auto readEnd = Clock::now();

    calculateFinalGrades(students, CalculationMethod::Average);

    const auto sortStart = Clock::now();
    sortContainer(students);
    const auto sortEnd = Clock::now();

    Container failed;
    Container passed;

    const auto splitStart = Clock::now();
    if (strategy == SplitStrategy::Strategy1) {
        splitStrategy1(students, failed, passed);
    }
    else {
        splitStrategy2(students, failed);
    }
    const auto splitEnd = Clock::now();

    const std::string sizeLabel = extractSizeLabel(fileName);
    const std::string failedFile = buildOutputFileName(containerName, "failed", sizeLabel, strategy);
    const std::string passedFile = buildOutputFileName(containerName, "passed", sizeLabel, strategy);

    const auto writeStart = Clock::now();
    if (strategy == SplitStrategy::Strategy1) {
        writeContainerToFile(failedFile, failed, CalculationMethod::Average);
        writeContainerToFile(passedFile, passed, CalculationMethod::Average);
    }
    else {
        writeContainerToFile(failedFile, failed, CalculationMethod::Average);
        writeContainerToFile(passedFile, students, CalculationMethod::Average);
    }
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
        BenchmarkResult vectorResult =
            benchmarkContainerFile<std::vector<Person>>(fileName, "vector", strategy);
        BenchmarkResult dequeResult =
            benchmarkContainerFile<std::deque<Person>>(fileName, "deque", strategy);
        BenchmarkResult listResult =
            benchmarkContainerFile<std::list<Person>>(fileName, "list", strategy);

        writeRow(report, "vector", vectorResult);
        writeRow(report, "deque", dequeResult);
        writeRow(report, "list", listResult);

        writeRow(std::cout, "vector", vectorResult);
        writeRow(std::cout, "deque", dequeResult);
        writeRow(std::cout, "list", listResult);
    }

    std::cout << "\nAll container benchmark results saved to benchmark/all_containers_benchmark.txt\n";
}