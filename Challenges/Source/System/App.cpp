/**
 * @file App.cpp
 * @author Alunya
 * @brief Contains the app class implementation.
 * @date 24.05.2025
 */

#include "Include/System/App.hpp"

#include "Include/Challenges/Challenge_01.hpp"
#include "Include/Debug/Debug.hpp"
#include "Include/UI/Menu.hpp"

#include <cstdint>
#include <iostream>
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
 * @date 24.05.2025
 */
void App::startChallenge( uint16_t challengeNumber ) {
    std::string path = "Data/Challenge_";
    if ( challengeNumber < 10 ) {
        path += "0";
    }
    path += std::to_string( challengeNumber );
    path += ".txt";

    switch ( challengeNumber ) {
        case 1 : {
            Challenge01 challenge( path );
            challenge.runChallenge();
            break;
        }
        default : debug::logMessage( "Well I shouldn't be here, means this Challenge is not implemented yet." ); break;
    } // switch ( challengeNumber )

    std::cout << "\nPress any key to continue...\n";
    std::cin.ignore(); // "Flush" the leftover input from the previous call of std::cin
    std::cin.get();
}

} // namespace aoc
