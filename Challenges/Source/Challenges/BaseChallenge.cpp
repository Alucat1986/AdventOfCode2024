/**
 * @file BaseChallenge.cpp
 * @author Alunya
 * @brief Contains the abstract base challenge class implementation.
 * @date 27.07.2025
 */

#include "BaseChallenge.hpp"

#include <string>
#include <string_view>

namespace aoc {

// ****************************************************************************************************************** //
//                                                      PUBLIC                                                        //
// ****************************************************************************************************************** //

/**
 * @brief Constructor.
 * @author Alunya
 * @date 30.07.2025
 * @param[in] filePath The path to the input file.
 */
BaseChallenge::BaseChallenge( const std::string_view filePath )
        : mFilePath( filePath ) {} // BaseChallenge::BaseChallenge(...)

// ****************************************************************************************************************** //
//                                                       END                                                          //
// ****************************************************************************************************************** //

} // namespace aoc