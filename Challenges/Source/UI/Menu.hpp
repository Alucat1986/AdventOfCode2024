/**
 * @file Menu.hpp
 * @author Alunya
 * @brief Contains the menu class declaration.
 * @date 04.08.2025
 */

#ifndef MENU_HPP
#define MENU_HPP

#include <cstdint>

namespace aoc {

/**
 * @class Menu
 * @brief Manages the console menu.
 * @author Alunya
 * @date 04.08.2025
 */
class Menu {
public:
    Menu();
    ~Menu() = default;

    void          showMenu() const;
    void          nextPage();
    void          previousPage();
    bool          selectChallenge( int challengeNumber );
    std::uint16_t getSelectedChallenge() const;

private:
    void displayChallenges() const;

private:
    std::uint16_t       mCurrentPage;
    std::uint16_t       mSelectedChallenge;
    const std::uint16_t mChallengesPerPage;
    const std::uint16_t mTotalChallenges;
    const std::uint16_t mTotalPages;
}; // class Menu

} // namespace aoc

#endif // ifndef MENU_HPP
