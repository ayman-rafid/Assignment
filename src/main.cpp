#include <exception>
#include <iostream>
#include <string>
#include <vector>

#include "Benchmark.h"
#include "ContainerBenchmark.h"
#include "FileManager.h"
#include "Generator.h"
#include "Input.h"
#include "Processor.h"

namespace {
SplitStrategy readSplitStrategyChoice() {
    while (true) {
        std::cout << "\nChoose split strategy:\n";
        std::cout << "1. Strategy 1 - keep original students, create failed and passed containers\n";
        std::cout << "2. Strategy 2 - move failed students out, keep passed students in base container\n";
        std::cout << "Enter choice (1-2): ";

        int choice = 0;
        if (std::cin >> choice) {
            if (choice == 1) {
                return SplitStrategy::Strategy1;
            }
            if (choice == 2) {
                return SplitStrategy::Strategy2;
            }
        }

        std::cout << "Invalid choice. Please enter 1 or 2.\n";
        std::cin.clear();
        std::cin.ignore(10000, '\n');
    }
}
}

int main() {
    try {
        while (true) {
            const int choice = readMainMenuChoice();

            if (choice == 1) {
                std::vector<Person> students = collectStudentsInteractive();

                const std::string fileName = "Students.txt";
                if (!saveStudentsToFile(fileName, students)) {
                    std::cout << "Could not create file: " << fileName << '\n';
                    return 1;
                }

                std::cout << "\nSaved file: " << fileName << '\n';

                const CalculationMethod method = readMethodChoice();
                for (Person& student : students) {
                    student.calculateFinalGrade(method);
                }

                printSingleMethodTable(students, method);
            }
            else if (choice == 2) {
                const std::string fileName = "Students.txt";
                std::vector<Person> students = loadStudentsFromFile(fileName);

                if (students.empty()) {
                    std::cout << "No valid student records were loaded from " << fileName << ".\n";
                    continue;
                }

                const SortField sortField = readSortField();
                sortStudents(students, sortField);
                printAvgMedTable(students);
            }
            else if (choice == 3) {
                std::cout << "\nGenerating benchmark files...\n";
                generateAllBenchmarkFiles();
                std::cout << "Done.\nFiles created in data/\n";
            }
            else if (choice == 4) {
                std::cout << "\nRunning vector benchmark...\n";
                benchmarkAllFiles();
            }
            else if (choice == 5) {
                const SplitStrategy strategy = readSplitStrategyChoice();

                std::cout << "\nRunning all container benchmarks...\n";
                benchmarkAllContainers(strategy);
            }
            else if (choice == 6) {
                std::cout << "Goodbye!\n";
                return 0;
            }
        }
    }
    catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << '\n';
        return 1;
    }
}