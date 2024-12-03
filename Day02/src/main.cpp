#include <algorithm>
//#include <array>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

//bool readFile(const std::string& filePath, std::array<int, 200>& leftSide, std::array<int, 200>& rightSide);
//void printArray(const std::array<int, 200>& arrayToPrint);
bool readFile(const std::string& filePath, std::vector<std::vector<int>>& reports);
void printVector(const std::vector<int>& vectorToPrint);

bool isSafeReport(const std::vector<int>& vectorToCount);
int isSavable(std::vector<int> vectorToVerify);

int main() {
	char fileToLoad;
	int safeReports = 0;
	std::string file = "input\\";
	std::vector<std::vector<int>> reportVector;

	std::cout << "Loading (E)xample or (I)nput? ";
	fileToLoad = std::cin.get();

	if ( fileToLoad == 'E' || fileToLoad == 'e' )
		file += "example2.txt";
	else if ( fileToLoad == 'I' || fileToLoad == 'i' )
		file += "input.txt";
	else
		std::cout << "No File found with that name\n";

	readFile(file, reportVector);

	//printVector(reportVector);

	for ( const auto& vector : reportVector ) {
		//if ( isSafeReport(vector) ) {
		//	safeReports++;
		//} else 
		if ( isSavable(vector) ) {
			safeReports++;
		}
	}

	std::cout << "Safe Reports: " << safeReports << "\n";

	std::cin.get();
}

bool readFile(const std::string& filePath, std::vector<std::vector<int>>& reports) {
	std::vector<int> tempVector;
	std::ifstream fileToRead(filePath);
	std::string line = "";
	size_t position = 0;

	if ( !fileToRead.is_open() ) {
		std::cout << "Failed to open " << filePath << " ...\n";
		return false;
	}

	std::cout << "Reading file " << filePath << " ...\n";
	while ( std::getline(fileToRead, line) ) {
		std::cout << "Line read: " << line << " Length(" << line.size() << "): ";

		while ( position != std::string::npos ) {
			position = line.find_first_of(' ');
			tempVector.emplace_back(std::stoi(line.substr(0, position)));
			std::cout << std::stoi(line.substr(0, position)) << " ";
			line = line.substr(position + 1, line.size());
		} // while ( !line.empty() )
		std::cout << "\n";
		reports.push_back(tempVector);
		tempVector.clear();
		position = 0;
	} // while ( std::getline(fileToRead, line) )

	fileToRead.close();
	return true;
}

void printVector(const std::vector<int>& vectorToPrint) {
	std::cout << "Vector: ";
	for ( const auto& element : vectorToPrint )
		std::cout << element << ' ';
	std::cout << "\n";
}

bool isSafeReport(const std::vector<int>& vectorToCount) {
	bool safe = true;
	int lastElement = 0;
	bool increasing = false;

	auto iter = vectorToCount.begin();
	lastElement = *iter;
	iter++;

	if ( lastElement < *iter ) {
		std::cout << "INC ";
		increasing = true;
	} else if ( lastElement > *iter ) {
		std::cout << "DEC ";
		increasing = false;
	}

	std::cout << "START ";

	for ( ; iter != vectorToCount.end(); iter++ ) {
		// It's not safe anymore when there is a change in direction.
		if ( lastElement <= *iter && increasing == false ||
			lastElement >= *iter && increasing == true ) {
			std::cout << "DChange ";
			safe = false;
			break;
		}

		// It's not safe when the increase or decrease is smaller than 1 or bigger than 3
		if ( lastElement < *iter && increasing == true || lastElement > *iter && increasing == false ) {
			if ( std::abs(lastElement - *iter) < 1 || std::abs(lastElement - *iter) > 3 ) {
				std::cout << "Toolarge ";
				safe = false;
				break;
			}
		}
		lastElement = *iter;
	} // for ( auto iter = vector.begin(); iter != vector.end(); iter++ )
	std::cout << "\n";
	
	return safe;
}

int isSavable(std::vector<int> vectorToVerify) {
	int savable = 0;
	int amountOfFixes = 0;
	std::vector<int> tempVector = vectorToVerify;
	auto iter = tempVector.begin();

	for ( size_t iterator = 0; iterator <= tempVector.size() + 1; iterator++ ) {
		printVector(tempVector);

		if ( isSafeReport(tempVector) ) {
			std::cout << "Fixable!\n";
			savable = 1;
			break;
		} else {
			tempVector = vectorToVerify;
			iter = tempVector.begin();
			if ( iter + iterator != tempVector.end() && iterator != tempVector.size() +1 ) {
				tempVector.erase(iter + iterator);
			}
		}
	}
	return savable;

	/*
	if ( iter == vectorToVerify.end() )
		return savable;
	else {
		iter++;
		tempVector.erase(tempVector.begin());
		if ( isSafeReport(tempVector) ) {
			return 0;
		} else {
			return isSavable(tempVector);
		}

		if ( lastElement < *iter ) {
			lastElement = *iter;
			iter++;

			for ( ; iter != vectorToVerify.end(); iter++ ) {
				if ( std::abs(lastElement - *iter) >= 1 && std::abs(lastElement - *iter <= 3) ) {
					if ( lastElement >= *iter ) {
						amountOfFixes++;
					}
				} else {
					amountOfFixes++;
				}
			}
		} else if ( lastElement > *iter ) {
			lastElement = *iter;
			iter++;

			for ( ; iter != vectorToVerify.end(); iter++ ) {
				if ( std::abs(lastElement - *iter) >= 1 && std::abs(lastElement - *iter <= 3) ) {
					if ( lastElement <= *iter ) {
						amountOfFixes++;
					}
				} else {
					amountOfFixes++;
				}
			}
		} else if ( lastElement == *iter ) {
			amountOfFixes++;
			lastElement = *iter;
			iter++;

			bool ascending = lastElement < *iter;

			for ( ; iter != vectorToVerify.end(); iter++ ) {
				if ( std::abs(lastElement - *iter) >= 1 && std::abs(lastElement - *iter <= 3) ) {
					if ( ascending && lastElement >= *iter ) {
						amountOfFixes++;
					} else if ( !ascending && lastElement <= *iter ) {
						amountOfFixes++;
					}
				} else {
					amountOfFixes++;
				}
			}
		}
		
		if ( amountOfFixes > 1 )
			savable++;
		else {
			savable = true;
			std::cout << "Savable\n";
		}
	}
	*/
	return savable;
}