# StreamMatch Program Logic

## Input

The program collects four preferences from the user:

- Age
- Genre
- Mood
- Available viewing time

All inputs are validated before recommendation processing begins.

## Filtering

Two hard filters are applied first.

### Age Filter

Movies with a minimum age above the user's age are removed.

### Runtime Filter

Movies longer than the user's available viewing time are removed.

These filters are never ignored.

## Recommendation Score

Eligible movies receive:

- 5 points for matching the preferred genre
- 3 points for matching the preferred mood

The highest possible score is 8.

## Sorting

Recommendations are ordered by:

1. Highest recommendation score
2. Shorter runtime
3. Lower movie ID

The program displays up to three eligible movies.

## Example

For:

- Age: 23
- Genre: Comedy
- Mood: Light
- Available time: 95 minutes

The program returns:

1. Campus Detour - 8/8
2. Weekend Mix-Up - 8/8
3. Robot Roommate - 3/8

The first two movies match both genre and mood. Robot Roommate matches the selected mood but has a different genre.

## Input Validation

Invalid values do not terminate the program. The user is asked to enter the value again until a valid input is provided.

This applies to:

- Main menu
- Age
- Genre
- Mood
- Available viewing time