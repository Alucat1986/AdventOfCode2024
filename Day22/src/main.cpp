#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <numeric>
#include <ranges>
#include <string>
#include <vector>

#include "debug.hpp"

/*
* Steps:
* 1) Multiply secret number by 64, then mix it into the secret number, then prune it.
* 2) Divide secret number by 32, round it down to nearest integer, then mix into secret number, prune it.
* 3) Multiply secret number by 2048, then mix it and prune it.
* 
* Mixing it:
* Bitwise XOR of secret number and given value, e.g. 42 ^ 15 = 37
* 
* Pruning it:
* Secret Number modulo 16777216, e.g. 100'000'000 % 16'777'216 = 16'113'920.
*/

struct Buyer {
	std::int64_t InitialNumber;
	std::int64_t TwoThousandthNumber;
};

bool readFile(const std::string& filePath, std::vector<Buyer>& data);
std::int64_t calculateSumOfAllBuyers(std::vector<Buyer>& buyers);
std::int64_t mixAndPrune(const std::int64_t& currentNumber, const std::int64_t& value);
void multiplyMixAndPrune(std::int64_t& currentNumber, const std::int64_t& value);
void divideMixAndPrune(std::int64_t& currentNumber, const std::int64_t& value);

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

	std::vector<Buyer> buyers;
	if ( !readFile(file, buyers) )
		std::cout << "Couldn't open file \"" << file << "\"!\n";

	const auto start = std::chrono::system_clock::now();
	std::int64_t partI = calculateSumOfAllBuyers(buyers);
	const auto end = std::chrono::system_clock::now();

	const auto startt = std::chrono::system_clock::now();
	std::int64_t partII = calculateSumOfAllBuyers(buyers);
	const auto endd = std::chrono::system_clock::now();

	std::cout << "\n===========================================================\n";
	std::cout << "PartI:\t" << partI << "\tTime of execution:\t";
	std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - start) << "\n";
	std::cout << "PartII:\t" << partII << "\tTime of execution:\t";
	std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(endd - startt) << "\n";
	std::cout << "===========================================================\n";

	std::cin.get();
}

bool readFile(const std::string& filePath, std::vector<Buyer>& data) {
	std::ifstream fileToRead(filePath);
	std::string line = "";

	if ( !fileToRead.is_open() ) {
		std::cout << "Failed to open " << filePath << " ...\n";
		return false;
	}

	std::cout << "\nReading file " << filePath << " ...\n\n";
	while ( std::getline(fileToRead, line) ) {
		debugLog("Length: ", line.size(), "\t", "Line: ", line);
		data.emplace_back(Buyer{ std::stoll(line), 0 });
	} // while ( std::getline(fileToRead, line) )

	fileToRead.close();

	std::cout << "\nFinished reading file " << filePath << " ...\n\n";
	return true;
}

std::int64_t calculateSumOfAllBuyers(std::vector<Buyer>& buyers) {
	for ( auto& buyer : buyers ) {
		std::int64_t currentSecretNumber = buyer.InitialNumber;
		for ( std::int64_t counter = 0; counter < 2000; counter++ ) {
			multiplyMixAndPrune(currentSecretNumber, 64);
			divideMixAndPrune(currentSecretNumber, 32);
			multiplyMixAndPrune(currentSecretNumber, 2048);
		} // for ( std::int64_t counter = 0; counter < 2000; counter++ )
		buyer.TwoThousandthNumber = currentSecretNumber;
	} // for ( auto& buyer : buyers )
	return std::ranges::fold_left(buyers, 0, [](std::int64_t sum, const Buyer& buyer) {
		return sum + buyer.TwoThousandthNumber; });
}

std::int64_t mixAndPrune(const std::int64_t& currentNumber, const std::int64_t& value) {
	return (currentNumber ^ value) % 16'777'216;
}

void multiplyMixAndPrune(std::int64_t& currentNumber, const std::int64_t& value) {
	std::int64_t result = currentNumber * value;
	currentNumber = mixAndPrune(currentNumber, result);
}

void divideMixAndPrune(std::int64_t& currentNumber, const std::int64_t& value) {
	std::int64_t result = static_cast<std::int64_t>(currentNumber / value);
	currentNumber = mixAndPrune(currentNumber, result);
}