/**
 * @file App.cpp
 * @author Alunya
 * @brief Contains the app class implementation.
 * @date 13.05.2025
 */

#include "Include/System/App.hpp"

#include "Include/Debug/Debug.hpp"
#include "Include/UI/Menu.hpp"

#include <iostream>
#include <string>

namespace aoc {

App::App()
        : mRunning( true ),
          mInput( "0" ) {} // App::App()

void App::run() {
    while ( mRunning ) {
        std::cout << "=============================================================\n";
        std::cout << "==================== Advent of Code 2025 ====================\n";
        std::cout << "=============================================================\n";
        std::cout << "0-9 for menu, n for next page, p for previous page, q to quit\n";
        std::cout << "Select a challenge: ";
        std::cin >> mInput;
        std::cout << "\n";

        handleUserInput();
    } // while ( !exit )
    return;
} // void App::Run( ... )

void App::handleUserInput() {
    if ( mInput == "q" ) {
        mRunning = false;
        debug::logMessage( "Exiting program..." );
        mInput = "0";
        return;
    } // if ( input == "q" )

    return;
}

} // namespace aoc
