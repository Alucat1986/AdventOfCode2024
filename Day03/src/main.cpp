#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

bool readFile(const std::string& filePath, std::vector<std::string>& mulSequence);
unsigned long long findMatches(const std::string& input, const std::string& regularExpression, bool& enabled);
unsigned long long multiplyString(std::string stringToMultiply);

int main() {
	char fileToLoad;
	std::string file = "input\\";
	std::vector<std::string> mulOperations;
	unsigned long long result = 0;
	bool enabled = true;

	std::cout << "Loading (E)xample or (I)nput? ";
	fileToLoad = std::cin.get();

	if ( fileToLoad == 'E' || fileToLoad == 'e' )
		file += "example2.txt";
	else if ( fileToLoad == 'I' || fileToLoad == 'i' )
		file += "input.txt";
	else
		std::cout << "No File found with that name\n";

	readFile(file, mulOperations);
	for ( int pos = 0; pos < mulOperations.size(); pos++ )
		result += findMatches(mulOperations.at(pos), R"(mul\((\d{1,3}),(\d{1,3})\))", enabled);
	std::cout << "Final Result: " << result;
	
	std::cin.get();
}

bool readFile(const std::string& filePath, std::vector<std::string>& mulSequence) {
	std::ifstream fileToRead(filePath);
	std::string line;
	
	if ( !fileToRead.is_open() ) {
		std::cout << "Failed to open " << filePath << " ...\n";
		return false;
	}

	std::cout << "Reading file " << filePath << " ...\n";
	while ( std::getline(fileToRead, line) ) {
		std::cout << "Length: " << line.size() << "\t";
		std::cout << "Line: " << line;
		std::cout << "\n";
		mulSequence.emplace_back(line);
	} // while ( std::getline(fileToRead, line) )

	fileToRead.close();
	return true;
}

unsigned long long findMatches(const std::string& input, const std::string& regularExpression, bool& enabled) {
	std::smatch mulSMatch, doSMatch, dontSMatch;
	std::string tempInput = input;
	unsigned long long result = 0;

	while ( /*tempInput.size() >= 9*/ std::regex_search(tempInput, mulSMatch, std::regex(regularExpression)) ) {
  		//std::regex_search(tempInput, doSMatch, std::regex(R"(do\(\))"));
		//std::regex_search(tempInput, dontSMatch, std::regex(R"(don\'t\(\))"));
		size_t matchPos = tempInput.find(mulSMatch[0]);
		size_t doPos = tempInput.find("do()");
		size_t dontPos = tempInput.find("don't()");
		std::cout << "SMatch: " << mulSMatch[0] << "\n";

		if ( enabled && dontPos < matchPos )
			enabled = false;
		else if ( !enabled && doPos < matchPos )
			enabled = true;

		if ( enabled )
			result += multiplyString(mulSMatch[0]);

		matchPos += 8;
		tempInput = tempInput.substr(matchPos);
	}
	std::cout << "Result: " << result << "\n";
	return result;
}

unsigned long long multiplyString(std::string stringToMultiply) {
	// R"(mul\((\d{1,3}),(\d{1,3})\))"
	unsigned long long result = 0;
	std::smatch multi;

	std::cout << "W: ";
	while ( std::regex_search(stringToMultiply, multi, std::regex(R"((\d{1,3}))")) ) {
		std::cout << multi[0] << " ";

		size_t pos = stringToMultiply.find(multi[0]);
		long long number = std::stoll(multi[0]);
		if ( number >= 100 )
			pos += 3;
		else if ( number >= 10 && number <= 99 )
			pos += 2;
		else if ( number >= 1 && number <= 9 )
			pos++;
		stringToMultiply = stringToMultiply.substr(pos);

		if ( !result )
			result = number;
		else {
			std::cout << " " << result << " * " << number;
			result *= number;
		}
	}

	std::cout << "\n";
	return result;
}