/**
 * @file App.cpp
 * @author Alunya
 * @brief Contains the app class implementation.
 * @date 24.05.2025
 */

#include "Include/System/App.hpp"

#include "Include/Debug/Debug.hpp"
#include "Include/UI/Menu.hpp"

#include <iostream>

namespace aoc {

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
    else if ( mInput >= '0' && mInput <= '9' ) {
        mMenu.selectChallenge( mInput - '0' );
    } // else if ( mInput >= '0' && mInput <= '9' )

    mInput = 'o';
}

} // namespace aoc
