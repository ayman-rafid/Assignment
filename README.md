# Assignment v0.25

This project is a C++ student grade calculator and benchmark project.

## Containers used

- `std::vector`
- `std::deque`
- `std::list`

## Features

- refactored multi-file project (`include/` and `src/`)
- `Person` class with rule-of-three members
- final grade calculation using average or median
- interactive input and random generation
- reading from `Students.txt`
- benchmark data generation for:
  - 1,000
  - 10,000
  - 100,000
  - 1,000,000
  - 10,000,000
- splitting students into:
  - failed (`< 5.0`)
  - passed (`>= 5.0`)
- timing analysis for:
  - read
  - sort
  - split
  - write
  - total runtime

## Build

`bash`
g++ -std=c++17 -O2 -Wall -Wextra -Iinclude src/*.cpp -o grade_calc.exe

## Menu

1. Interactive student input
2. Read Students.txt and print Avg/Med table
3. Generate benchmark data files
4. Run vector benchmark
5. Run vector + deque + list benchmarks
6. Exit

## Benchmark Reports

- benchmark/vector_benchmark.txt
- benchmark/all_containers_benchmark.txt

