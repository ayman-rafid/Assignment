# Project101

Project101 is a C++ student grade calculator. It collects student name/surname,
homework scores, and exam score, then calculates final points using average or
median logic.

On every run, the app starts with interactive data entry and creates a new
runtime file:

- `Students_<userTag>_<timestamp>.txt`

After that, you can continue in:

- `I` interactive result mode (calculates and prints one selected method)
- `F` file mode (loads the newly generated file and prints both Avg/Med results)

## Final Grade Formula

- Using homework average:
  `Final = 0.4 * HomeworkAverage + 0.6 * Exam`
- Using homework median:
  `Final = 0.4 * HomeworkMedian + 0.6 * Exam`

## Setup (Windows)

1. Install Git (if needed):
   `winget install --id Git.Git -e`
2. Install MinGW g++ (if needed):
   `winget install --id BrechtSanders.WinLibs.POSIX.UCRT -e`
3. Open PowerShell and add g++ to current PATH:
   `$env:Path += ";C:\Users\Administrator\AppData\Local\Microsoft\WinGet\Packages\BrechtSanders.WinLibs.POSIX.UCRT_Microsoft.Winget.Source_8wekyb3d8bbwe\mingw64\bin"`
4. Clone and open the project:
   `git clone https://github.com/ayman-rafid/Project101.git`
   `cd Project101`
5. Checkout assignment branch:
   `git checkout v0.1`

## Build

Compile from project root:

`g++ -std=c++17 -O2 -Wall -Wextra -pedantic -static -static-libgcc -static-libstdc++ main.cpp -o grade_calc.exe`

## Execute and Generate Data

1. Run program:
   `.\grade_calc.exe`
2. Enter run user tag (single word).
3. Enter interactive data first (always required):
   - choose input mode: `M` manual or `R` random
   - enter number of students
   - enter/generate scores
4. Program generates a new file for this run:
   - `Students_<userTag>_<timestamp>.txt`
5. Choose next mode:
   - `I`: choose `A` or `M` and print one final-point column
   - `F`: load the file generated in this same run and print both Avg/Med columns
6. Generated file remains on disk after the run.

## Quick Example Input

```
firstuser
M
1
Ayan
Khan
8
9
-1
10
F
S
```
