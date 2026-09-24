# StreamMatch

StreamMatch is the C++ program for our LDCW6123 group project. The project is connected to our research on Netflix and disruptive innovation.

The program is a simple movie recommendation assistant. It asks the user for their age, preferred genre, preferred mood and available viewing time, then recommends up to three suitable movies from a fictional catalogue.

## Group Members

- Momen Elgasim Mohamed Seed Ahmed - Group Leader
- Makhlouf Mayas
- Yazan Ihab Mahmoud Saleh
- Mohammed Abdelmoneim Nourelgalil Elbashir
- Alarraj Mohammed
- Anas Abdalla Elhassan Mohamed

## Program Features

- Menu-based console interface
- Input validation
- Age filtering
- Runtime filtering
- Genre and mood matching
- Recommendation scoring
- Up to three recommendations
- Clear explanation for each result

## Recommendation Logic

The program first removes movies that do not meet the user's age or available viewing time.

The remaining movies are scored using:

- Genre match: 5 points
- Mood match: 3 points

The maximum score is 8 points. If two movies have the same score, the shorter movie is shown first.

## Project Structure

```text
src/
  main.cpp
  recommender.cpp
  recommender.hpp

tests/
  test_recommender.cpp
  test_cli.py

evidence/
  01_successful_recommendation.png
  02_input_validation.png
  03_no_match_and_exit.png

CMakeLists.txt
README.md
```

## Build and Run

The program uses C++17.

### Windows with g++

```powershell
g++ -std=c++17 -Wall -Wextra -Wpedantic .\src\main.cpp .\src\recommender.cpp -o streammatch.exe
.\streammatch.exe
```

### CMake

```powershell
cmake -S . -B build
cmake --build build --config Release
```

## Manual Testing

We compiled and ran the program on Windows using g++ 14.2.0.

### Test 1: Successful recommendation

Input:

```text
Menu: 2
Age: 23
Genre: 2 - Comedy
Mood: 1 - Light
Available time: 95 minutes
```

Result:

```text
1. Campus Detour - 8/8
2. Weekend Mix-Up - 8/8
3. Robot Roommate - 3/8
```

### Test 2: Invalid input

We tested invalid menu and age values. The program rejected the input and asked for a valid value instead of closing or crashing.

### Test 3: No eligible movie

Input:

```text
Age: 23
Genre: 2 - Comedy
Mood: 1 - Light
Available time: 30 minutes
```

Result:

```text
No eligible titles fit your age and available time.
Try a longer viewing time; age restrictions stay enforced.
```

The program also exited normally when option `0` was selected.

## Connection to Part 1

Part 1 of our project studies Netflix using Clayton Christensen's Disruptive Innovation Model. StreamMatch connects to that topic by implementing a basic content-discovery feature in C++.

Instead of using real Netflix data, the program uses a fictional movie catalogue so that the recommendation logic can be demonstrated clearly and tested consistently.

## Course

LDCW6123 - Fundamentals of Digital Competence for Programmer  
Group Project - Trimester 2620
