#include "Generator.h"

#include <filesystem>
#include <fstream>
#include <random>
#include <string>
#include <vector>

#include "Exceptions.h"

namespace {
std::vector<std::size_t> benchmarkSizes() {
    return {1000, 10000, 100000, 1000000, 10000000};
}
}

void generateStudentFile(const std::string& fileName, std::size_t count, int homeworkCount) {
    std::filesystem::path path(fileName);
    if (path.has_parent_path()) {
        std::filesystem::create_directories(path.parent_path());
    }

    std::ofstream file(fileName);
    if (!file.is_open()) {
        throw FileOpenException("Could not create file: " + fileName);
    }

    file << "Name Surname";
    for (int i = 1; i <= homeworkCount; ++i) {
        file << " HW" << i;
    }
    file << " Exam\n";

    std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> scoreDistribution(1, 10);

    for (std::size_t i = 1; i <= count; ++i) {
        file << "Name" << i << ' ' << "Surname" << i;

        for (int hw = 0; hw < homeworkCount; ++hw) {
            file << ' ' << scoreDistribution(rng);
        }

        file << ' ' << scoreDistribution(rng) << '\n';
    }
}

void generateAllBenchmarkFiles() {
    for (std::size_t size : benchmarkSizes()) {
        const std::string fileName = "data/students" + std::to_string(size) + ".txt";
        generateStudentFile(fileName, size);
    }
}