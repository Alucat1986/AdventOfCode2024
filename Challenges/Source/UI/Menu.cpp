/**
 * @file Menu.cpp
 * @author Alunya
 * @brief Contains the menu class implementation.
 * @date 04.08.2025
 */

#include "Menu.hpp"

#include "../Debug/Debug.hpp"

#include <cstdint>
#include <iostream>

namespace aoc {

/**
 * @brief Constructor.
 * @author Alunya
 * @date 24.05.2025
 */
Menu::Menu()
        : mCurrentPage( 0 ),
          mSelectedChallenge( 1 ),
          mChallengesPerPage( 10 ),
          mTotalChallenges( 25 ),
          mTotalPages( ( mTotalChallenges + mChallengesPerPage - 1 ) / mChallengesPerPage ) {} // Menu::Menu( ... )

/**
 * @brief Displays the console menu.
 * @author Alunya
 * @date 24.05.2025
 */
void Menu::showMenu() const {
    std::cout << "\033[H\033[J";
    std::cout << "=============================================================\n";
    std::cout << "==================== Advent of Code 2024 ====================\n";
    std::cout << "=============================================================\n";
    std::cout << "\n";
    std::cout << "=============== Next Page(n), Previous Page(p) ==============\n";
    std::cout << "=================  Run Challenge(r),Quit(q) =================\n";
    std::cout << "\n";
    displayChallenges();
    std::cout << "-------------------------------------------------------------\n";
    std::cout << "Page (" << mCurrentPage + 1 << "/" << mTotalPages << ") - ";
    std::cout << "Selected challenge: " << mSelectedChallenge << "\n";
    std::cout << "=============================================================\n";
} // void Menu::showMenu(...) const

/**
 * @brief Navigates to the next page.
 * @author Alunya
 * @date 24.05.2025
 */
void Menu::nextPage() {
    if ( mCurrentPage < mTotalPages - 1 ) {
        mCurrentPage++;
    } // if ( mCurrentPage < mTotalPages - 1 )
    else if ( mCurrentPage == mTotalPages - 1 ) {
        mCurrentPage = 0;
    } // else if ( mCurrentPage == mTotalPages - 1 )
} // Menu::nextPage(...)

/**
 * @brief Navigates to the previous page.
 * @author Alunya
 * @date 24.05.2025
 */
void Menu::previousPage() {
    if ( mCurrentPage > 0 ) {
        mCurrentPage--;
    } // if ( mCurrentPage > 0 )
    else if ( mCurrentPage == 0 ) {
        mCurrentPage = mTotalPages - 1;
    } // else if ( mCurrentPage == 0 )
} // Menu::previousPage(...)

/**
 * @brief Selects a challenge.
 * @author Alunya
 * @date 24.05.2025
 * @param[in] challengeNumber The number of the challenge to select.
 * @return True if the challenge was selected successfully, false otherwise.
 */
bool Menu::selectChallenge( int challengeNumber ) {
    if ( challengeNumber + ( mCurrentPage * mChallengesPerPage ) > mTotalChallenges - 1 ) {
        return false;
    }

    mSelectedChallenge = challengeNumber + ( mCurrentPage * mChallengesPerPage ) + 1;
    debug::logMessage( "Selected challenge: ", mSelectedChallenge );
    return true;
} // Menu::selectChallenge(...)

/**
 * @brief Gets the currently selected challenge.
 * @author Alunya
 * @date 04.08.2025
 * @return The currently selected challenge.
 */
std::uint16_t Menu::getSelectedChallenge() const {
    return mSelectedChallenge;
} // Menu::getSelectedChallenge(...) const

/**
 * @brief Displays the challenges on the current page.
 * @author Alunya
 * @date 04.08.2025
 */
void Menu::displayChallenges() const {
    for ( std::uint16_t i = 0; i < mChallengesPerPage && ( mCurrentPage * mChallengesPerPage + i ) < mTotalChallenges;
          ++i ) {
        std::cout << i << ": Challenge " << ( mCurrentPage * mChallengesPerPage + i + 1 ) << "\n";
    } // for ( std::uint16_t i = 0; ... )
} // void Menu::displayChallenges(...) const

} // namespace aoc
