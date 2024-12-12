#ifndef DEBUG_HPP
#define DEBUG_HPP

#include <iostream>
#include <vector>

template<typename... Args>
void logMessage(Args... args) {
	(std::cout << ... << args) << "\n";
}

void logVector(const std::string& prefix, std::vector<long long>& data) {
	std::cout << prefix << " ";
	for ( const long long element : data ) {
		std::cout << element << " ";
	} // for ( const int element : data ) {
	std::cout << "\n";
}

#ifdef DEBUG
#define debugLog(...) logMessage(__VA_ARGS__)
#define debugLogVector(prefix, data) logVector(prefix, data)
#else
#define debugLog(...)
#define debugLogVector(prefix, data)
#endif // ifdef DEBUG

#endif // ifndef DEBUG_HPP