/**
 * @file Challenge02.hpp
 * @author Alunya
 * @brief Contains the day 02 challenge declaration.
 * @date 04.08.2025
 */

#ifndef CHALLENGE_02_HPP
#define CHALLENGE_02_HPP

#include "BaseChallenge.hpp"
#include "../Utils/ChallengeResult.hpp"

#include <cstdint>
#include <string>
#include <string_view>
#include <vector>

namespace aoc {

/**
 * @class Challenge02
 * @brief Class for the day 02 challenge.
 * @author Alunya
 * @date 04.08.2025
 *
 * The Challenge02 class implements the second challenge of the 'Advent of Code 2024'.
 */
class Challenge02 : public BaseChallenge {
public:
    Challenge02( const std::string_view filePath );
    virtual ~Challenge02() = default;

    ChallengeResult runChallenge() override;

private:
    bool         readFile() override;
    std::int64_t getDistanceBetweenVectors() const;
    std::int64_t getSimilarityScore() const;

private:
    std::vector<std::int64_t> mLeftVector;
    std::vector<std::int64_t> mRightVector;
}; // class Challenge02

} // namespace aoc

#endif // ifndef CHALLENGE_02_HPP
