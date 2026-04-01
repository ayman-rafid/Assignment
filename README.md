# Student Grade Calculator and Benchmark Project

This project is a C++17 console application for entering student data, calculating final grades, generating benchmark files, and comparing the performance of different STL containers.

## Releases

### v0.1
- Basic student grade calculator
- Interactive input
- Final grade calculation using average or median
- File output support

### v0.2
- Project split into `include/` and `src/`
- Added benchmark file generation
- Added student splitting into two groups: failed and passed
- Added performance measurement for larger data files

### v0.25
- Added benchmarking for three container types:
  - `std::vector`
  - `std::deque`
  - `std::list`
- Added multi-container benchmark report
- Added current 6-option menu structure

### v1.0
- Added menu selection for split strategies
- Implemented two student split strategies for all three containers
- Compared container performance using:
  - Strategy 1: keep original students and create two new containers
  - Strategy 2: keep passed students in the base container and move failed students to one new container
- Refactored the fastest chosen path using `<algorithm>` library functions
- Added `Makefile` for Unix-like systems
- Added `CMakeLists.txt` for cross-platform builds
- Cleaned repository structure for submission

## Split Strategies

### Strategy 1
The original students container stays unchanged.  
Two new containers are created:
- `failed`
- `passed`

This is simple, but it uses more memory because the original data stays in place and extra containers are created.

### Strategy 2
Only one new container is created:
- `failed`

Students who fail are separated from the base container.  
The base students container is reduced and keeps only the passed students.

This uses memory more efficiently, but deletion/rearrangement cost depends heavily on the container type.

## Build Instructions

### Build with Make (Unix / MSYS2 / MinGW environments)

make

## Run

./grade_calc

## Clean Build File:

make clean

## Build with CMake (any OS)

# Configure

cmake -S . -B build

# Build

cmake --build build

## Run on Linux/macOS

./build/grade_calc

## Run on windows:

./build/grade_calc.exe

## Application Usage

- Build the project
- Run the executable
- Choose one of the menu options

## Menu Options

1. Interactive student input
- Enter student data manually
- Save to `Students.txt`
- Calculate final grade using average or median
2. Read `Students.txt` and print Avg/Med table
- Load saved student data
- Sort students
- Print both average-based and median-based final results
3. Generate benchmark files
- Creates large test files in `data/`
4. Run vector benchmark
- Runs the existing vector-only benchmark
5. Run vector + deque + list benchmarks
- Prompts for split strategy
- Benchmarks all three containers
6. Exit
- Closes the program

