# Assignment v0.2

This project is a C++ student grade calculator based on `std::vector`.

## Features

- `Person` class with:
  - constructor
  - copy constructor
  - copy assignment operator
  - destructor
  - overloaded `>>`
  - overloaded `<<`
- final grade calculation using:
  - average
  - median
- interactive student input
- random homework and exam generation
- reading student data from `Students.txt`
- sorting by name or surname
- exception handling for file and validation errors
- generation of benchmark files:
  - 1,000
  - 10,000
  - 100,000
  - 1,000,000
  - 10,000,000
- splitting students into:
  - passed (`>= 5.0`)
  - failed (`< 5.0`)
- timing analysis for:
  - file reading
  - sorting
  - splitting
  - writing output files
  - total runtime

## Project Structure

- `include/` - header files
- `src/` - source files
- `data/` - generated input files
- `output/` - generated passed/failed files
- `benchmark/` - benchmark result files

## Build

Compile from project root:

g++ -std=c++17 -O2 -Wall -Wextra -Iinclude src/*.cpp -o grade_calc.exe