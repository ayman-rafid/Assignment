#pragma once

#include <iosfwd>
#include <string>
#include <vector>

#include "Person.h"
#include "Types.h"

std::vector<Person> loadStudentsFromFile(const std::string& fileName);
std::vector<Person> loadStudentsFromStream(std::istream& input, const std::string& sourceName);
bool saveStudentsToFile(const std::string& fileName, const std::vector<Person>& students);
void saveStudentsToStream(std::ostream& output, const std::vector<Person>& students);
void writeStudentsToFile(const std::string& fileName,
                         const std::vector<Person>& students,
                         CalculationMethod method);
