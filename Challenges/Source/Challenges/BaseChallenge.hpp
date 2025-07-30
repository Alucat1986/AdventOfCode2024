/**
 * @file BaseChallenge.hpp
 * @author Alunya
 * @brief Contains the abstract base challenge class declaration.
 * @date 27.07.2025
 */

#ifndef BASE_CHALLENGE_HPP
#define BASE_CHALLENGE_HPP

#include <string>
#include <string_view>

namespace aoc {

/**
 * @class BaseChallenge
 * @brief Abstract class for challenges.
 * @author Alunya
 * @date 30.07.2025
 *
 * The BaseChallenge class serves as a blueprint for all challenge implementations.
 * It provides a common interface and shared functionality for reading input files,
 * running challenges, and storing results.
 */
class BaseChallenge {
public:
    BaseChallenge( const std::string_view filePath );
    virtual ~BaseChallenge()    = default;

    virtual void runChallenge() = 0;

protected:
    virtual bool readFile() = 0;

protected:
    const std::string mFilePath;

}; // class BaseChallenge

} // namespace aoc

#endif // ifndef BASE_CHALLENGE_HPP