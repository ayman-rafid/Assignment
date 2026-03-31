#include <iostream>
#include <string>
#include <vector>

#include "FileManager.h"
#include "Input.h"
#include "Processor.h"

int main() {
    const DataSource source = readDataSource();

    if (source == DataSource::File) {
        const std::string fileName = "Students.txt";
        std::vector<Person> students = loadStudentsFromFile(fileName);

        if (students.empty()) {
            std::cout << "No valid student records were loaded from " << fileName << ".\n";
            return 1;
        }

        const SortField sortField = readSortField();
        sortStudents(students, sortField);
        printAvgMedTable(students);
        return 0;
    }

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
    return 0;
}