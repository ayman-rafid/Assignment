#include "FileManager.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

std::vector<Person> loadStudentsFromFile(const std::string& fileName) {
    std::ifstream file(fileName);
    std::vector<Person> students;

    if (!file.is_open()) {
        std::cout << "Could not open file: " << fileName << '\n';
        return students;
    }

    std::string line;
    std::getline(file, line);  // skip header

    while (std::getline(file, line)) {
        if (line.empty()) {
            continue;
        }

        std::istringstream row(line);

        std::string name;
        std::string surname;

        if (!(row >> name >> surname)) {
            continue;
        }

        std::vector<int> scores;
        int score = 0;

        while (row >> score) {
            scores.push_back(score);
        }

        if (scores.size() < 2) {
            continue;
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