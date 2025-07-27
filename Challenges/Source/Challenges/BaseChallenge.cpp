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

BaseChallenge::BaseChallenge( const std::string_view filePath )
        : mFilePath( filePath ) {} // BaseChallenge::BaseChallenge(...)

// ****************************************************************************************************************** //
//                                                       END                                                          //
// ****************************************************************************************************************** //

} // namespace aoc