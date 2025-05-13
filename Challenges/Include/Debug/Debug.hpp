/**
 * @file debug.hpp
 * @author Alunya
 * @brief Contains the Implementation of the debug functions.
 * @date 13.05.2025
 */

#ifndef DEBUG_HPP
#define DEBUG_HPP

#include <iostream>
#include <vector>

namespace debug {

template<typename... Args>
void logMessage( Args... args ) {
#ifdef DEBUG
    ( std::cout << ... << args ) << "\n";
#endif // ifdef DEBUG
}

void logVector( const std::string& prefix, std::vector<long long>& data ) {
#ifdef DEBUG
    std::cout << prefix << " ";
    for ( const long long element : data ) {
        std::cout << element << " ";
    } // for ( const int element : data ) {
    std::cout << "\n";
#endif // ifdef DEBUG
}

} // namespace debug

/*
#ifdef DEBUG
#  define debugLog( ... )                debug::logMessage( __VA_ARGS__ )
#  define debugLogVector( prefix, data ) debug::logVector( prefix, data )
#else
#  define debugLog( ... )
#  define debugLogVector( prefix, data )
#endif // ifdef DEBUG
*/
#endif // ifndef DEBUG_HPP
