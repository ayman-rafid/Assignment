#include <algorithm>
#include <cctype>
#include <iomanip>
#include <iostream>
#include <limits>
#include <random>
#include <string>
#include <vector>

enum class CalculationMethod { Average, Median };
enum class InputMode { Manual, Random };

class Person {
public:
    Person() : firstName_(), surname_(), homework_(), examResult_(0), finalGrade_(0.0) {}

    Person(const std::string& firstName, const std::string& surname, const std::vector<int>& homework, int examResult)
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

    void calculateFinalGrade(CalculationMethod method) {
        const double homeworkComponent = (method == CalculationMethod::Average) ? homeworkAverage() : homeworkMedian();
        finalGrade_ = 0.4 * homeworkComponent + 0.6 * examResult_;
    }

    const std::string& firstName() const { return firstName_; }
    const std::string& surname() const { return surname_; }
    double finalGrade() const { return finalGrade_; }

    friend std::istream& operator>>(std::istream& in, Person& person);
    friend std::ostream& operator<<(std::ostream& out, const Person& person);

private:
    double homeworkAverage() const {
        if (homework_.empty()) {
            return 0.0;
        }
        double sum = 0.0;
        for (int score : homework_) {
            sum += score;
        }
        return sum / homework_.size();
    }

    double homeworkMedian() const {
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

    std::string firstName_;
    std::string surname_;
    std::vector<int> homework_;
    int examResult_;
    double finalGrade_;
};

std::istream& operator>>(std::istream& in, Person& person) {
    person.homework_.clear();

    std::cout << "Name and surname: ";
    in >> person.firstName_ >> person.surname_;

    std::cout << "Enter homework scores (1-10). Type -1 to finish.\n";
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

InputMode readInputMode() {
    char choice = '\0';
    while (true) {
        std::cout << "Choose input mode - manual (M) or random (R): ";
        if (std::cin >> choice) {
            choice = static_cast<char>(std::toupper(static_cast<unsigned char>(choice)));
            if (choice == 'M') {
                return InputMode::Manual;
            }
            if (choice == 'R') {
                return InputMode::Random;
            }
        }
        std::cout << "Please enter M or R.\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

CalculationMethod readMethodChoice() {
    char choice = '\0';
    while (true) {
        std::cout << "Choose final grade method - average (A) or median (M): ";
        if (std::cin >> choice) {
            choice = static_cast<char>(std::toupper(static_cast<unsigned char>(choice)));
            if (choice == 'A') {
                return CalculationMethod::Average;
            }
            if (choice == 'M') {
                return CalculationMethod::Median;
            }
        }
        std::cout << "Please enter A or M.\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

Person createRandomPerson(std::mt19937& rng) {
    std::string name;
    std::string surname;

    std::cout << "Name and surname: ";
    std::cin >> name >> surname;

    const int homeworkCount = readPositiveInt("How many homework scores to generate? ");

    std::uniform_int_distribution<int> scoreDistribution(1, 10);
    std::vector<int> homework(homeworkCount);
    for (int& score : homework) {
        score = scoreDistribution(rng);
    }
    const int exam = scoreDistribution(rng);

    std::cout << "Generated homework: ";
    for (std::size_t i = 0; i < homework.size(); ++i) {
        std::cout << homework[i];
        if (i + 1 < homework.size()) {
            std::cout << ", ";
        }
    }
    std::cout << "\nGenerated exam: " << exam << '\n';

    return Person(name, surname, homework, exam);
}

int main() {
    const CalculationMethod method = readMethodChoice();
    const InputMode inputMode = readInputMode();
    const int studentCount = readPositiveInt("How many students will you enter? ");
    std::vector<Person> students(studentCount);
    std::random_device rd;
    std::mt19937 rng(rd());

    for (int i = 0; i < studentCount; ++i) {
        std::cout << "\nEntering student " << (i + 1) << ":\n";
        if (inputMode == InputMode::Manual) {
            std::cin >> students[i];
        } else {
            students[i] = createRandomPerson(rng);
        }
        students[i].calculateFinalGrade(method);
    }

    const std::string methodTitle =
        (method == CalculationMethod::Average) ? "Final_Point(Aver.)" : "Final_Point(Med.)";

    std::cout << "\n"
              << std::left << std::setw(12) << "Name" << std::setw(15) << "Surname" << std::right << std::setw(20)
              << methodTitle << '\n';
    std::cout << "-----------------------------------------------\n";

    for (const Person& student : students) {
        std::cout << student << '\n';
    }

    return 0;
}
