/**
 * @file Challenge_01.cpp
 * @author Alunya
 * @brief Contains the day 01 challenge implementation.
 * @date 26.07.2025
 */

#include "Challenge_01.hpp"

#include "../Debug/Debug.hpp"

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

void Challenge01::runChallenge() {
    const auto partOneStart = std::chrono::system_clock::now();
    partI                   = getDistanceBetweenVectors();
    const auto partOneEnd   = std::chrono::system_clock::now();

    const auto partTwoStart = std::chrono::system_clock::now();
    partII                  = getSimilarityScore();
    const auto partTwoEnd   = std::chrono::system_clock::now();

    std::cout << "\n===========================================================\n";
    std::cout << std::left << std::setw( 8 ) << "PartI:" << std::right << std::setw( 10 ) << partI << std::left
              << std::setw( 14 ) << "  Time of execution:" << std::right << std::setw( 10 )
              << std::chrono::duration_cast<std::chrono::microseconds>( partOneEnd - partOneStart ) << "\n";
    std::cout << std::left << std::setw( 8 ) << "PartII:" << std::right << std::setw( 10 ) << partII << std::left
              << std::setw( 14 ) << "  Time of execution:" << std::right << std::setw( 10 )
              << std::chrono::duration_cast<std::chrono::microseconds>( partTwoEnd - partTwoStart ) << "\n";
    std::cout << "===========================================================\n";
} // void Challenge01::RunChallenge(...)

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

int64_t Challenge01::getDistanceBetweenVectors() {
    size_t  iterator{ 0 };
    int64_t distance{ 0 };

    for ( int64_t element : mLeftVector ) {
        distance += std::abs( element - mRightVector.at( iterator ) );
        iterator++;
    }
    return distance;
} // int64_t Challenge01::getDistanceBetweenVectors(...)

int64_t Challenge01::getSimilarityScore() {
    size_t  iterator{ 0 };
    int64_t similarity{ 0 };

    for ( int64_t element : mLeftVector ) {
        similarity += element * std::ranges::count( mRightVector, element );
    } // for ( int element : leftSide )
    return similarity;
} // int64_t Challenge01::getSimilarityScore(...)

} // namespace aoc
