/**
 * @file Menu.hpp
 * @author Alunya
 * @brief Contains the menu class declaration.
 * @date 24.05.2025
 */

#ifndef MENU_HPP
#define MENU_HPP

#include <cstdint>

namespace aoc {

using std::uint16_t;

/**
 * @class Menu
 * @brief Manages the console menu.
 * @author Alunya
 * @date 24.05.2025
 */
class Menu {
public:
    Menu();
    ~Menu() = default;

    void showMenu() const;
    void nextPage();
    void previousPage();
    bool selectChallenge( int challengeNumber );

private:
    void displayChallenges() const;

private:
    uint16_t       mCurrentPage;
    uint16_t       mSelectedChallenge;
    const uint16_t mChallengesPerPage;
    const uint16_t mTotalChallenges;
    const uint16_t mTotalPages;
}; // class Menu

} // namespace aoc

#endif // ifndef MENU_HPP
