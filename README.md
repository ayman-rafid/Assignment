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
- Refactored the chosen path using `<algorithm>` library functions and compared the performance after the change
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

This uses memory more efficiently, but deletion and rearrangement cost depends heavily on the container type.

## Build Instructions

### Build with Make (Unix / MSYS2 / MinGW environments)
```bash
make
```

Run:
```bash
./grade_calc
```

Clean build files:
```bash
make clean
```

### Build with CMake (any OS)
Configure:
```bash
cmake -S . -B build
```

Build:
```bash
cmake --build build
```

Run on Linux/macOS:
```bash
./build/grade_calc
```

Run on Windows:
```bash
./build/grade_calc.exe
```

## Application Usage

1. Build the project.
2. Run the executable.
3. Choose one of the menu options.

### Menu options
1. **Interactive student input**
   - Enter student data manually
   - Save to `Students.txt`
   - Calculate the final grade using average or median

2. **Read `Students.txt` and print Avg/Med table**
   - Load saved student data
   - Sort students
   - Print both average-based and median-based final results

3. **Generate benchmark files**
   - Creates large test files in `data/`

4. **Run vector benchmark**
   - Runs the vector-only benchmark

5. **Run vector + deque + list benchmarks**
   - Prompts for split strategy
   - Benchmarks all three containers

6. **Exit**
   - Closes the program

## Benchmark Observations

### Before algorithm-based refactor of the chosen path
Strategy 2 results at `students1000000.txt`:

| Container | Total time (ms) |
|----------|-----------------:|
| vector   | 6489.01 |
| deque    | 6624.84 |
| list     | 4765.16 |

Observation:
- `list + Strategy 2` was the fastest overall combination in total runtime.

### After algorithm-based refactor
After replacing the chosen `list + Strategy 2` split with `std::stable_partition` and comparing again at `students1000000.txt`:

| Container | Total time (ms) |
|----------|-----------------:|
| vector   | 6557.72 |
| deque    | 6725.39 |
| list     | 5092.60 |

Observation:
- `list` still remained the fastest overall container in total runtime.
- However, the split step for `list` became slower after the `<algorithm>` refactor.
- This shows that generic algorithms from `<algorithm>` do not always outperform container-specific logic.
- Even so, the algorithm-based version is useful for comparison and satisfies the assignment requirement to test and compare performance after refactoring.

## Conclusions

- Strategy 2 is more memory-friendly than Strategy 1.
- `std::list` performed best overall in the tested runs for the full workflow.
- `std::vector` and `std::deque` improved significantly after replacing repeated erase operations with better algorithm-based partitioning.
- Very large files such as `students10000000.txt` can take a long time to process, especially because reading, sorting, splitting, and writing all contribute to total runtime.

## Project Structure

```text
include/
src/
data/
output/
benchmark/
README.md
Makefile
CMakeLists.txt
```