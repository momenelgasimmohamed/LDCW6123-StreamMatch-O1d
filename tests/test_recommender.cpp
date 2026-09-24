#include "recommender.hpp"

#include <iostream>
#include <set>
#include <stdexcept>
#include <string>

using namespace streammatch;

namespace {
int passed = 0;
int failed = 0;

void check(bool condition, const std::string& name) {
    std::cout << (condition ? "PASS " : "FAIL ") << name << '\n';
    condition ? ++passed : ++failed;
}

bool rejects(const Preferences& preferences) {
    try {
        (void)recommend(catalogue(), preferences);
        return false;
    } catch (const std::invalid_argument&) {
        return true;
    }
}
}  // namespace

int main() {
    int value = 42;
    check(parseInteger("0", 0, 120, value) && value == 0, "U01 lower input boundary");
    check(parseInteger("120", 0, 120, value) && value == 120, "U02 upper input boundary");
    check(parseInteger(" 23 \t", 0, 120, value) && value == 23, "U03 surrounding whitespace");
    check(!parseInteger("", 0, 120, value), "U04 blank rejected");
    check(!parseInteger("   ", 0, 120, value), "U05 whitespace-only rejected");
    check(!parseInteger("abc", 0, 120, value), "U06 alphabetic rejected");
    check(!parseInteger("2abc", 0, 120, value), "U07 trailing text rejected");
    check(!parseInteger("2.5", 0, 120, value), "U08 decimal rejected");
    check(!parseInteger("2 3", 0, 120, value), "U09 multiple integers rejected");
    check(!parseInteger("-1", 0, 120, value), "U10 below range rejected");
    check(!parseInteger("121", 0, 120, value), "U11 above range rejected");
    check(!parseInteger("999999999999999999999", 0, 120, value), "U12 overflow rejected");
    check(!parseInteger("1e2", 0, 120, value), "U13 exponent notation rejected");
    value = 42;
    check(!parseInteger("bad", 0, 120, value) && value == 42, "U14 failure preserves output");

    const Preferences normal{23, 2, 1, 95};
    const auto typical = recommend(catalogue(), normal);
    check(typical.size() == 3 && typical[0].movie.id == 1 && typical[0].score == 8,
          "U15 expected top match");
    check(typical.size() == 3 && typical[1].movie.id == 6 && typical[1].score == 8,
          "U16 shorter runtime breaks score tie");
    check(typical.size() == 3 && typical[2].movie.id == 9 && typical[2].score == 3 &&
          !typical[2].genreMatches && typical[2].moodMatches, "U17 alternative is labelled honestly");
    const auto ageBoundary = recommend(catalogue(), {13, 1, 2, 95}, 20);
    bool includes13 = false;
    bool safe = true;
    for (const Match& match : ageBoundary) {
        includes13 = includes13 || match.movie.id == 4;
        safe = safe && match.movie.minimumAge <= 13 && match.movie.minutes <= 95;
    }
    check(includes13 && safe, "U18 inclusive age and runtime boundary");
    const auto child = recommend(catalogue(), {0, 4, 1, 240}, 20);
    bool childSafe = child.size() == 3;
    for (const Match& match : child) childSafe = childSafe && match.movie.minimumAge == 0;
    check(childSafe, "U19 age-zero filter");
    check(recommend(catalogue(), {23, 2, 1, 30}).empty(), "U20 no eligible runtime");
    check(recommend({}, normal).empty(), "U21 empty catalogue");
    check(recommend(catalogue(), normal, 0).empty(), "U22 zero requested results");
    check(recommend(catalogue(), normal, 1).size() == 1, "U23 result limit");
    const auto one = recommend(catalogue(), {0, 4, 1, 80});
    check(one.size() == 1 && one[0].movie.id == 9, "U24 fewer than three eligible");
    const auto noneExact = recommend(catalogue(), {23, 4, 3, 100});
    check(!noneExact.empty() && noneExact[0].score == 5 && noneExact[0].movie.id == 9,
          "U25 no exact preference match");
    const std::vector<Movie> tied = {
        {5,"Fifth",1,1,90,0,"Fictional"},
        {2,"Second",1,1,90,0,"Fictional"}
    };
    const auto tie = recommend(tied, {20,1,1,90});
    check(tie.size() == 2 && tie[0].movie.id == 2, "U26 stable ID tie-break");
    check(rejects({-1,1,1,90}) && rejects({121,1,1,90}), "U27 reject invalid age API input");
    check(rejects({20,0,1,90}) && rejects({20,5,1,90}), "U28 reject invalid genre API input");
    check(rejects({20,1,0,90}) && rejects({20,1,4,90}), "U29 reject invalid mood API input");
    check(rejects({20,1,1,29}) && rejects({20,1,1,241}), "U30 reject invalid time API input");
    std::set<int> ids;
    bool validCatalogue = catalogue().size() == 12;
    for (const Movie& movie : catalogue()) {
        validCatalogue = validCatalogue && ids.insert(movie.id).second &&
            !movie.title.empty() && !movie.summary.empty() &&
            movie.genre >= 1 && movie.genre <= 4 && movie.mood >= 1 && movie.mood <= 3 &&
            movie.minutes >= 30 && movie.minutes <= 240 &&
            movie.minimumAge >= 0 && movie.minimumAge <= 120;
    }
    check(validCatalogue, "U31 catalogue integrity and unique IDs");
    check(genreName(4) == "Science fiction" && moodName(3) == "Thoughtful" &&
          genreName(99) == "Unknown" && moodName(99) == "Unknown", "U32 display names and fallback");
    bool exhaustiveSafe = true;
    for (int age : {0,7,12,13,16,18,120}) {
        for (int genre = 1; genre <= 4; ++genre) {
            for (int mood = 1; mood <= 3; ++mood) {
                for (int minutes : {30,80,85,90,95,100,110,120,130,240}) {
                    const auto matches = recommend(catalogue(), {age,genre,mood,minutes});
                    exhaustiveSafe = exhaustiveSafe && matches.size() <= 3;
                    for (std::size_t i = 0; i < matches.size(); ++i) {
                        const Match& match = matches[i];
                        exhaustiveSafe = exhaustiveSafe && match.movie.minimumAge <= age &&
                            match.movie.minutes <= minutes && match.score >= 0 && match.score <= 8;
                        if (i > 0) exhaustiveSafe = exhaustiveSafe && matches[i-1].score >= match.score;
                    }
                }
            }
        }
    }
    check(exhaustiveSafe, "U33 840 preference combinations preserve constraints and score order");
    std::cout << "\nRESULT: " << passed << " passed, " << failed << " failed.\n";
    return failed == 0 ? 0 : 1;
}
