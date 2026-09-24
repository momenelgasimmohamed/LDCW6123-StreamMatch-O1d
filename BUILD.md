# Build and Run Guide

## Environment Used

The project was compiled and tested on Windows using:

- g++ 14.2.0
- C++17
- Windows PowerShell

## Compile

From the project root:

```powershell
g++ -std=c++17 -Wall -Wextra -Wpedantic .\src\main.cpp .\src\recommender.cpp -o streammatch.exe