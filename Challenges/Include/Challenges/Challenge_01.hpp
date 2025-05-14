/**
 * @file challenge_01.hpp
 * @author Alunya
 * @brief Contains the day 01 challenge declaration.
 * @date 13.05.2025
 */

#ifndef CHALLENGE_01_HPP
#define CHALLENGE_01_HPP

#include <cstdint>
#include <string>
#include <vector>

namespace aoc {

class Challenge01 {
public:
    Challenge01( const std::string& filePath );
    void runChallenge();

private:
    bool         readFile();
    std::int64_t getDistanceBetweenVectors();
    std::int64_t getSimilarityScore();

private:
    const std::string         mFilePath;
    std::vector<std::int64_t> mLeftVector;
    std::vector<std::int64_t> mRightVector;
}; // class Challenge01

} // namespace aoc

#endif // ifndef CHALLENGE_01_HPP
