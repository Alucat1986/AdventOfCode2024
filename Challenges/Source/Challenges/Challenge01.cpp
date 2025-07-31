/**
 * @file Challenge_01.cpp
 * @author Alunya
 * @brief Contains the day 01 challenge implementation.
 * @date 30.07.2025
 */

#include "Challenge_01.hpp"

#include "../Debug/Debug.hpp"
#include "../Utils/ChallengeResult.hpp"

#include <algorithm>
#include <chrono>
#include <cstdint>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <ranges>
#include <string>
#include <string_view>
#include <vector>

namespace aoc {
using std::int64_t, std::size_t;

// ****************************************************************************************************************** //
//                                                      PUBLIC                                                        //
// ****************************************************************************************************************** //

/**
 * @brief Constructor.
 * @author Alunya
 * @date 30.07.2025
 * @param[in] filePath The path to the input file.
 */
Challenge01::Challenge01( const std::string_view filePath )
        : BaseChallenge( filePath ) {
    if ( !readFile() ) {
        debug::logMessage( "Challenge 01:: Initialisation failed: File not loaded", mFilePath );
        return;
    } // if ( !readFile() )

    std::ranges::sort( mLeftVector );
    std::ranges::sort( mRightVector );

    debug::logMessage( "Challenge 01 initialised with file: ", mFilePath );
} // Challenge01::Challenge01(...)

/**
 * @brief Runs the challenge.
 * @author Alunya
 * @date 30.07.2025
 * @return The result of the challenge including their execution times.
 */
ChallengeResult Challenge01::runChallenge() {
    const auto partOneStart     = std::chrono::system_clock::now();
    mResult.partI               = getDistanceBetweenVectors();
    const auto partOneEnd       = std::chrono::system_clock::now();
    mResult.executionTimePartI  = std::chrono::duration_cast<std::chrono::microseconds>( partOneEnd - partOneStart );

    const auto partTwoStart     = std::chrono::system_clock::now();
    mResult.partII              = getSimilarityScore();
    const auto partTwoEnd       = std::chrono::system_clock::now();
    mResult.executionTimePartII = std::chrono::duration_cast<std::chrono::microseconds>( partTwoEnd - partTwoStart );

    return mResult;
} // ChallengeResult Challenge01::RunChallenge(...)

// ****************************************************************************************************************** //
//                                                      PRIVATE                                                       //
// ****************************************************************************************************************** //

/**
 * @brief Reads the input file and populates the left and right vectors.
 * @author Alunya
 * @date 30.07.2025
 * @return true When the file was read successfully, otherwise false.
 */
bool Challenge01::readFile() {
    std::ifstream fileToRead( mFilePath );
    std::string   line = "", tmpSubString = "";
    size_t        position{ 0 };

    if ( !fileToRead.is_open() ) {
        std::cout << "Failed to open " << mFilePath << " ...\n";
        return false;
    }

    std::cout << "Reading file " << mFilePath << " ...\n\n";
    while ( std::getline( fileToRead, line ) ) {
        debug::logMessage( "Line read: ", line, " Length(", line.size(), "): " );

        position     = line.find_first_of( ' ' );
        tmpSubString = line.substr( 0, position );
        mLeftVector.emplace_back( std::stoi( tmpSubString ) );
        debug::logMessage( std::stoi( tmpSubString ), " / " );

        position     = line.find_last_of( ' ' );
        tmpSubString = line.substr( position, line.size() );
        mRightVector.emplace_back( std::stoi( tmpSubString ) );
        debug::logMessage( std::stoi( tmpSubString ) );
    } // while ( std::getline(fileToRead, line) )
    return true;
} // bool Challenge01::readFile(...)

/**
 * @brief Calculates the sum of distances between the left and right vectors.
 * @author Alunya
 * @date 30.07.2025
 * @return The total distance between the two vectors.
 */
int64_t Challenge01::getDistanceBetweenVectors() {
    size_t  iterator{ 0 };
    int64_t distance{ 0 };

    for ( int64_t element : mLeftVector ) {
        distance += std::abs( element - mRightVector.at( iterator ) );
        iterator++;
    }
    return distance;
} // int64_t Challenge01::getDistanceBetweenVectors(...)

/**
 * @brief Calculates the similarity score between the left and right vectors.
 * @author Alunya
 * @date 30.07.2025
 * @return The similarity score.
 */
int64_t Challenge01::getSimilarityScore() {
    int64_t similarity{ 0 };

    for ( int64_t element : mLeftVector ) {
        similarity += element * std::ranges::count( mRightVector, element );
    } // for ( int element : leftSide )
    return similarity;
} // int64_t Challenge01::getSimilarityScore(...)

} // namespace aoc