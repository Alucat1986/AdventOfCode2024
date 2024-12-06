#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

struct Guard {
	size_t PosX = 0;
	size_t PosY = 0;
	int Direction = 0; // 0 = up, 1 = right, 2 = down, 3 = left

	void changeDirection();
	void move();
	bool operator==(const Guard& other) const;
};

bool readFile(const std::string& filePath, std::vector<std::vector<char>>& data);
void sendGuardOnPatrol(std::vector<std::vector<char>>& mapToTraverse);
Guard findGuard(const std::vector<std::vector<char>>& mapToTraverse);
int countVisitedPositions(const std::vector<std::vector<char>>& mapToTraverse);
int countPossibleLoopLocationsBruteForce(std::vector<std::vector<char>> mapToTraverse);
bool checkPatrolLoops(std::vector<std::vector<char>>& mapToTraverse);
void printLaboratory(const std::vector<std::vector<char>>& mapToTraverse);

int main() {
	char fileToLoad;
	std::string file = "input\\";
	std::vector<std::vector<char>> mapLaboratory;
	std::vector<std::vector<char>> backup;

	std::cout << "Loading (E)xample or (I)nput? ";
	fileToLoad = std::cin.get();

	if ( fileToLoad == 'E' || fileToLoad == 'e' )
		file += "example.txt";
	else if ( fileToLoad == 'I' || fileToLoad == 'i' )
		file += "input.txt";
	else
		std::cout << "No File found with that name\n";

	if ( !readFile(file, mapLaboratory) )
		std::cout << "Couldn't open file \"" << file << "\"!\n";

	std::ranges::copy(mapLaboratory.begin(), mapLaboratory.end(), std::back_inserter(backup));
	sendGuardOnPatrol(mapLaboratory);
	printLaboratory(mapLaboratory);
	std::cout << "Visited Positions: " << countVisitedPositions(mapLaboratory) << "\n";
	mapLaboratory.clear();
	std::ranges::copy(backup.begin(), backup.end(), std::back_inserter(mapLaboratory));
	std::cout << "Loop Positions: " << countPossibleLoopLocationsBruteForce(mapLaboratory) << "\n";

	std::cin.get();
}

void Guard::changeDirection() {
	if ( Direction == 3 )
		Direction = 0;
	else
		Direction++;
}

void Guard::move() {
	switch ( Direction ) {
	case 0:
		PosY--;
		break;
	case 1:
		PosX++;
		break;
	case 2:
		PosY++;
		break;
	case 3:
		PosX--;
		break;
	default:
		std::cout << "I should not be here...\n";
		break;
	}
}

bool Guard::operator==(const Guard& other) const {
	return PosX == other.PosX &&
		   PosY == other.PosY &&
		   Direction == other.Direction;
}

bool readFile(const std::string& filePath, std::vector<std::vector<char>>& data) {
	std::ifstream fileToRead(filePath);
	std::vector<char> lineVector;
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

		for ( size_t pos = 0; pos < line.size(); pos++ ) {
			lineVector.emplace_back(line.at(pos));
		}

		data.emplace_back(lineVector);
		lineVector.clear();
	} // while ( std::getline(fileToRead, line) )

	fileToRead.close();
	return true;
}

void sendGuardOnPatrol(std::vector<std::vector<char>>& mapToTraverse) {
	Guard guard = findGuard(mapToTraverse);

	// Go forward if no obstacle
	// Otherwise turn right 90°
	while ( guard.PosX >= 0 && guard.PosY >= 0 &&
		   guard.PosX < mapToTraverse.at(0).size() &&
		   guard.PosY < mapToTraverse.size() ) {
		switch ( guard.Direction ) {
		case 0:
			if ( guard.PosY > 0 && mapToTraverse.at(guard.PosY - 1).at(guard.PosX) == '#' ) {
				guard.changeDirection();
			} else {
				mapToTraverse.at(guard.PosY).at(guard.PosX) = 'X';
				guard.move();
			}
			break;
		case 1:
			if ( guard.PosX < (mapToTraverse.at(guard.PosY).size() - 1) &&
				mapToTraverse.at(guard.PosY).at(guard.PosX + 1) == '#' ) {
				guard.changeDirection();
			} else {
				mapToTraverse.at(guard.PosY).at(guard.PosX) = 'X';
				guard.move();
			}
			break;
		case 2:
			if ( guard.PosY < (mapToTraverse.size() - 1) &&
				mapToTraverse.at(guard.PosY + 1).at(guard.PosX) == '#' ) {
				guard.changeDirection();
			} else {
				mapToTraverse.at(guard.PosY).at(guard.PosX) = 'X';
				guard.move();
			}
			break;
		case 3:
			if ( guard.PosX > 0 && mapToTraverse.at(guard.PosY).at(guard.PosX - 1) == '#' ) {
				guard.changeDirection();
			} else {
				mapToTraverse.at(guard.PosY).at(guard.PosX) = 'X';
				guard.move();
			}
			break;
		default:
			std::cout << "Uhh...\n";
			break;
		}
	}
}

Guard findGuard(const std::vector<std::vector<char>>& mapToTraverse) {
	for ( size_t y = 0; y < mapToTraverse.size(); y++ ) {
		if ( std::ranges::contains(mapToTraverse.at(y), '^') ) {
			for ( size_t x = 0; x < mapToTraverse.at(y).size(); x++ ) {
				if ( mapToTraverse.at(y).at(x) == '^' ) {
					//std::cout << "Guard found at (" << x << "/" << y << ").\n";
					return { x, y, 0 };
				}
			}
		}
	}
	std::cout << "No Guard found!\n";
	return { 0, 0, 0 };
}

int countVisitedPositions(const std::vector<std::vector<char>>& mapToTraverse) {
	int result = 0;
	for ( size_t y = 0; y < mapToTraverse.size(); y++ ) {
		result += std::ranges::count(mapToTraverse.at(y), 'X');
	}
	return result;
}

int countPossibleLoopLocationsBruteForce(std::vector<std::vector<char>> mapToTraverse) {
	int locations = 0;
	std::vector<std::vector<char>> backup;
	std::ranges::copy(mapToTraverse.begin(), mapToTraverse.end(), std::back_inserter(backup));

	for ( size_t y = 0; y < mapToTraverse.size(); y++ ) {
		std::cout << "\nRow " << y << "\n";
		for ( size_t x = 0; x < mapToTraverse.at(y).size(); x++ ) {
			if ( mapToTraverse.at(y).at(x) == '.' ) {
				mapToTraverse.at(y).at(x) = 'O';
			} else {
				continue;
			}
			
			if ( checkPatrolLoops(mapToTraverse) ) {
				locations++;
			}
			mapToTraverse.clear();
			std::ranges::copy(backup.begin(), backup.end(), std::back_inserter(mapToTraverse));
		}
	}
	return locations;
}

// Modified for part 2
bool checkPatrolLoops(std::vector<std::vector<char>>& mapToTraverse) {
	std::vector<Guard> visitedPositions;
	Guard guard = findGuard(mapToTraverse);

	// Go forward if no obstacle
	// Otherwise turn right 90°
	while ( guard.PosX >= 0 && guard.PosY >= 0 &&
		   guard.PosX < mapToTraverse.at(0).size() &&
		   guard.PosY < mapToTraverse.size() ) {
		if ( std::ranges::contains(visitedPositions, guard) ) {
			return true;
		} else
			visitedPositions.push_back(guard);

		switch ( guard.Direction ) {
		case 0:
			if ( guard.PosY > 0 && (mapToTraverse.at(guard.PosY - 1).at(guard.PosX) == '#' ||
									mapToTraverse.at(guard.PosY - 1).at(guard.PosX) == 'O') ) {
				guard.changeDirection();
			} else {
				guard.move();
			}
			break;
		case 1:
			if ( guard.PosX < (mapToTraverse.at(guard.PosY).size() - 1) &&
				(mapToTraverse.at(guard.PosY).at(guard.PosX + 1) == '#' ||
				 mapToTraverse.at(guard.PosY).at(guard.PosX + 1) == 'O') ) {
				guard.changeDirection();
			} else {
				guard.move();
			}
			break;
		case 2:
			if ( guard.PosY < (mapToTraverse.size() - 1) &&
				(mapToTraverse.at(guard.PosY + 1).at(guard.PosX) == '#' ||
				 mapToTraverse.at(guard.PosY + 1).at(guard.PosX) == 'O') ) {
				guard.changeDirection();
			} else {
				guard.move();
			}
			break;
		case 3:
			if ( guard.PosX > 0 && (mapToTraverse.at(guard.PosY).at(guard.PosX - 1) == '#' || mapToTraverse.at(guard.PosY).at(guard.PosX - 1) == 'O') ) {
				guard.changeDirection();
			} else {
				guard.move();
			}
			break;
		default:
			std::cout << "Uhh...\n";
			break;
		}
	}
	return false;
}

void printLaboratory(const std::vector<std::vector<char>>& mapToTraverse) {
	std::cout << "Laboratory:\n";
	for ( size_t y = 0; y < mapToTraverse.size(); y++ ) {
		for ( size_t x = 0; x < mapToTraverse.at(y).size(); x++ ) {
			std::cout << mapToTraverse.at(y).at(x);
		}
		std::cout << "\n";
	}
	std::cout << "\n";
}