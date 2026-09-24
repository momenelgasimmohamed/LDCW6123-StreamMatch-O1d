#include "recommender.hpp"

#include <iomanip>
#include <iostream>
#include <string>

using namespace streammatch;

namespace {

bool readInteger(const std::string& prompt, int low, int high, int& value) {
    std::string text;
    while (true) {
        std::cout << prompt;
        if (!std::getline(std::cin, text)) {
            std::cout << "\nInput ended. Closing StreamMatch.\n";
            return false;
        }
        if (parseInteger(text, low, high, value)) {
            return true;
        }
        std::cout << "Invalid input. Enter a whole number from "
                  << low << " to " << high << ".\n";
    }
}

void browseCatalogue() {
    std::cout << "\nFICTIONAL CATALOGUE (12 titles)\n"
              << "Age labels are invented teaching data, not official ratings.\n\n";
    for (const Movie& movie : catalogue()) {
        std::cout << std::setw(2) << movie.id << ". " << movie.title
                  << " | " << genreName(movie.genre)
                  << " | " << moodName(movie.mood)
                  << " | " << movie.minutes << " min | age "
                  << movie.minimumAge << "+\n";
    }
}

void explainMatching() {
    std::cout << "\nHOW MATCHING WORKS\n"
              << "1. Exclude titles above your age or available time.\n"
              << "2. Add 5 points for genre and 3 points for mood.\n"
              << "3. Sort by score, then shorter runtime, then smaller ID.\n"
              << "4. Show up to three eligible titles.\n"
              << "A score out of 8 is not a probability or film quality rating.\n"
              << "Alternatives may differ from your preferences; we label them.\n"
              << "No viewing history is saved. No internet or Netflix account\n"
              << "is used. This is a rule-based classroom demonstration.\n";
}

bool recommendInteractive() {
    Preferences preferences{};
    std::cout << "\nFIND SOMETHING TO WATCH\n";
    if (!readInteger("Your age (0-120): ", 0, 120, preferences.age)) {
        return false;
    }
    std::cout << "Genres: 1 Action | 2 Comedy | 3 Drama | 4 Science fiction\n";
    if (!readInteger("Preferred genre (1-4): ", 1, 4, preferences.genre)) {
        return false;
    }
    std::cout << "Moods: 1 Light | 2 Suspense | 3 Thoughtful\n";
    if (!readInteger("Preferred mood (1-3): ", 1, 3, preferences.mood) ||
        !readInteger("Available minutes (30-240): ", 30, 240,
                     preferences.availableMinutes)) {
        return false;
    }

    const auto matches = recommend(catalogue(), preferences);
    if (matches.empty()) {
        std::cout << "\nNo eligible titles fit your age and available time.\n"
                  << "Try a longer viewing time; age restrictions stay enforced.\n";
        return true;
    }

    std::cout << "\nTOP MATCHES (up to 3)\n";
    if (matches.front().score < 8) {
        std::cout << "No exact genre-and-mood match; showing labelled alternatives.\n";
    }
    for (std::size_t index = 0; index < matches.size(); ++index) {
        const Match& match = matches[index];
        std::cout << "\n" << index + 1 << ". " << match.movie.title
                  << " | score " << match.score << "/8\n"
                  << "   " << genreName(match.movie.genre) << " | "
                  << moodName(match.movie.mood) << " | "
                  << match.movie.minutes << " min | age "
                  << match.movie.minimumAge << "+\n"
                  << "   Why: genre "
                  << (match.genreMatches ? "matches" : "differs")
                  << ", mood " << (match.moodMatches ? "matches" : "differs")
                  << "; within age and time limits.\n"
                  << "   " << match.movie.summary << "\n";
    }
    return true;
}

}  // namespace

int main() {
    std::cout << "==============================================\n"
              << " STREAMMATCH | Movie Recommendation Assistant\n"
              << "==============================================\n"
              << "Netflix-inspired educational prototype.\n"
              << "All titles and age labels are fictional.\n";

    while (true) {
        std::cout << "\n1. Browse catalogue\n2. Recommend a movie\n"
                  << "3. How matching works\n0. Exit\n";
        int choice = 0;
        if (!readInteger("Choose (0-3): ", 0, 3, choice)) {
            return 0;
        }
        switch (choice) {
            case 0:
                std::cout << "Goodbye. No personal data was stored.\n";
                return 0;
            case 1:
                browseCatalogue();
                break;
            case 2:
                if (!recommendInteractive()) {
                    return 0;
                }
                break;
            case 3:
                explainMatching();
                break;
            default:
                // Defensive branch; validated input already prevents this.
                std::cout << "Please use the listed menu options.\n";
                break;
        }
    }
}
