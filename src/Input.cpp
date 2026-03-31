#include "Input.h"

#include <cctype>
#include <iostream>
#include <limits>

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

char readChoiceChar(const std::string& prompt) {
    char choice = '\0';
    std::cout << prompt;
    std::cin >> choice;
    return static_cast<char>(std::toupper(static_cast<unsigned char>(choice)));
}

int readMainMenuChoice() {
    while (true) {
        std::cout << "\n===== MENU =====\n";
        std::cout << "1. Interactive student input\n";
        std::cout << "2. Read Students.txt and print Avg/Med table\n";
        std::cout << "3. Generate benchmark data files\n";
        std::cout << "4. Run vector benchmark\n";
        std::cout << "5. Run vector + deque + list benchmarks\n";
        std::cout << "6. Exit\n";

        const int choice = readPositiveInt("Choose option (1-6): ");
        if (choice >= 1 && choice <= 6) {
            return choice;
        }

        std::cout << "Please choose a number from 1 to 6.\n";
    }
}

DataSource readDataSource() {
    while (true) {
        const char choice = readChoiceChar("Choose next mode - interactive (I) or file (F): ");

        if (choice == 'I') {
            return DataSource::Interactive;
        }

        if (choice == 'F') {
            return DataSource::File;
        }

        std::cout << "Please enter I or F.\n";
    }
}

InputMode readInputMode() {
    while (true) {
        const char choice = readChoiceChar("Choose input mode - manual (M) or random (R): ");

        if (choice == 'M') {
            return InputMode::Manual;
        }

        if (choice == 'R') {
            return InputMode::Random;
        }

        std::cout << "Please enter M or R.\n";
    }
}

CalculationMethod readMethodChoice() {
    while (true) {
        const char choice = readChoiceChar("Choose final grade method - average (A) or median (M): ");

        if (choice == 'A') {
            return CalculationMethod::Average;
        }

        if (choice == 'M') {
            return CalculationMethod::Median;
        }

        std::cout << "Please enter A or M.\n";
    }
}

SortField readSortField() {
    while (true) {
        const char choice = readChoiceChar("Sort by name (N) or surname (S): ");

        if (choice == 'N') {
            return SortField::Name;
        }

        if (choice == 'S') {
            return SortField::Surname;
        }

        std::cout << "Please enter N or S.\n";
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

std::vector<Person> collectStudentsInteractive() {
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
    }

    return students;
}