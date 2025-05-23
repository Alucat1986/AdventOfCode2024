/**
 * @file challenge_01.cpp
 * @author Alunya
 * @brief Contains the day 01 challenge implementation.
 * @date 13.05.2025
 */

#include "Include/Challenges/Challenge_01.hpp"

#include "Include/Debug/Debug.hpp"

#include <algorithm>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

namespace aoc {

Challenge01::Challenge01( const std::string& filePath )
        : mFilePath( filePath ) {
    if ( !readFile() ) {
        debug::logMessage( "Challenge 01:: Initialisation failed: File not loaded", mFilePath );
        return;
    } // if ( !readFile() )

    std::ranges::sort( mLeftVector );
    std::ranges::sort( mRightVector );

    debug::logMessage( "Challenge 01 initialized with file: ", mFilePath );
} // Challenge01::Challenge01( ... )

void Challenge01::runChallenge() {
    std::cout << "Summed Distance: ";
    std::cout << getDistanceBetweenVectors() << "\n";

    std::cout << "Summed Similarity: ";
    std::cout << getSimilarityScore() << "\n";
} // void Challenge01::RunChallenge( ... )

bool Challenge01::readFile() {
    std::ifstream fileToRead( mFilePath );
    std::string   line = "", tmpSubString = "";
    size_t        position{ 0 };

    if ( !fileToRead.is_open() ) {
        std::cout << "Failed to open " << mFilePath << " ...\n";
        return false;
    }

    std::cout << "Reading file " << mFilePath << " ...\n";
    while ( std::getline( fileToRead, line ) ) {
        std::cout << "Line read: " << line << " Length(" << line.size() << "): ";

        position     = line.find_first_of( ' ' );
        tmpSubString = line.substr( 0, position );
        mLeftVector.emplace_back( std::stoi( tmpSubString ) );
        std::cout << std::stoi( tmpSubString ) << " / ";

        position     = line.find_last_of( ' ' );
        tmpSubString = line.substr( position, line.size() );
        mRightVector.emplace_back( std::stoi( tmpSubString ) );
        std::cout << std::stoi( tmpSubString ) << "\n";
    } // while ( std::getline(fileToRead, line) )
    return true;
}

std::int64_t Challenge01::getDistanceBetweenVectors() {
    size_t       iterator{ 0 };
    std::int64_t distance{ 0 };

    for ( std::int64_t element : mLeftVector ) {
        distance += std::abs( element - mRightVector.at( iterator ) );
        iterator++;
    }
    return distance;
}

std::int64_t Challenge01::getSimilarityScore() {
    size_t       iterator{ 0 };
    std::int64_t similarity{ 0 };

    for ( std::int64_t element : mLeftVector ) {
        similarity += element * std::ranges::count( mRightVector, element );
    } // for ( int element : leftSide )
    return similarity;
}

} // namespace aoc
