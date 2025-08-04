/**
 * @file ChallengeResult.hpp
 * @author Alunya
 * @brief Contains the declaration of the ChallengeResult struct.
 * @date 04.08.2025
 */

#ifndef CHALLENGE_RESULT_HPP
#define CHALLENGE_RESULT_HPP

#include <chrono>
#include <cstdint>

namespace aoc {
using std::int64_t;

struct ChallengeResult {
    int64_t                   partI{ 0 };
    int64_t                   partII{ 0 };
    std::chrono::microseconds executionTimePartI{ 0 };
    std::chrono::microseconds executionTimePartII{ 0 };
}; // struct ChallengeResult

} // namespace aoc

#endif // ifndef CHALLENGE_RESULT_HPP