#pragma once

#include <vector>

#include "Person.h"
#include "Types.h"

void sortStudents(std::vector<Person>& students, SortField field);
void splitStudents(const std::vector<Person>& all,
                   std::vector<Person>& failed,
                   std::vector<Person>& passed,
                   CalculationMethod method);
void printSingleMethodTable(const std::vector<Person>& students, CalculationMethod method);
void printAvgMedTable(const std::vector<Person>& students);