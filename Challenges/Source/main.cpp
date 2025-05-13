/**
 * @file main.cpp
 * @author Alunya
 * @brief Contains the main function.
 * @date 13.05.2025
 */

#include <iostream>

int main( int argc, char* argv[] ) {
    std::cout << "Hello, World!" << "\n";
    std::cout << "Arguments: " << argc << "\n";
    for ( size_t i = 0; i < argc; i++ ) {
        std::cout << argv[i] << "\n";
    } // for ( size_t i = 0; i < argc; i++ )
} // int main( int argc, char* argv[] )
