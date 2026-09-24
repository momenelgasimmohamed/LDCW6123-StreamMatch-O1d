#include "recommender.hpp"

#include <algorithm>
#include <sstream>
#include <stdexcept>

namespace streammatch {

namespace {
constexpr int GENRE_MATCH_POINTS = 5;
constexpr int MOOD_MATCH_POINTS = 3;
}

const std::vector<Movie>& catalogue() {
    static const std::vector<Movie> movies = {
        {1, "Campus Detour", 2, 1, 85, 7,
         "Two classmates turn a missed bus into an unexpected adventure."},
        {2, "The Last Signal", 4, 2, 110, 13,
         "A radio student receives a message from an abandoned station."},
        {3, "Letters at Dawn", 3, 3, 100, 7,
         "A young archivist reconnects a family through forgotten letters."},
        {4, "City Sprint", 1, 2, 95, 13,
         "A courier races across a city to return a missing research drive."},
        {5, "Orbit Garden", 4, 3, 120, 7,
         "A space gardener must decide how to share a limited water supply."},
        {6, "Weekend Mix-Up", 2, 1, 95, 7,
         "Three neighbours accidentally exchange their holiday plans."},
        {7, "Night Platform", 3, 2, 105, 16,
         "A station worker investigates a mysterious late-night visitor."},
        {8, "Rescue Route", 1, 1, 90, 7,
         "A small volunteer team delivers supplies after a mountain storm."},
        {9, "Robot Roommate", 4, 1, 80, 0,
         "A friendly household robot learns the meaning of cooperation."},
        {10, "The Quiet Bridge", 3, 3, 90, 0,
         "Two neighbours rebuild an old footbridge and their friendship."},
        {11, "Midnight Pursuit", 1, 2, 130, 18,
         "A retired investigator follows a dangerous trail through the city."},
        {12, "Small Town Stage", 2, 3, 100, 0,
         "A community theatre discovers that its backstage stories matter."}
    };

    return movies;
}

std::string genreName(int genre) {
    switch (genre) {
        case 1: return "Action";
        case 2: return "Comedy";
        case 3: return "Drama";
        case 4: return "Science fiction";
        default: return "Unknown";
    }
}

std::string moodName(int mood) {
    switch (mood) {
        case 1: return "Light";
        case 2: return "Suspense";
        case 3: return "Thoughtful";
        default: return "Unknown";
    }
}

bool parseInteger(const std::string& text, int low, int high, int& result) {
    std::istringstream input(text);
    int value = 0;

    if (!(input >> value)) {
        return false;
    }

    input >> std::ws;

    if (!input.eof() || value < low || value > high) {
        return false;
    }

    result = value;
    return true;
}

std::vector<Match> recommend(const std::vector<Movie>& movies,
                             const Preferences& preferences,
                             std::size_t limit) {

    if (preferences.age < 0 || preferences.age > 120 ||
        preferences.genre < 1 || preferences.genre > 4 ||
        preferences.mood < 1 || preferences.mood > 3 ||
        preferences.availableMinutes < 30 ||
        preferences.availableMinutes > 240) {

        throw std::invalid_argument("Preferences outside supported ranges.");
    }

    std::vector<Match> matches;

    for (const Movie& movie : movies) {

        if (movie.minimumAge > preferences.age ||
            movie.minutes > preferences.availableMinutes) {
            continue;
        }

        const bool genreMatches =
            movie.genre == preferences.genre;

        const bool moodMatches =
            movie.mood == preferences.mood;

        const int score =
            (genreMatches ? GENRE_MATCH_POINTS : 0) +
            (moodMatches ? MOOD_MATCH_POINTS : 0);

        matches.push_back({
            movie,
            score,
            genreMatches,
            moodMatches
        });
    }

    std::sort(
        matches.begin(),
        matches.end(),
        [](const Match& left, const Match& right) {

            if (left.score != right.score) {
                return left.score > right.score;
            }

            if (left.movie.minutes != right.movie.minutes) {
                return left.movie.minutes < right.movie.minutes;
            }

            return left.movie.id < right.movie.id;
        }
    );

    if (matches.size() > limit) {
        matches.resize(limit);
    }

    return matches;
}

}  // namespace streammatch