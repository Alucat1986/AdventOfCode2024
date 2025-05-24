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
          mInput( 'n' ) {} // App::App()

/**
 * @brief Runs the main loop of the application.
 * @author Alunya
 * @date 24.05.2025
 */
void App::run() {
    while ( mRunning ) {
        std::cout << "=============================================================\n";
        std::cout << "==================== Advent of Code 2025 ====================\n";
        std::cout << "=============================================================\n";
        std::cout << "====== n for next page, p for previous page, q to quit=======\n";
        std::cout << "Select a challenge: ";
        std::cin >> mInput;
        std::cout << "\n";

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

    mInput = 'n';
}

} // namespace aoc
