/**
 * @file Challenge_01.hpp
 * @author Alunya
 * @brief Contains the day 01 challenge declaration.
 * @date 27.07.2025
 */

#ifndef CHALLENGE_01_HPP
#define CHALLENGE_01_HPP

#include "BaseChallenge.hpp"

#include <cstdint>
#include <string>
#include <string_view>
#include <vector>

namespace aoc {
using std::int64_t;

class Challenge01 : public BaseChallenge {
public:
    Challenge01( const std::string_view filePath );
    virtual ~Challenge01() = default;

    void runChallenge() override;

private:
    bool    readFile() override;
    int64_t getDistanceBetweenVectors();
    int64_t getSimilarityScore();

private:
    int64_t              partI;
    int64_t              partII;
    std::vector<int64_t> mLeftVector;
    std::vector<int64_t> mRightVector;
}; // class Challenge01

} // namespace aoc

#endif // ifndef CHALLENGE_01_HPP
