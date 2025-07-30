/**
 * @file App.hpp
 * @author Alunya
 * @brief Contains the app class declaration.
 * @date 24.05.2025
 */

#ifndef APP_HPP
#define APP_HPP

#include "../UI/Menu.hpp"

namespace aoc {

using std::uint16_t;

/**
 * @class App
 * @brief Manages the main loop and state of the program.
 * @author Alunya
 * @date 24.05.2025
 *
 * The App class is responsible for managing the main loop and state of
 * the program. It initializes the menu, starts the corresponding challenges
 * and times them.
 */
class App {
public:
    App();
    ~App() = default;
    void run();

private:
    void handleUserInput();
    void startChallenge( uint16_t challengeNumber );

private:
    bool mRunning;
    char mInput;

    Menu mMenu;
}; // class App

} // namespace aoc

#endif // ifndef APP_HPP