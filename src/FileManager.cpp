#include "FileManager.h"

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <istream>
#include <sstream>
#include <string>
#include <vector>

#include "Exceptions.h"

std::vector<Person> loadStudentsFromFile(const std::string& fileName) {
    std::ifstream file(fileName);

    if (!file.is_open()) {
        throw FileOpenException("Could not open file: " + fileName);
    }

    return loadStudentsFromStream(file, fileName);
}

std::vector<Person> loadStudentsFromStream(std::istream& input, const std::string& sourceName) {
    std::vector<Person> students;

    std::string line;
    if (!std::getline(input, line)) {
        throw ValidationException("File is empty: " + sourceName);
    }

    int lineNumber = 1;

    while (std::getline(input, line)) {
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

bool saveStudentsToFile(const std::string& fileName, const std::vector<Person>& students) {
    std::ofstream file(fileName);

    if (!file.is_open()) {
        return false;
    }

    saveStudentsToStream(file, students);
    return true;
}

void saveStudentsToStream(std::ostream& output, const std::vector<Person>& students) {
    std::size_t maxHomeworkCount = 0;
    for (const Person& student : students) {
        maxHomeworkCount = std::max(maxHomeworkCount, student.homework().size());
    }

    output << "Name Surname";
    for (std::size_t i = 0; i < maxHomeworkCount; ++i) {
        output << " HW" << (i + 1);
    }
    output << " Exam\n";

    for (const Person& student : students) {
        output << student.firstName() << ' ' << student.surname();

        for (int score : student.homework()) {
            output << ' ' << score;
        }

        output << ' ' << student.examResult() << '\n';
    }
}

void writeStudentsToFile(const std::string& fileName,
                         const std::vector<Person>& students,
                         CalculationMethod method) {
    std::filesystem::path path(fileName);
    if (path.has_parent_path()) {
        std::filesystem::create_directories(path.parent_path());
    }

    std::ofstream file(fileName);
    if (!file.is_open()) {
        throw FileOpenException("Could not create file: " + fileName);
    }

    const std::string title =
        (method == CalculationMethod::Average) ? "Final (Avg.)" : "Final (Med.)";

    file << std::left << std::setw(20) << "Name"
         << std::setw(20) << "Surname"
         << std::right << std::setw(15) << title << '\n';

    file << std::string(55, '-') << '\n';

    for (const Person& student : students) {
        const double finalValue =
            (method == CalculationMethod::Average)
                ? student.finalByAverage()
                : student.finalByMedian();

        file << std::left << std::setw(20) << student.firstName()
             << std::setw(20) << student.surname()
             << std::right << std::setw(15) << std::fixed << std::setprecision(2)
             << finalValue << '\n';
    }
}
