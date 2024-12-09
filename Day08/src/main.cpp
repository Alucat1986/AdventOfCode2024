#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <iterator>
#include <ranges>
#include <string>
#include <vector>

// Technically a Node again but for readability
struct Vector2D {
	size_t x = 0;
	size_t y = 0;
};

struct Node {
	size_t PosX = 0;
	size_t PosY = 0;

	Node operator+(const Vector2D& other) const {
		return Node{ PosX + other.x, PosY + other.y };
	}
	bool operator==(const Node& other) const = default;
};

struct Radar {
	Node Location = { 0, 0 };
	char Frequency = ',';
};

bool readFile(const std::string& filePath, std::vector<std::vector<char>>& data);
bool isInsideMap(const Node& node, const std::vector<std::vector<char>>& data);
int countUniqueAntiNodeLocations(const std::vector<std::vector<char>>& data);
int countUniqueAntiNodeLocationsPartII(const std::vector<std::vector<char>>& data);
std::vector<Radar> findAllRadarDishes(const std::vector<std::vector<char>>& data);
std::pair<Vector2D, Vector2D> calculateVector(Node first, Node second);

int main() {
	char fileToLoad;
	std::string file = "input\\";
	std::vector<std::vector<char>> radarMap;
	int locationsPI = 0, locationsPII = 0;

	std::cout << "Loading (E)xample or (I)nput? ";
	fileToLoad = std::cin.get();

	if ( fileToLoad == 'E' || fileToLoad == 'e' )
		file += "example.txt";
	else if ( fileToLoad == 'I' || fileToLoad == 'i' )
		file += "input.txt";
	else
		std::cout << "No File found with that name\n";

	if ( !readFile(file, radarMap) )
		std::cout << "Couldn't open file \"" << file << "\"!\n";

	const auto start = std::chrono::system_clock::now();
	locationsPI = countUniqueAntiNodeLocations(radarMap);
	const auto end = std::chrono::system_clock::now();

	const auto startPII = std::chrono::system_clock::now();
	locationsPII = countUniqueAntiNodeLocationsPartII(radarMap);
	const auto endPII = std::chrono::system_clock::now();

	std::cout << "Unique Locations of Antinodes: " << locationsPI << " : " << locationsPII << "\n";
	std::cout << "Time of execution(P1): " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start) << "\n";
	std::cout << "Time of execution(P2): " << std::chrono::duration_cast<std::chrono::milliseconds>(endPII - startPII) << "\n";

	std::cin.get();
}

bool readFile(const std::string& filePath, std::vector<std::vector<char>>& data) {
	std::ifstream fileToRead(filePath);
	std::string line;

	if ( !fileToRead.is_open() ) {
		std::cout << "Failed to open " << filePath << " ...\n";
		return false;
	} // if ( !fileToRead.is_open() )

	std::cout << "Reading file " << filePath << " ...\n";
	while ( std::getline(fileToRead, line) ) {
		std::vector<char> lineVector;

#ifdef DEBUG
		std::cout << "Length: " << line.size() << "\t";
		std::cout << "Line: " << line;
		std::cout << "\t\t|\t";
#endif

		for ( int pos = 0; pos < line.size(); pos++ ) {
			lineVector.emplace_back(line[pos]);
		} // for ( int pos = 0; pos < line.size(); pos++ )

		data.push_back(lineVector);

#ifdef DEBUG
		std::cout << "Values:\t\t";
		for ( const auto& element : lineVector ) {
			std::cout << element << " ";
		} // for ( const auto& element : lineVector )
		std::cout << "\n";
#endif
	} // while ( std::getline(fileToRead, line) )

	fileToRead.close();
	std::cout << "Finished reading file " << filePath << " ...\n";
	return true;
}

bool isInsideMap(const Node& node, const std::vector<std::vector<char>>& data) {
	return node.PosX >= 0 && node.PosX < data[0].size() &&
		   node.PosY >= 0 && node.PosY < data.size();
}

int countUniqueAntiNodeLocations(const std::vector<std::vector<char>>& data) {
	std::vector<Node> visitedNodes;
	std::vector<Radar> radarDishes = findAllRadarDishes(data);

	for ( auto currentDishIter = radarDishes.begin(); currentDishIter != radarDishes.end(); currentDishIter++ ) {
		auto pairUpIter = currentDishIter + 1;
		while ( pairUpIter != radarDishes.end() ) {
			if ( currentDishIter->Frequency == pairUpIter->Frequency ) {
				std::pair<Vector2D, Vector2D> distances = calculateVector(Node{ currentDishIter->Location }, Node{ pairUpIter->Location });
				Node antiNodeA = currentDishIter->Location + std::get<1>(distances);
				Node antiNodeB = pairUpIter->Location + std::get<0>(distances);

				if ( isInsideMap(antiNodeA, data) && !std::ranges::contains(visitedNodes, antiNodeA) ) {
#ifdef DEBUG
					std::cout << "A(" << antiNodeA.PosX << "/" << antiNodeA.PosY << ")\n";
#endif
					visitedNodes.push_back(antiNodeA);
				} // if ( isInsideMap(antiNodeA, data) )
				if ( isInsideMap(antiNodeB, data) && !std::ranges::contains(visitedNodes, antiNodeB) ) {
#ifdef DEBUG
					std::cout << "B(" << antiNodeB.PosX << "/" << antiNodeB.PosY << ")\n";
#endif
					visitedNodes.push_back(antiNodeB);
				} // if ( isInsideMap(antiNodeB, data) )
			} // if ( currentDishIter->Frequency == pairUpIter->Frequency )
			pairUpIter++;
		} // while ( pairUpIter != radarDishes.end() )
	} // for ( auto& currentDishIter = radarDishes.begin(); currentDishIter != radarDishes.end(); currentDishIter++ )
	return static_cast<int>(visitedNodes.size());
}

// Part 2
int countUniqueAntiNodeLocationsPartII(const std::vector<std::vector<char>>& data) {
	std::vector<Node> visitedNodes;
	std::vector<Radar> radarDishes = findAllRadarDishes(data);

	for ( auto currentDishIter = radarDishes.begin(); currentDishIter != radarDishes.end(); currentDishIter++ ) {
		auto pairUpIter = currentDishIter + 1;
		while ( pairUpIter != radarDishes.end() ) {
			if ( currentDishIter->Frequency == pairUpIter->Frequency ) {
				std::pair<Vector2D, Vector2D> distances = calculateVector(Node{ currentDishIter->Location }, Node{ pairUpIter->Location });
				Node antiNodeA = currentDishIter->Location + std::get<1>(distances);
				Node antiNodeB = pairUpIter->Location + std::get<0>(distances);

				if ( !std::ranges::contains(visitedNodes, currentDishIter->Location) ) {
					visitedNodes.push_back(currentDishIter->Location);
#ifdef DEBUG
					std::cout << "C(" << currentDishIter->Location.PosX << "/" << currentDishIter->Location.PosY << ")\n";
#endif
				}
					
				if ( !std::ranges::contains(visitedNodes, pairUpIter->Location) ) {
					visitedNodes.push_back(pairUpIter->Location);
#ifdef DEBUG
					std::cout << "P(" << pairUpIter->Location.PosX << "/" << pairUpIter->Location.PosY << ")\n";
#endif
				}
					

				while ( isInsideMap(antiNodeA, data) ) {
					if ( !std::ranges::contains(visitedNodes, antiNodeA) ) {
#ifdef DEBUG
						std::cout << "A(" << antiNodeA.PosX << "/" << antiNodeA.PosY << ")\n";
#endif
						visitedNodes.push_back(antiNodeA);
					} // if ( isInsideMap(antiNodeA, data) )
					antiNodeA = antiNodeA + std::get<1>(distances);
				} // while ( isInsideMap(antiNodeA, data) )
				
				while ( isInsideMap(antiNodeB, data) ) {
					if ( !std::ranges::contains(visitedNodes, antiNodeB) ) {
#ifdef DEBUG
						std::cout << "B(" << antiNodeB.PosX << "/" << antiNodeB.PosY << ")\n";
#endif
						visitedNodes.push_back(antiNodeB);
					} // if ( isInsideMap(antiNodeB, data) )
					antiNodeB = antiNodeB + std::get<0>(distances);
				} // while ( isInsideMap(antiNodeB, data) )
			} // if ( *currentDishIter == *pairUpIter )
			pairUpIter++;
		} // while ( pairUpIter != radarDishes.end() )
	} // for ( auto& currentDishIter = radarDishes.begin(); currentDishIter != radarDishes.end(); currentDishIter++ )
	return static_cast<int>(visitedNodes.size());
}

std::vector<Radar> findAllRadarDishes(const std::vector<std::vector<char>>& data) {
	std::vector<Radar> radarDishes;

	auto notDot = [](auto columnAndElement) { return std::get<1>(columnAndElement) != '.'; };
#ifdef DEBUG
	std::cout << "\nFinding all radar dishes and their frequencies...\n";
#endif
	for ( size_t row = 0; row < data.size(); row++ ) {
		for ( const auto& [column, symbol] : std::views::enumerate(data[row]) | std::views::filter(notDot) ) {
			radarDishes.emplace_back(Radar{ Node{static_cast<size_t>(column), row}, symbol });
#ifdef DEBUG
			std::cout << "Radar(" << column << "/" << row << "|" << symbol << ")\n";
#endif
		} // for ( const auto& [column, symbol] : std::views::enumerate(data[row]) | std::views::filter(notDot) )
	} // for ( size_t row = 0; row < data.size(); row++ )
#ifdef DEBUG
	std::cout << "All found.\n";
#endif
	return radarDishes;
}

std::pair<Vector2D, Vector2D> calculateVector(Node first, Node second) {
	Vector2D forward{ second.PosX - first.PosX, second.PosY - first.PosY };
	Vector2D backward{ first.PosX - second.PosX, first.PosY - second.PosY };
	return { forward, backward };
}