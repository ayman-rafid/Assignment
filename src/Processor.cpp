#include "Processor.h"

#include <algorithm>
#include <iomanip>
#include <iostream>

void sortStudents(std::vector<Person>& students, SortField field) {
    std::sort(students.begin(), students.end(),
              [field](const Person& a, const Person& b) {
                  if (field == SortField::Name) {
                      if (a.firstName() == b.firstName()) {
                          return a.surname() < b.surname();
                      }
                      return a.firstName() < b.firstName();
                  }

                  if (a.surname() == b.surname()) {
                      return a.firstName() < b.firstName();
                  }

                  return a.surname() < b.surname();
              });
}

void printSingleMethodTable(const std::vector<Person>& students, CalculationMethod method) {
    const std::string methodTitle =
        (method == CalculationMethod::Average)
            ? "Final_Point(Aver.)"
            : "Final_Point(Med.)";

    std::cout << "\n"
              << std::left << std::setw(12) << "Name"
              << std::setw(15) << "Surname"
              << std::right << std::setw(20) << methodTitle << '\n';

    std::cout << "-----------------------------------------------\n";

    for (const Person& student : students) {
        std::cout << student << '\n';
    }
}

void printAvgMedTable(const std::vector<Person>& students) {
    std::cout << "\n"
              << std::left << std::setw(12) << "Name"
              << std::setw(15) << "Surname"
              << std::right << std::setw(15) << "Final (Avg.)"
              << " | "
              << std::setw(12) << "Final (Med.)" << '\n';

    std::cout << "--------------------------------------------------------------\n";

    for (const Person& student : students) {
        std::cout << std::left << std::setw(12) << student.firstName()
                  << std::setw(15) << student.surname()
                  << std::right << std::setw(15) << std::fixed << std::setprecision(2)
                  << student.finalByAverage()
                  << " | "
                  << std::setw(12) << std::fixed << std::setprecision(2)
                  << student.finalByMedian()
                  << '\n';
    }
}