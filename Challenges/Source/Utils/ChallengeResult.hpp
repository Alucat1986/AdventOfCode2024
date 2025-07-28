/**
 * @file ChallengeResult.hpp
 * @author Alunya
 * @brief Contains the declaration of the ChallengeResult struct.
 * @date 27.07.2025
 */

#ifndef CHALLENGE_RESULT_HPP
#define CHALLENGE_RESULT_HPP

#include <chrono>
#include <cstdint>

namespace aoc {
using std::int64_t;

struct ChallengeResult {
    int64_t                   partI;
    int64_t                   partII;
    std::chrono::microseconds executionTimePartI;
    std::chrono::microseconds executionTimePartII;
}; // struct ChallengeResult

} // namespace aoc

#endif // ifndef CHALLENGE_RESULT_HPP
