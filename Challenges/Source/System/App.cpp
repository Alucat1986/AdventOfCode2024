/**
 * @file App.cpp
 * @author Alunya
 * @brief Contains the app class implementation.
 * @date 04.08.2025
 */

#include "App.hpp"

#include "../Challenges/Challenge01.hpp"
#include "../Challenges/Challenge02.hpp"
#include "../Debug/Debug.hpp"
#include "../UI/Menu.hpp"
#include "../Utils/ChallengeResult.hpp"

#include <cstdint>
#include <iosfwd>
#include <iostream>
#include <limits>
#include <string>

namespace aoc {

using std::uint16_t;

/**
 * @brief Constructor.
 * @author Alunya
 * @date 24.05.2025
 */
App::App()
        : mRunning( true ),
          mInput( 'o' ) {} // App::App()

/**
 * @brief Runs the main loop of the application.
 * @author Alunya
 * @date 24.05.2025
 */
void App::run() {
    while ( mRunning ) {
        mMenu.showMenu();
        std::cout << "Select a challenge: ";
        std::cin >> mInput;
        std::cout << "\n\n";

        handleUserInput();
    } // while ( !exit )
} // void App::Run( ... )

// ****************************************************************************************************************** //
//                                                      PRIVATE                                                       //
// ****************************************************************************************************************** //

/**
 * @brief Handles user input.
 * @author Alunya
 * @date 24.05.2025
 */
void App::handleUserInput() {
    if ( mInput == 'q' || mInput == 'Q' ) {
        mRunning = false;
        debug::logMessage( "Exiting program..." );
        return;
    } // if ( input == "q" )

    if ( mInput == 'n' || mInput == 'N' ) {
        mMenu.nextPage();
    } // if ( mInput == 'n' || mInput == 'N' )
    else if ( mInput == 'p' || mInput == 'P' ) {
        mMenu.previousPage();
    } // else if ( mInput == 'p' || mInput == 'P' )
    else if ( mInput == 'r' || mInput == 'R' ) {
        startChallenge( mMenu.getSelectedChallenge() );
    } // else if ( mInput == 'r' || mInput == 'R' )
    else if ( mInput >= '0' && mInput <= '9' ) {
        mMenu.selectChallenge( mInput - '0' );
    } // else if ( mInput >= '0' && mInput <= '9' )

    mInput = 'o';
} // void App::handleUserInput(...)

/**
 * @brief Starts the selected challenge.
 * @param[in] challengeNumber The number of the challenge to start.
 * @author Alunya
 * @date 04.08.2025
 */
void App::startChallenge( uint16_t challengeNumber ) {
    std::string path = "Data/Challenge";
    if ( challengeNumber < 10 ) {
        path += "0";
    } // if ( challengeNumber < 10 )
    path += std::to_string( challengeNumber );
    path += ".txt";

    ChallengeResult result;

    switch ( challengeNumber ) {
        case 1 : {
            Challenge01 challenge( path );
            result = challenge.runChallenge();
            break;
        } // case 1
        case 2 : {
            Challenge02 challenge( path );
            result = challenge.runChallenge();
            break;
        } // case 2
        default :
            debug::logMessage( "Well I shouldn't be here, means this Challenge is not implemented yet." );
            break;
    } // switch ( challengeNumber )

    printResults( result );

    std::cout << "\nPress Enter to continue...\n";
    // "Flush" the leftover input from the previous call of std::cin
    std::cin.ignore( std::numeric_limits<std::streamsize>::max(), '\n' );
    std::cin.get();
} // void App::startChallenge(...)

/**
 * @brief Prints the results of the challenge to the screen.
 * @author Alunya
 * @date 30.07.2025
 * @param[in] result The results to be printed.
 */
void App::printResults( const ChallengeResult& result ) const {
    std::cout << "\n===========================================================\n";
    std::cout << std::left << std::setw( 8 ) << "PartI:" << std::right << std::setw( 10 ) << result.partI << std::left
              << std::setw( 14 ) << "  Time of execution:" << std::right << std::setw( 10 )
              << std::chrono::duration_cast<std::chrono::microseconds>( result.executionTimePartI ) << "\n";
    std::cout << std::left << std::setw( 8 ) << "PartII:" << std::right << std::setw( 10 ) << result.partII << std::left
              << std::setw( 14 ) << "  Time of execution:" << std::right << std::setw( 10 )
              << std::chrono::duration_cast<std::chrono::microseconds>( result.executionTimePartII ) << "\n";
    std::cout << "===========================================================\n";
} // void App::printResults(...)

} // namespace aoc