/**
 * @file Challenge02.cpp
 * @author Alunya
 * @brief Contains the day 02 challenge implementation.
 * @date 31.07.2025
 */

#include "Challenge02.hpp"

#include "BaseChallenge.hpp"
#include "../Debug/Debug.hpp"
#include "../Utils/ChallengeResult.hpp"

#include <algorithm>
// #include <array>
#include <chrono>
#include <cstdint>
#include <cstdlib>
#include <fstream>
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
 * @date 31.07.2025
 * @param[in] filePath The path to the input file.
 */
Challenge02::Challenge02( const std::string_view filePath )
        : BaseChallenge( filePath ) {
    if ( !readFile() ) {
        debug::logMessage( "Challenge 02:: Initialisation failed: File not loaded", mFilePath );
        return;
    } // if ( !readFile() )

    std::ranges::sort( mLeftVector );
    std::ranges::sort( mRightVector );
} // Challenge02::Challenge02(...)

/**
 * @brief Runs the challenge.
 * @author Alunya
 * @date 31.07.2025
 * @return The result of the challenge including their execution times.
 */
ChallengeResult Challenge02::runChallenge() {
    const auto partOneStart     = std::chrono::system_clock::now();
    mResult.partI               = getDistanceBetweenVectors();
    const auto partOneEnd       = std::chrono::system_clock::now();
    mResult.executionTimePartI  = std::chrono::duration_cast<std::chrono::microseconds>( partOneEnd - partOneStart );

    const auto partTwoStart     = std::chrono::system_clock::now();
    mResult.partII              = getSimilarityScore();
    const auto partTwoEnd       = std::chrono::system_clock::now();
    mResult.executionTimePartII = std::chrono::duration_cast<std::chrono::microseconds>( partTwoEnd - partTwoStart );

    return mResult;
} // ChallengeResult Challenge02::runChallenge(...)

// ****************************************************************************************************************** //
//                                                      PRIVATE                                                       //
// ****************************************************************************************************************** //

/**
 * @brief Reads the input file and populates the left and right vectors.
 * @author Alunya
 * @date 31.07.2025
 * @return True if the file was read successfully, false otherwise.
 */
bool Challenge02::readFile() {
    std::ifstream fileToRead( mFilePath );
    std::string   line{ "" }, tmpSubString{ "" };
    size_t        position{ 0 };

    if ( !fileToRead.is_open() ) {
        std::cout << "Failed to open " << mFilePath << " ...\n";
        return false;
    }

    std::cout << "Reading file " << mFilePath << " ...\n";
    while ( std::getline( fileToRead, line ) ) {
        debug::logMessage( "Line read: ", line, " Length(", line.size(), "): " );

        position     = line.find_first_of( ' ' );
        tmpSubString = line.substr( 0, position );
        mLeftVector.emplace_back( static_cast<int64_t>( std::stoi( tmpSubString ) ) );
        debug::logMessage( std::stoi( tmpSubString ), " / " );

        position     = line.find_last_of( ' ' );
        tmpSubString = line.substr( position, line.size() );
        mRightVector.emplace_back( static_cast<int64_t>( std::stoi( tmpSubString ) ) );
        debug::logMessage( std::stoi( tmpSubString ) );
    } // while ( std::getline(fileToRead, line) )
    return true;
} // bool Challenge02::readFile(...)

/**
 * @brief Calculates the distance between the two vectors.
 * @author Alunya
 * @date 31.07.2025
 * @return The summed distance between the two vectors.
 */
int64_t Challenge02::getDistanceBetweenVectors() const {
    size_t  iterator{ 0 };
    int64_t distance{ 0 };

    for ( int64_t element : mLeftVector ) {
        distance += std::abs( element - mRightVector.at( iterator ) );
        iterator++;
    }
    return distance;
} // int64_t Challenge02::getDistanceBetweenVectors() const

/**
 * @brief Calculates the similarity score between the two vectors.
 * @author Alunya
 * @date 31.07.2025
 * @return The similarity score.
 */
int64_t Challenge02::getSimilarityScore() const {
    int64_t similarity{ 0 };

    for ( int64_t element : mLeftVector ) {
        similarity += element * std::ranges::count( mRightVector, element );
    } // for ( int64_t element : mLeftVector )
    return similarity;
} // int64_t Challenge02::getSimilarityScore() const

} // namespace aoc
