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
