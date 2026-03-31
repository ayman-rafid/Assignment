#pragma once

#include <random>
#include <string>
#include <vector>

#include "Person.h"
#include "Types.h"

int readPositiveInt(const std::string& prompt);
char readChoiceChar(const std::string& prompt);

DataSource readDataSource();
InputMode readInputMode();
CalculationMethod readMethodChoice();
SortField readSortField();

Person createRandomPerson(std::mt19937& rng);
std::vector<Person> collectStudentsInteractive();
