#include <fstream>
#include <iostream>
#include <string>
#include <vector>

bool readFile(const std::string& filePath, std::vector<std::vector<char>>& data);
unsigned int countXmas(const std::vector<std::vector<char>>& data);
unsigned int countMas(const std::vector<std::vector<char>>& data);

int main() {
	char fileToLoad;
	std::string file = "input\\";
	std::vector<std::vector<char>> xmasSearch;

	std::cout << "Loading (E)xample or (I)nput? ";
	fileToLoad = std::cin.get();

	if ( fileToLoad == 'E' || fileToLoad == 'e' )
		file += "example.txt";
	else if ( fileToLoad == 'I' || fileToLoad == 'i' )
		file += "input.txt";
	else
		std::cout << "No File found with that name\n";

	if ( !readFile(file, xmasSearch) )
		std::cout << "Couldn't open file \"" << file << "\"!\n";

	std::cout << "XMAS Amount: " << countXmas(xmasSearch) << "\n";
	std::cout << "MAS Amount: " << countMas(xmasSearch) << "\n";

	std::cin.get();
}

bool readFile(const std::string& filePath, std::vector<std::vector<char>>& data) {
	std::ifstream fileToRead(filePath);
	std::vector<char> lineVector;
	std::string line;
	size_t row = 0;

	if ( !fileToRead.is_open() ) {
		std::cout << "Failed to open " << filePath << " ...\n";
		return false;
	}

	std::cout << "Reading file " << filePath << " ...\n";
	while ( std::getline(fileToRead, line) ) {
		std::cout << "Length: " << line.size() << "\t";
		std::cout << "Line: " << line;
		std::cout << "\n";

		for ( size_t pos = 0; pos < line.size(); pos++ ) {
			lineVector.emplace_back(line.at(pos));
		}

		data.emplace_back(lineVector);
		lineVector.clear();
		row++;
	} // while ( std::getline(fileToRead, line) )

	fileToRead.close();
	return true;
}

unsigned int countXmas(const std::vector<std::vector<char>>& data) {
	unsigned int result = 0;

	for ( size_t row = 0; row < data.size(); row++ ) {
		for ( size_t column = 0; column < data.at(row).size(); column++ ) {
			if ( data.at(row).at(column) != 'X' )
				continue;

			// You can check upwards if the row is low enough
			if ( row >= 3 ) {
				if ( data.at(row - 1).at(column) == 'M' &&
					data.at(row - 2).at(column) == 'A' &&
					data.at(row - 3).at(column) == 'S' )
					result++;

				if ( column >= 3 ) {
					if ( data.at(row - 1).at(column - 1) == 'M' &&
						data.at(row - 2).at(column - 2) == 'A' &&
						data.at(row - 3).at(column - 3) == 'S' )
						result++;
				}

				if ( column <= data.at(row).size() - 4 ) {
					if ( data.at(row - 1).at(column + 1) == 'M' &&
						data.at(row - 2).at(column + 2) == 'A' &&
						data.at(row - 3).at(column + 3) == 'S' )
						result++;
				}
			}

			// You can check downwards if the row is not too low
			if ( row <= data.size() - 4 ) {
				if ( data.at(row + 1).at(column) == 'M' &&
					data.at(row + 2).at(column) == 'A' &&
					data.at(row + 3).at(column) == 'S' )
					result++;
				if ( column >= 3 ) {
					if ( data.at(row + 1).at(column - 1) == 'M' &&
						data.at(row + 2).at(column - 2) == 'A' &&
						data.at(row + 3).at(column - 3) == 'S' )
						result++;
				}

				if ( column <= data.at(row).size() - 4 ) {
					if ( data.at(row + 1).at(column + 1) == 'M' &&
						data.at(row + 2).at(column + 2) == 'A' &&
						data.at(row + 3).at(column + 3) == 'S' )
						result++;
				}
			}

			// You can check to the right if the column is not to close to the right edge
			if ( column <= data.at(row).size() - 4 ) {
				if ( data.at(row).at(column + 1) == 'M' &&
					data.at(row).at(column + 2) == 'A' &&
					data.at(row).at(column + 3) == 'S' )
					result++;
			}

			// You can check to the left if the column is not to close to the left edge
			if ( column >= 3 ) {
				if ( data.at(row).at(column - 1) == 'M' &&
					data.at(row).at(column - 2) == 'A' &&
					data.at(row).at(column - 3) == 'S' )
					result++;
			}
		}
	}
	return result;
}

unsigned int countMas(const std::vector<std::vector<char>>& data) {
	unsigned int result = 0;

	for ( size_t row = 0; row < data.size(); row++ ) {
		for ( size_t column = 0; column < data.at(row).size(); column++ ) {
			if ( data.at(row).at(column) != 'A' )
				continue;

			if ( row >= 1 && column >= 1 && row < data.size() - 1 && column < data.at(row).size() - 1 ) {
				/*
				// Checking for + shapes
				if ( data.at(row - 1).at(column) == 'M' &&
					data.at(row).at(column - 1) == 'M' &&
					data.at(row + 1).at(column) == 'S' &&
					data.at(row).at(column + 1) == 'S' )
					result++;
				
				if ( data.at(row - 1).at(column) == 'S' &&
					data.at(row).at(column - 1) == 'S' &&
					data.at(row + 1).at(column) == 'M' &&
					data.at(row).at(column + 1) == 'M' )
					result++;

				if ( data.at(row - 1).at(column) == 'M' &&
					data.at(row).at(column - 1) == 'S' &&
					data.at(row + 1).at(column) == 'S' &&
					data.at(row).at(column + 1) == 'M' )
					result++;

				if ( data.at(row - 1).at(column) == 'S' &&
					data.at(row).at(column - 1) == 'M' &&
					data.at(row + 1).at(column) == 'M' &&
					data.at(row).at(column + 1) == 'S' )
					result++;
				*/
				// Checking for x shapes
				if ( data.at(row - 1).at(column - 1) == 'M' &&
					data.at(row - 1).at(column + 1) == 'M' &&
					data.at(row + 1).at(column - 1) == 'S' &&
					data.at(row + 1).at(column + 1) == 'S' )
					result++;

				if ( data.at(row - 1).at(column - 1) == 'S' &&
					data.at(row - 1).at(column + 1) == 'S' &&
					data.at(row + 1).at(column - 1) == 'M' &&
					data.at(row + 1).at(column + 1) == 'M' )
					result++;

				if ( data.at(row - 1).at(column - 1) == 'M' &&
					data.at(row - 1).at(column + 1) == 'S' &&
					data.at(row + 1).at(column - 1) == 'M' &&
					data.at(row + 1).at(column + 1) == 'S' )
					result++;

				if ( data.at(row - 1).at(column - 1) == 'S' &&
					data.at(row - 1).at(column + 1) == 'M' &&
					data.at(row + 1).at(column - 1) == 'S' &&
					data.at(row + 1).at(column + 1) == 'M' )
					result++;
			}
		}
	}
	return result;
}