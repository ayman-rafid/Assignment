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
            } else if (choice == 2) {
                const std::string fileName = "Students.txt";
                std::vector<Person> students = loadStudentsFromFile(fileName);

                if (students.empty()) {
                    std::cout << "No valid student records were loaded from " << fileName << ".\n";
                    continue;
                }

                const SortField sortField = readSortField();
                sortStudents(students, sortField);
                printAvgMedTable(students);
            } else if (choice == 3) {
                std::cout << "\nGenerating benchmark files...\n";
                generateAllBenchmarkFiles();
                std::cout << "Done. Files created in data/\n";
            } else if (choice == 4) {
                std::cout << "\nRunning vector benchmark...\n";
                benchmarkAllFiles();
            } else if (choice == 5) {
                std::cout << "\nRunning all container benchmarks...\n";
                benchmarkAllContainers();
            } else if (choice == 6) {
                std::cout << "Goodbye!\n";
                return 0;
            }
        }
    } catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << '\n';
        return 1;
    }
}