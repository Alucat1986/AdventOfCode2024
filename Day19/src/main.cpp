#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <queue>
#include <ranges>
#include <unordered_map>
#include <sstream>
#include <string>
#include <vector>

#include "debug.hpp"

using StringVector = std::vector<std::string>;

bool readFile(const std::string& filePath, StringVector& towels, StringVector& toDisplay);
void printVector(const StringVector& toPrint);
std::int64_t sumOfPossibleDesigns(const StringVector& towels, const StringVector& toDisplay);
bool isDesignPossible(const std::string& displayTowel, const StringVector& towels);

int main() {
	char fileToLoad;
	std::string file = "input\\";
	std::int64_t partI = 0, partII = 0;

	std::cout << "Loading (E)xample or (I)nput? ";
	fileToLoad = static_cast<char>(std::cin.get());

	if ( fileToLoad == 'E' || fileToLoad == 'e' )
		file += "example.txt";
	else if ( fileToLoad == 'I' || fileToLoad == 'i' )
		file += "input.txt";
	else
		std::cout << "No File found with that name\n";

	StringVector combinationTowels, towelsToDisplay;
	if ( !readFile(file, combinationTowels, towelsToDisplay) )
		std::cout << "Couldn't open file \"" << file << "\"!\n";

	printVector(combinationTowels);
	std::cout << "Amount of Towels: " << combinationTowels.size() << "\n";
	printVector(towelsToDisplay);
	std::cout << "Amount of Displays: " << towelsToDisplay.size() << "\n";
	
	const auto start = std::chrono::system_clock::now();
	partI = sumOfPossibleDesigns(combinationTowels, towelsToDisplay);
	const auto end = std::chrono::system_clock::now();

	const auto startt = std::chrono::system_clock::now();
	//partII = plutonianPebblesPartII(pebbles);
	const auto endd = std::chrono::system_clock::now();

	std::cout << "\n===========================================================\n";
	std::cout << "PartI:\t" << partI << "\tTime of execution:\t";
	std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - start) << "\n";
	std::cout << "PartII:\t" << partII << "\tTime of execution:\t";
	std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(endd - startt) << "\n";
	std::cout << "===========================================================\n";

	std::cin.get();
}

bool readFile(const std::string& filePath, StringVector& towels, StringVector& toDisplay) {
	std::ifstream fileToRead(filePath);
	std::string line = "";

	if ( !fileToRead.is_open() ) {
		std::cout << "Failed to open " << filePath << " ...\n";
		return false;
	}

	bool displayLines = false;
	std::cout << "\nReading file " << filePath << " ...\n\n";
	while ( std::getline(fileToRead, line) ) {
		if ( line.empty() ) {
			displayLines = true;
			continue;
		}

		debugLog("Length: ", line.size(), "\t", "Line: ", line);

		if ( !displayLines ) {
			std::stringstream sStream(line);
			std::string subString;
			while ( std::getline(sStream, subString, ',') ) {
				subString.erase(0, subString.find_first_not_of(' '));
				subString.erase(subString.find_last_not_of(' ') + 1);
				towels.push_back(subString);
			} // while ( !line.empty() )
			continue;
		} // if ( !displayLines )

		toDisplay.push_back(line);
		
	} // while ( std::getline(fileToRead, line) )

	fileToRead.close();
	std::cout << "\nFinished reading file " << filePath << " ...\n\n";
	return true;
}

void printVector(const StringVector& toPrint) {
	std::cout << "Element: ";
	for ( const auto& element : toPrint ) {
		std::cout << element << " ";
	}
	std::cout << "\n\n";
}

std::int64_t sumOfPossibleDesigns(const StringVector& towels, const StringVector& toDisplay) {
	std::int64_t result = 0;

	for ( const auto& displayTowel : toDisplay ) {
		if ( isDesignPossible(displayTowel, towels) ) {
			debugLog("Towel ", displayTowel, " adds 1 to Score.");
			result++;
		} // if ( isPossible(displayTowel, towels) )
	} // for ( const auto& displayTowel : toDisplay )

	return result;
}

bool isDesignPossible(const std::string& displayTowel, const StringVector& towels) {
	for ( const auto& towel : towels ) {
		debugLog("isDesignPossible? ", displayTowel, " ", towel);
		if ( displayTowel.starts_with(towel) ) {
			//debugLog("Starts with ", towel);
			auto remainingDisplayTowel = displayTowel.substr(towel.size());
			
			if ( remainingDisplayTowel.empty() ) {
				debugLog("Correct Towel: ", displayTowel);
				return true;
			} // if ( remainingDisplayTowel.empty() )

			if ( isDesignPossible(remainingDisplayTowel, towels) ) {
				debugLog("Continuing with ", remainingDisplayTowel);
				return true;
			} // if ( isDesignPossible(remainingDisplayTowel, towel, towels) )

		} // if ( displayTowel.starts_with(towel) )
	} // for ( const auto& towel : towels )
	return false;
}