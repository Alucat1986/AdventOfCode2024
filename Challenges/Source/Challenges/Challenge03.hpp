/**
 * @file Challenge03.hpp
 * @author Alunya
 * @brief Contains the day 03 challenge declaration.
 * @date 04.08.2025
 */

#ifndef CHALLENGE_03_HPP
#define CHALLENGE_03_HPP

#include "BaseChallenge.hpp"
#include "../Utils/ChallengeResult.hpp"

#include <cstdint>
#include <string>
#include <string_view>
#include <vector>

namespace aoc {

/**
 * @class Challenge03
 * @brief Class for the day 03 challenge.
 * @author Alunya
 * @date 04.08.2025
 *
 * The Challenge03 class implements the third challenge of the 'Advent of Code 2024'.
 */
class Challenge03 : public BaseChallenge {
public:
    Challenge03( const std::string_view filePath );
    virtual ~Challenge03() = default;

    ChallengeResult runChallenge() override;

private:
    bool         readFile() override;
    std::int64_t findMatches( const std::string& input, const std::string& regularExpression, bool enabled ) const;
    std::int64_t multiplyString( std::string stringToMultiply ) const;

private:
    std::vector<std::string> mMulOperations;
}; // class Challenge03

} // namespace aoc

#endif // ifndef CHALLENGE_03_HPP