#ifndef STREAMMATCH_RECOMMENDER_HPP
#define STREAMMATCH_RECOMMENDER_HPP

#include <cstddef>
#include <string>
#include <vector>

namespace streammatch {

struct Movie {
    int id;
    std::string title;
    int genre;       // 1 action, 2 comedy, 3 drama, 4 science fiction
    int mood;        // 1 light, 2 suspense, 3 thoughtful
    int minutes;
    int minimumAge;  // Invented teaching label, not an official classification.
    std::string summary;
};

struct Preferences {
    int age;
    int genre;
    int mood;
    int availableMinutes;
};

struct Match {
    Movie movie;
    int score;
    bool genreMatches;
    bool moodMatches;
};

const std::vector<Movie>& catalogue();
std::string genreName(int genre);
std::string moodName(int mood);
bool parseInteger(const std::string& text, int low, int high, int& result);
std::vector<Match> recommend(const std::vector<Movie>& movies,
                             const Preferences& preferences,
                             std::size_t limit = 3);

}  // namespace streammatch

#endif
