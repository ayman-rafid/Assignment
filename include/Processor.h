#pragma once

#include <string>
#include <vector>

#include "Person.h"
#include "Types.h"

void sortStudents(std::vector<Person>& students, SortField field);
void splitStudents(const std::vector<Person>& all,
                   std::vector<Person>& failed,
                   std::vector<Person>& passed,
                   CalculationMethod method);
std::string formatSingleMethodTable(const std::vector<Person>& students, CalculationMethod method);
std::string formatAvgMedTable(const std::vector<Person>& students);
void printSingleMethodTable(const std::vector<Person>& students, CalculationMethod method);
void printAvgMedTable(const std::vector<Person>& students);
