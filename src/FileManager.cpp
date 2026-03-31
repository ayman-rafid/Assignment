#include "FileManager.h"

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>

#include "Exceptions.h"

std::vector<Person> loadStudentsFromFile(const std::string& fileName) {
    std::ifstream file(fileName);
    std::vector<Person> students;

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

bool saveStudentsToFile(const std::string& fileName, const std::vector<Person>& students) {
    std::ofstream file(fileName);

    if (!file.is_open()) {
        return false;
    }

    std::size_t maxHomeworkCount = 0;
    for (const Person& student : students) {
        maxHomeworkCount = std::max(maxHomeworkCount, student.homework().size());
    }

    file << "Name Surname";
    for (std::size_t i = 0; i < maxHomeworkCount; ++i) {
        file << " HW" << (i + 1);
    }
    file << " Exam\n";

    for (const Person& student : students) {
        file << student.firstName() << ' ' << student.surname();

        for (int score : student.homework()) {
            file << ' ' << score;
        }

        file << ' ' << student.examResult() << '\n';
    }

    return true;
}