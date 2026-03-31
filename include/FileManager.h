#pragma once

#include <string>
#include <vector>

#include "Person.h"
#include "Types.h"

std::vector<Person> loadStudentsFromFile(const std::string& fileName);
bool saveStudentsToFile(const std::string& fileName, const std::vector<Person>& students);
void writeStudentsToFile(const std::string& fileName,
                         const std::vector<Person>& students,
                         CalculationMethod method);