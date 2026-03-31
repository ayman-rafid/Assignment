#include "Person.h"

#include <algorithm>
#include <iomanip>
#include <iostream>

Person::Person()
    : firstName_(), surname_(), homework_(), examResult_(0), finalGrade_(0.0) {
}

Person::Person(const std::string& firstName,
               const std::string& surname,
               const std::vector<int>& homework,
               int examResult)
    : firstName_(firstName),
      surname_(surname),
      homework_(homework),
      examResult_(examResult),
      finalGrade_(0.0) {
}

Person::Person(const Person& other)
    : firstName_(other.firstName_),
      surname_(other.surname_),
      homework_(other.homework_),
      examResult_(other.examResult_),
      finalGrade_(other.finalGrade_) {
}

Person& Person::operator=(const Person& other) {
    if (this != &other) {
        firstName_ = other.firstName_;
        surname_ = other.surname_;
        homework_ = other.homework_;
        examResult_ = other.examResult_;
        finalGrade_ = other.finalGrade_;
    }
    return *this;
}

Person::~Person() = default;

void Person::calculateFinalGrade(CalculationMethod method) {
    finalGrade_ = (method == CalculationMethod::Average)
        ? finalByAverage()
        : finalByMedian();
}

const std::string& Person::firstName() const {
    return firstName_;
}

const std::string& Person::surname() const {
    return surname_;
}

const std::vector<int>& Person::homework() const {
    return homework_;
}

int Person::examResult() const {
    return examResult_;
}

double Person::finalGrade() const {
    return finalGrade_;
}

double Person::finalByAverage() const {
    return 0.4 * homeworkAverage() + 0.6 * examResult_;
}

double Person::finalByMedian() const {
    return 0.4 * homeworkMedian() + 0.6 * examResult_;
}

double Person::homeworkAverage() const {
    if (homework_.empty()) {
        return 0.0;
    }

    double sum = 0.0;
    for (int score : homework_) {
        sum += score;
    }

    return sum / homework_.size();
}

double Person::homeworkMedian() const {
    if (homework_.empty()) {
        return 0.0;
    }

    std::vector<int> sortedHomework = homework_;
    std::sort(sortedHomework.begin(), sortedHomework.end());

    const std::size_t middle = sortedHomework.size() / 2;

    if (sortedHomework.size() % 2 == 0) {
        return (sortedHomework[middle - 1] + sortedHomework[middle]) / 2.0;
    }

    return static_cast<double>(sortedHomework[middle]);
}

std::istream& operator>>(std::istream& in, Person& person) {
    person.homework_.clear();

    std::cout << "Name and surname: ";
    in >> person.firstName_ >> person.surname_;

    std::cout << "Enter homework scores (1-10).\n";
    std::cout << "Type -1 to finish.\n";

    while (true) {
        int score = 0;
        std::cout << "HW: ";
        in >> score;

        if (!in) {
            return in;
        }

        if (score == -1) {
            if (person.homework_.empty()) {
                std::cout << "At least one homework score is required.\n";
                continue;
            }
            break;
        }

        if (score < 1 || score > 10) {
            std::cout << "Score must be between 1 and 10.\n";
            continue;
        }

        person.homework_.push_back(score);
    }

    std::cout << "Exam score (1-10): ";
    in >> person.examResult_;

    return in;
}

std::ostream& operator<<(std::ostream& out, const Person& person) {
    out << std::left << std::setw(12) << person.firstName_
        << std::setw(15) << person.surname_
        << std::right << std::setw(10) << std::fixed << std::setprecision(2)
        << person.finalGrade();

    return out;
}
