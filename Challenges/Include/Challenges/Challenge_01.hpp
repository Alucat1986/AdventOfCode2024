/**
 * @file challenge_01.hpp
 * @author Alunya
 * @brief Contains the day 01 challenge declarations.
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
    void RunChallenge();

private:
    bool readFile();
    int  getDistanceBetweenVectors();
    int  getSimilarityScore();

private:
    const std::string         mFilePath;
    std::vector<std::int32_t> mLeftVector;
    std::vector<std::int32_t> mRightVector;
}; // class Challenge01

} // namespace aoc

#endif // ifndef CHALLENGE_01_HPP
