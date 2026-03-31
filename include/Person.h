#pragma once

#include <iosfwd>
#include <string>
#include <vector>

#include "Types.h"

class Person {
public:
    Person();
    Person(const std::string& firstName,
           const std::string& surname,
           const std::vector<int>& homework,
           int examResult);

    Person(const Person& other);
    Person& operator=(const Person& other);
    ~Person();

    void calculateFinalGrade(CalculationMethod method);

    const std::string& firstName() const;
    const std::string& surname() const;
    const std::vector<int>& homework() const;
    int examResult() const;
    double finalGrade() const;

    double finalByAverage() const;
    double finalByMedian() const;

    friend std::istream& operator>>(std::istream& in, Person& person);
    friend std::ostream& operator<<(std::ostream& out, const Person& person);

private:
    double homeworkAverage() const;
    double homeworkMedian() const;

    std::string firstName_;
    std::string surname_;
    std::vector<int> homework_;
    int examResult_;
    double finalGrade_;
};
