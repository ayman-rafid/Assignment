#include "Processor.h"

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <sstream>

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

void splitStudents(const std::vector<Person>& all,
                   std::vector<Person>& failed,
                   std::vector<Person>& passed,
                   CalculationMethod method) {
    failed.clear();
    passed.clear();

    for (const Person& original : all) {
        Person student = original;
        student.calculateFinalGrade(method);

        if (student.finalGrade() < 5.0) {
            failed.push_back(student);
        } else {
            passed.push_back(student);
        }
    }
}

std::string formatSingleMethodTable(const std::vector<Person>& students, CalculationMethod method) {
    const std::string methodTitle =
        (method == CalculationMethod::Average)
            ? "Final_Point(Aver.)"
            : "Final_Point(Med.)";

    std::ostringstream output;
    output << "\n"
           << std::left << std::setw(12) << "Name"
           << std::setw(15) << "Surname"
           << std::right << std::setw(20) << methodTitle << '\n';

    output << "-----------------------------------------------\n";

    for (const Person& student : students) {
        output << student << '\n';
    }

    return output.str();
}

std::string formatAvgMedTable(const std::vector<Person>& students) {
    std::ostringstream output;
    output << "\n"
           << std::left << std::setw(12) << "Name"
           << std::setw(15) << "Surname"
           << std::right << std::setw(15) << "Final (Avg.)"
           << " | "
           << std::setw(12) << "Final (Med.)" << '\n';

    output << "--------------------------------------------------------------\n";

    for (const Person& student : students) {
        output << std::left << std::setw(12) << student.firstName()
               << std::setw(15) << student.surname()
               << std::right << std::setw(15) << std::fixed << std::setprecision(2)
               << student.finalByAverage()
               << " | "
               << std::setw(12) << std::fixed << std::setprecision(2)
               << student.finalByMedian()
               << '\n';
    }

    return output.str();
}

void printSingleMethodTable(const std::vector<Person>& students, CalculationMethod method) {
    std::cout << formatSingleMethodTable(students, method);
}

void printAvgMedTable(const std::vector<Person>& students) {
    std::cout << formatAvgMedTable(students);
}
