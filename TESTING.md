# StreamMatch Testing

The program was compiled and tested locally on Windows using g++ 14.2.0.

## Test 1: Successful Recommendation

Input:
- Age: 23
- Genre: Comedy
- Mood: Light
- Available time: 95 minutes

Expected result:
- Campus Detour - 8/8
- Weekend Mix-Up - 8/8
- Robot Roommate - 3/8

Result: Passed.

## Test 2: Invalid Input

The following invalid inputs were tested:
- Menu option outside 0-3
- Text entered for age
- Invalid genre value

The program rejected the invalid values and requested valid input again.

Result: Passed.

## Test 3: No Eligible Movie

Input:
- Age: 23
- Genre: Comedy
- Mood: Light
- Available time: 30 minutes

The program displayed:

`No eligible titles fit your age and available time.`

Result: Passed.

## Test 4: Program Exit

Menu option `0` was selected.

The program exited normally.

Result: Passed.