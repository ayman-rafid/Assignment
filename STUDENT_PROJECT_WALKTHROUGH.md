# Assignment Project Walkthrough

This document explains the full project as if you (the student) built it from scratch.  
Use it to prepare for teacher questions, presentation, and viva.

## 1. Project Goal

Build a C++ student grade calculator that:

1. Stores student data in a `Person` class.
2. Applies Rule of Three (copy constructor, assignment operator, destructor).
3. Supports input/output with overloaded stream operators (`>>`, `<<`).
4. Calculates final grade by average or median.
5. Supports dynamic homework count using `std::vector`.
6. Supports random score generation.
7. Supports file-based processing and sorted output.
8. Uses Git branches/commits/releases according to assignment workflow.

## 2. Final Repository and Branch Strategy

Repository name was updated to **Assignment**.

- Default branch: `main` (initial setup and README baseline)
- Development branch: `v0.1` (all assignment implementation work)
- Release tag: `v0.1`

Why this branch strategy:

1. `main` remains stable.
2. `v0.1` contains iterative implementation history.
3. Tag `v0.1` marks the release snapshot requested by assignment.

## 3. Full Commit Timeline (What Was Done, Step by Step)

Chronological summary:

1. `2f78234` - `Initial commit`  
   Basic repository start.

2. `340d2c7` - `Add project overview and grading formulas to README`  
   README with intro and formula on `main`.

3. `67de66f` - `Implement initial Person class workflow with average grade output`  
   First working C++ version (Rule of Three + average).

4. `7199269` - `Add median option and flexible vector-based homework input`  
   Added median logic and dynamic homework count.

5. `6ca5a44` - `Support random generation of homework and exam scores`  
   Added random data generation mode.

6. `0c977e7` - `Read student data from file and print sorted avg/median table`  
   Added file reading and formatted sorted output.

7. `59cc969` - `Add runtime student file writer`  
   Added ability to save generated students into runtime file.

8. `27a6ad8` - `Require interactive setup on every run`  
   New flow: each run starts in interactive setup, then user chooses next mode.

9. `8c85858` - `Remove static student data file`  
   Removed old fixed file approach.

10. `95c5a32` - `Ignore generated files`  
    Added `.gitignore` for `Students_*.txt` and build outputs.

11. `0a34d9a` - `Update README run guide`  
    Added full setup/build/run docs.

12. `8650bd4` - `Fix README prompt order`  
    Corrected docs to exact runtime prompt sequence.

13. `c21ef78` - `Modified Readme again - Final`  
    Final manual documentation cleanup.

14. `07df447` - `Rename project to Assignment`  
    Updated naming references from Project101 to Assignment.

## 4. Code Architecture (main.cpp)

Important sections in `main.cpp`:

- Enums (`CalculationMethod`, `InputMode`, `DataSource`, `SortField`)
- `Person` class
- Input/output operator overloads
- Utility input functions
- Interactive collection function
- File save/load functions
- Sorting + formatted printing functions
- `main()` orchestration

### 4.1 Enums

Enums make input choices explicit and type-safe:

1. `CalculationMethod` -> `Average`, `Median`
2. `InputMode` -> `Manual`, `Random`
3. `DataSource` -> `Interactive`, `File`
4. `SortField` -> `Name`, `Surname`

### 4.2 Person Class Design

`Person` stores:

1. `firstName_`
2. `surname_`
3. `homework_` (`std::vector<int>`)
4. `examResult_`
5. `finalGrade_`

### 4.3 Rule of Three Implementation

The class explicitly includes:

1. Copy constructor
2. Copy assignment operator
3. Destructor

Even though `std::vector` and `std::string` already manage memory safely, this is kept because assignment requires Rule of Three.

### 4.4 Operator Overloading

1. `operator>>` for interactive input into `Person`
2. `operator<<` for formatted single-row output

This satisfies the assignment requirement for overloaded `cin`/`cout` behavior.

## 5. Core Grading Logic

Formula:

- Average mode: `0.4 * homeworkAverage + 0.6 * exam`
- Median mode: `0.4 * homeworkMedian + 0.6 * exam`

Median handling:

1. Sort homework vector
2. Odd count -> middle value
3. Even count -> average of two middle values

## 6. Runtime Workflow (Current Final Behavior)

Every run follows this order:

1. Program starts and requires interactive setup.
2. User enters `userTag`.
3. User chooses input mode:
   - `M` manual
   - `R` random
4. User enters number of students and input data is collected.
5. Program generates a new runtime file:
   - `Students_<userTag>_<timestamp>.txt`
6. User chooses next mode:
   - `I` interactive result mode
   - `F` file result mode
7. If `I`:
   - choose grade method `A` or `M`
   - print one final column
8. If `F`:
   - choose sort field `N` or `S`
   - load the generated file from this run
   - print both Avg/Med columns
9. Generated file remains on disk.

Design reason:

1. Guarantees fresh data each run.
2. Keeps file-mode tied to just-created data.
3. Avoids stale static input file dependency.

## 7. File Format and Parsing

Generated file header style:

`Name Surname HW1 HW2 ... Exam`

Reading logic:

1. Skip first line (header)
2. Read each row
3. First two tokens -> name + surname
4. Remaining numeric values:
   - last value -> exam
   - previous values -> homework vector

## 8. Sorting and Output Formatting

Sorting:

1. By name (`N`) or surname (`S`)
2. Tie-breaker uses the other field

Formatting:

1. Uses `setw`, `left`, `right`, `fixed`, `setprecision(2)`
2. Produces aligned table-like output

## 9. Setup and Run on a New Device

### 9.1 Install tools (Windows)

1. Install Git:
   `winget install --id Git.Git -e`
2. Install MinGW toolchain:
   `winget install --id BrechtSanders.WinLibs.POSIX.UCRT -e`

### 9.2 Clone

1. `git clone https://github.com/ayman-rafid/Assignment.git`
2. `cd Assignment`
3. `git checkout v0.1`

### 9.3 Add g++ to current shell path

`$env:Path += ";C:\Users\Administrator\AppData\Local\Microsoft\WinGet\Packages\BrechtSanders.WinLibs.POSIX.UCRT_Microsoft.Winget.Source_8wekyb3d8bbwe\mingw64\bin"`

### 9.4 Build

`g++ -std=c++17 -O2 -Wall -Wextra -pedantic -static -static-libgcc -static-libstdc++ main.cpp -o grade_calc.exe`

### 9.5 Run

`.\grade_calc.exe`

## 10. What to Say in Presentation (Short Script)

You can present like this:

1. "I created the project with `main` and `v0.1` branch strategy, then delivered features incrementally through commits."
2. "I implemented a `Person` class with Rule of Three and stream operator overloads."
3. "I used `std::vector` for variable homework count and added average/median final grade options."
4. "I added random generation for faster data creation and testing."
5. "I implemented runtime file generation and file processing, then sorting and formatted output."
6. "I finalized with release `v0.1`, documentation, and repository rename to Assignment."

## 11. Common Teacher Questions and Good Answers

### Q1: Why `std::vector` for homework?
Because homework count is not fixed beforehand; vector is dynamic and satisfies assignment requirement.

### Q2: Why Rule of Three if STL already handles memory?
Assignment explicitly asked for Rule of Three, so it was implemented for compliance and OOP practice.

### Q3: Why separate interactive and file modes?
Interactive mode is for data entry/generation; file mode validates persistence and batch processing behavior.

### Q4: Why use sorting options?
Assignment asked for ordered output by name or surname.

### Q5: Why format to two decimals?
Assignment explicitly requests precise final-grade display formatting.

## 12. Final Checklist (Submission Readiness)

1. Branch `v0.1` exists with full implementation history.
2. README has setup/build/run instructions.
3. Rule of Three and operator overloading are present.
4. Average and median logic are present.
5. Vector-based flexible homework input is present.
6. Random generation is present.
7. File save/load with sorted output is present.
8. Release `v0.1` exists and points to final state.
9. Repository name is `Assignment`.

---

If you memorize sections 3, 4, 6, and 11, you can explain the project clearly under most teacher questioning styles.
