/**
 * @file Challenge_01.hpp
 * @author Alunya
 * @brief Contains the day 01 challenge declaration.
 * @date 30.07.2025
 */

#ifndef CHALLENGE_01_HPP
#define CHALLENGE_01_HPP

#include "BaseChallenge.hpp"
#include "../Utils/ChallengeResult.hpp"

#include <cstdint>
#include <string>
#include <string_view>
#include <vector>

namespace aoc {
using std::int64_t;

/**
 * @class Challenge01
 * @brief Class for the day 01 challenge.
 * @author Alunya
 * @date 30.07.2025
 *
 * The Challenge01 class implements the first challenge of the 'Advent of Code 2024'.
 */
class Challenge01 : public BaseChallenge {
public:
    Challenge01( const std::string_view filePath );
    virtual ~Challenge01() = default;

    ChallengeResult runChallenge() override;

private:
    bool    readFile() override;
    int64_t getDistanceBetweenVectors();
    int64_t getSimilarityScore();

private:
    std::vector<int64_t> mLeftVector;
    std::vector<int64_t> mRightVector;
}; // class Challenge01

} // namespace aoc

#endif // ifndef CHALLENGE_01_HPP