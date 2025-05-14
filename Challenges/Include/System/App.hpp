/**
 * @file App.hpp
 * @author Alunya
 * @brief Contains the app class declaration.
 * @date 13.05.2025
 */

#ifndef APP_HPP
#define APP_HPP

#include "Include/UI/Menu.hpp"

#include <string>

namespace aoc {
/**
 * @class App
 * @brief Manages the main loop and state of the program.
 * @author Alunya
 * @date 13.05.2025
 *
 * The App class is responsible for managing the main loop and state of
 * the program. It initializes the menu, starts the corresponding challenges
 * and times them.
 */
class App {
public:
    App();
    void run();

private:
    void handleUserInput();

private:
    bool        mRunning;
    std::string mInput;
}; // class App

} // namespace aoc

#endif // ifndef APP_HPP
