/**
 * @file Challenge03.cpp
 * @author Alunya
 * @brief Contains the day 03 challenge implementation.
 * @date 04.08.2025
 */

#include "Challenge03.hpp"

#include "BaseChallenge.hpp"
#include "../Debug/Debug.hpp"
#include "../Utils/ChallengeResult.hpp"

#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <string_view>
#include <vector>

namespace aoc {

// ****************************************************************************************************************** //
//                                                      PUBLIC                                                        //
// ****************************************************************************************************************** //

/**
 * @brief Constructor.
 * @author Alunya
 * @date 04.08.2025
 * @param[in] filePath The path to the input file.
 */
Challenge03::Challenge03( const std::string_view filePath )
        : BaseChallenge( filePath ) {
    if ( !readFile() ) {
        debug::logMessage( "Challenge 03:: Initialisation failed: File not loaded", mFilePath );
        return;
    } // if ( !readFile() )
} // Challenge03::Challenge03(...)

/**
 * @brief Runs the challenge.
 * @author Alunya
 * @date 04.08.2025
 * @return The result of the challenge including their execution times.
 */
ChallengeResult Challenge03::runChallenge() {
    bool enabled = true;

    for ( const std::string& text : mMulOperations ) {
        mResult.partI += findMatches( text, R"(mul\((\d{1,3}),(\d{1,3})\))", enabled );
    }
    return mResult;
} // ChallengeResult Challenge03::runChallenge(...)

// ****************************************************************************************************************** //
//                                                      PRIVATE                                                       //
// ****************************************************************************************************************** //

/**
 * @brief Reads the input file and populates the left and right vectors.
 * @author Alunya
 * @date 04.08.2025
 * @return True if the file was read successfully, false otherwise.
 */
bool Challenge03::readFile() {
    std::ifstream fileToRead( mFilePath );
    std::string   line;

    if ( !fileToRead.is_open() ) {
        std::cout << "Failed to open " << mFilePath << " ...\n";
        return false;
    }

    std::cout << "Reading file " << mFilePath << " ...\n";
    while ( std::getline( fileToRead, line ) ) {
        std::cout << "Length: " << line.size() << "\t";
        std::cout << "Line: " << line;
        std::cout << "\n";
        mMulOperations.emplace_back( line );
    } // while ( std::getline(fileToRead, line) )

    return true;
} // bool Challenge03::readFile(...)

/**
 * @brief Finds matches in the input string based on the provided regular expression.
 * @author Alunya
 * @date 04.08.2025
 * @param[in] input The input string to search.
 * @param[in] regularExpression The regular expression to use for matching.
 * @param[in] enabled A flag indicating whether matching is enabled.
 * @return std::uint64_t The number of matches found.
 */
std::int64_t Challenge03::findMatches( const std::string& input, const std::string& regularExpression,
                                       bool enabled ) const {
    std::smatch  mulSMatch, doSMatch, dontSMatch;
    std::string  tempInput = input;
    std::int64_t result    = 0;

    while ( /*tempInput.size() >= 9*/ std::regex_search( tempInput, mulSMatch, std::regex( regularExpression ) ) ) {
        // std::regex_search(tempInput, doSMatch, std::regex(R"(do\(\))"));
        // std::regex_search(tempInput, dontSMatch, std::regex(R"(don\'t\(\))"));
        std::size_t matchPos = tempInput.find( mulSMatch[0] );
        std::size_t doPos    = tempInput.find( "do()" );
        std::size_t dontPos  = tempInput.find( "don't()" );
        std::cout << "SMatch: " << mulSMatch[0] << "\n";

        if ( enabled && dontPos < matchPos ) {
            enabled = false;
        }
        else if ( !enabled && doPos < matchPos ) {
            enabled = true;
        }

        if ( enabled ) {
            result += multiplyString( mulSMatch[0] );
        }

        matchPos  += 8;
        tempInput  = tempInput.substr( matchPos );
    }
    std::cout << "Result: " << result << "\n";
    return result;
} // std::uint64_t Challenge03::findMatches(...) const

/**
 * @brief Multiplies the numbers in the given string.
 * @author Alunya
 * @date 04.08.2025
 * @param[in] stringToMultiply The string containing the numbers to multiply.
 * @return The result of the multiplication.
 */
std::int64_t Challenge03::multiplyString( std::string stringToMultiply ) const {
    // R"(mul\((\d{1,3}),(\d{1,3})\))"
    std::int64_t result{ 0 };
    std::smatch  multi;

    std::cout << "W: ";
    while ( std::regex_search( stringToMultiply, multi, std::regex( R"((\d{1,3}))" ) ) ) {
        std::cout << multi[0] << " ";

        std::size_t pos    = stringToMultiply.find( multi[0] );
        long long   number = std::stoll( multi[0] );
        if ( number >= 100 ) {
            pos += 3;
        }
        else if ( number >= 10 && number <= 99 ) {
            pos += 2;
        }
        else if ( number >= 1 && number <= 9 ) {
            pos++;
        }
        stringToMultiply = stringToMultiply.substr( pos );

        if ( !result ) {
            result = number;
        }
        else {
            std::cout << " " << result << " * " << number;
            result *= number;
        }
    }

    std::cout << "\n";
    return result;
} // std::int64_t Challenge03::multiplyString(...) const

} // namespace aoc