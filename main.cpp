#include <array>
#include <iomanip>
#include <iostream>
#include <limits>
#include <string>
#include <vector>

class Person {
public:
    Person()
        : firstName_(), surname_(), homework_{0, 0, 0, 0, 0}, examResult_(0), finalGrade_(0.0) {}

    Person(const std::string& firstName, const std::string& surname, const std::array<int, 5>& homework, int examResult)
        : firstName_(firstName), surname_(surname), homework_(homework), examResult_(examResult), finalGrade_(0.0) {}

    Person(const Person& other)
        : firstName_(other.firstName_),
          surname_(other.surname_),
          homework_(other.homework_),
          examResult_(other.examResult_),
          finalGrade_(other.finalGrade_) {}

    Person& operator=(const Person& other) {
        if (this != &other) {
            firstName_ = other.firstName_;
            surname_ = other.surname_;
            homework_ = other.homework_;
            examResult_ = other.examResult_;
            finalGrade_ = other.finalGrade_;
        }
        return *this;
    }

    ~Person() = default;

    void calculateFinalAverage() {
        double sum = 0.0;
        for (int score : homework_) {
            sum += score;
        }
        const double average = sum / homework_.size();
        finalGrade_ = 0.4 * average + 0.6 * examResult_;
    }

    const std::string& firstName() const { return firstName_; }
    const std::string& surname() const { return surname_; }
    double finalGrade() const { return finalGrade_; }

    friend std::istream& operator>>(std::istream& in, Person& person);
    friend std::ostream& operator<<(std::ostream& out, const Person& person);

private:
    std::string firstName_;
    std::string surname_;
    std::array<int, 5> homework_;
    int examResult_;
    double finalGrade_;
};

std::istream& operator>>(std::istream& in, Person& person) {
    in >> person.firstName_ >> person.surname_;
    for (std::size_t i = 0; i < person.homework_.size(); ++i) {
        in >> person.homework_[i];
    }
    in >> person.examResult_;
    return in;
}

std::ostream& operator<<(std::ostream& out, const Person& person) {
    out << std::left << std::setw(12) << person.firstName_ << std::setw(15) << person.surname_ << std::right
        << std::setw(10) << std::fixed << std::setprecision(2) << person.finalGrade_;
    return out;
}

int readPositiveInt(const std::string& prompt) {
    int value = 0;
    while (true) {
        std::cout << prompt;
        if (std::cin >> value && value > 0) {
            return value;
        }
        std::cout << "Please enter a positive integer.\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

int main() {
    const int studentCount = readPositiveInt("How many students will you enter? ");
    std::vector<Person> students(studentCount);

    for (int i = 0; i < studentCount; ++i) {
        std::cout << "\nEnter student " << (i + 1)
                  << " data in format: Name Surname HW1 HW2 HW3 HW4 HW5 Exam\n> ";
        std::cin >> students[i];
        students[i].calculateFinalAverage();
    }

    std::cout << "\n"
              << std::left << std::setw(12) << "Name" << std::setw(15) << "Surname" << std::right << std::setw(20)
              << "Final_Point(Aver.)\n";
    std::cout << "-----------------------------------------------\n";

    for (const Person& student : students) {
        std::cout << student << '\n';
    }

    return 0;
}
