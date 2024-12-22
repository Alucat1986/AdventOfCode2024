#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <ranges>
#include <unordered_map>
#include <string>
#include <vector>

#include "debug.hpp"

/*
* numeric Keypad(start on 'A'):
* +---+---+---+
* | 7 | 8 | 9 |
* +---+---+---+
* | 4 | 5 | 6 |
* +---+---+---+
* | 1 | 2 | 3 |
* +---+---+---+
*     | 0 | A |
*     +---+---+
* 
* directional Keypad(start on 'A'):
*     +---+---+
*     | ^ | A |
* +---+---+---+
* | < | v | > |
* +---+---+---+
* 
* Human(directional) -> Robot(directional) -> Robot(directional) -> Robot(numeric)
*/


bool readFile(const std::string& filePath, std::vector<long long>& data);

int main() {
	char fileToLoad;
	std::string file = "input\\";

	std::cout << "Loading (E)xample or (I)nput? ";
	fileToLoad = std::cin.get();

	if ( fileToLoad == 'E' || fileToLoad == 'e' )
		file += "example.txt";
	else if ( fileToLoad == 'I' || fileToLoad == 'i' )
		file += "input.txt";
	else
		std::cout << "No File found with that name\n";

	if ( !readFile(file, pebbles) )
		std::cout << "Couldn't open file \"" << file << "\"!\n";

	const auto start = std::chrono::system_clock::now();
	partI = plutonianPebblesPartI(pebbles);
	const auto end = std::chrono::system_clock::now();

	const auto startt = std::chrono::system_clock::now();
	partII = plutonianPebblesPartII(pebbles);
	const auto endd = std::chrono::system_clock::now();

	std::cout << "\n===========================================================\n";
	std::cout << "PartI:\t" << partI.size() << "\tTime of execution:\t";
	std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - start) << "\n";
	std::cout << "PartII:\t" << partII << "\tTime of execution:\t";
	std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(endd - startt) << "\n";
	std::cout << "===========================================================\n";

	std::cin.get();
}

bool readFile(const std::string& filePath, std::vector<long long>& data) {
	std::ifstream fileToRead(filePath);
	std::string line = "";

	if ( !fileToRead.is_open() ) {
		std::cout << "Failed to open " << filePath << " ...\n";
		return false;
	}

	std::cout << "\nReading file " << filePath << " ...\n\n";
	std::getline(fileToRead, line);

	debugLog("Length: ", line.size(), "\t", "Line: ", line);
	fileToRead.close();

	for ( std::size_t pos = line.find(" "); pos != std::string::npos; pos = line.find(" ") ) {
		data.push_back(std::stoi(line.substr(0, pos)));
		line = line.substr(pos + 1);
		debugLog("New line: ", line);
	} // for ( std::size_t pos = line.find(" "); pos != std::string::npos; pos = line.find(" ") )
	// Adding the last stone
	data.push_back(std::stoi(line));

	std::cout << "\nFinished reading file " << filePath << " ...\n\n";
	return true;
}

std::vector<long long> plutonianPebblesPartI(const std::vector<long long>& data) {
	std::vector<long long> newStones, tempStones = data;
	int blinks = 25;

	while ( blinks > 0 ) {
		newStones.clear();
		for ( long long pebble : tempStones ) {
			newStones.append_range(applyRules(pebble));
		} // for ( long long pebble : data )
		debugLogVector("Vector: ", newStones);
		std::cout << "Blink: " << blinks << "\n";
		tempStones = newStones;
		blinks--;
	} // while ( blinks > 0 )

	return newStones;
}

std::vector<long long> applyRules(const long long element) {
	if ( !element ) {
		debugLog("Stone was 0!");
		return std::vector<long long>{1};
	} else if ( std::to_string(element).size() % 2 == 0 ) {
		debugLog("Stone was of even amount of digits: ", element);
		std::size_t length = std::to_string(element).size();
		std::string leftPart = std::to_string(element).substr(0, length / 2);
		std::string rightPart = std::to_string(element).substr(length / 2);
		return std::vector<long long>{std::stoll(leftPart), std::stoll(rightPart)};
	} else {
		debugLog("multiply by 2024: ", element);
		return std::vector<long long>{element * 2024};
	}
}

std::int64_t plutonianPebblesPartII(const std::vector<long long>& data) {
	std::int64_t result = 0;
	std::int64_t blinks = 75;
	std::unordered_map<Stone, std::int64_t, StoneHash> cache;
	
	for ( auto pebble : data ) {
		result += calculatePebbles(cache, { pebble, blinks });
	}

	return result;
}

PebbleResult applyRulesPartII(Stone pebble) {
	const auto toBlink = pebble.TimesToBlink - 1;
	if ( !pebble.Value ) {
		debugLog("Stone was 0!");
		return PebbleResult{ { 1, toBlink }, { NoStone, toBlink } };
	} else if ( std::to_string(pebble.Value).size() % 2 == 0 ) {
		debugLog("Stone was of even amount of digits: ", pebble.Value);
		std::size_t length = std::to_string(pebble.Value).size();
		std::string leftPart = std::to_string(pebble.Value).substr(0, length / 2);
		std::string rightPart = std::to_string(pebble.Value).substr(length / 2);
		return PebbleResult{ { std::stoll(leftPart), toBlink }, { std::stoll(rightPart), toBlink } };
	} else {
		debugLog("multiply by 2024: ", pebble.Value);
		return PebbleResult{ { pebble.Value * 2024, toBlink }, { NoStone, toBlink } };
	}
}

std::int64_t calculatePebbles(std::unordered_map<Stone, std::int64_t, StoneHash>& cache, Stone currentPebble) {
	if ( currentPebble.TimesToBlink == 0 ) {
		return 1;
	} // if ( currentPebble.TimesToBlink == 0 )

	auto iter = cache.find(currentPebble);
	if ( iter != cache.end() ) {
		return iter->second;
	} // if ( iter != cache.end() )

	const auto pebbleResult = applyRulesPartII(currentPebble);
	auto result = calculatePebbles(cache, pebbleResult.FirstPebble);
	if ( pebbleResult.SecondPebble.Value != NoStone ) {
		result += calculatePebbles(cache, pebbleResult.SecondPebble);
	} // if ( pebbleResult.SecondPebble.Value != NoStone )

	cache.insert({ currentPebble, result });
	return result;
}