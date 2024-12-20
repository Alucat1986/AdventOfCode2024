#include <algorithm>
#include <chrono>
#include <cmath>
#include <fstream>
#include <iostream>
#include <queue>
#include <ranges>
#include <unordered_map>
#include <string>

#include "debug.hpp"

struct MapSize {
	std::size_t rows = 0;
	std::size_t columns = 0;
};

static MapSize mapSize;

struct Wall {
	std::size_t PosX = 0;
	std::size_t PosY = 0;

	bool operator==(const Wall& other) const = default;
	void print() { std::cout << '#'; }
};

struct Node {
	std::size_t PosX = 0;
	std::size_t PosY = 0;
	std::int64_t Cost = 1; // Cost from start to this node
	std::int64_t TotalCost = 0; // cost + h
};

bool readFile(const std::string& filePath, std::vector<Wall>& data, Node& start, Node& end);
void printMap(const std::vector<Wall>& toPrint, const Node& start, const Node& end);

std::int64_t manhattenDistance(std::int64_t x1, std::int64_t y1, std::int64_t x2, std::int64_t y2);
auto operator<(const Node& a, const Node& b);
std::int64_t aStarPathfinding(const std::vector<Wall>& walls, Node& start, Node& end);

int main() {
	char fileToLoad;
	std::string file = "input\\";

	std::cout << "Loading (E)xample or (I)nput? ";
	fileToLoad = std::cin.get();

	if ( fileToLoad == 'E' || fileToLoad == 'e' ) {
		file += "example.txt";
	}
	else if ( fileToLoad == 'I' || fileToLoad == 'i' )
		file += "input.txt";
	else
		std::cout << "No File found with that name\n";

	std::vector<Wall> raceTrackWalls;
	Node startNode;
	Node endNode;
	if ( !readFile(file, raceTrackWalls, startNode, endNode) )
		std::cout << "Couldn't open file \"" << file << "\"!\n";

	std::cout << "Start: (" << startNode.PosX << "," << startNode.PosY << ")\n";
	std::cout << "End: (" << endNode.PosX << "," << endNode.PosY << ")\n";
	std::cout << "MapSize = " << mapSize.columns << " * " << mapSize.rows << "\n";
	printMap(raceTrackWalls, startNode, endNode);

	const auto start = std::chrono::system_clock::now();
	std::int64_t partI = aStarPathfinding(raceTrackWalls, startNode, endNode);
	const auto end = std::chrono::system_clock::now();
	const auto startt = std::chrono::system_clock::now();
	std::int64_t partII = 0;// aStarPathfinding(memorySpace, startNode, endNode);
	const auto endd = std::chrono::system_clock::now();

	std::cout << "\n===========================================================\n";
	std::cout << "PartI:\t" << partI << "\tTime of execution:\t";
	std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - start) << "\n";
	std::cout << "PartII:\t" << partII << "\tTime of execution:\t";
	std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(endd - startt) << "\n";
	std::cout << "===========================================================\n";

	std::cin.get();
}

bool readFile(const std::string& filePath, std::vector<Wall>& data, Node& start, Node& end) {
	std::ifstream fileToRead(filePath);
	std::string line = "";

	if ( !fileToRead.is_open() ) {
		std::cout << "Failed to open " << filePath << " ...\n";
		return false;
	}

	auto findWall = [](auto indexAndSym) { return std::get<1>(indexAndSym) == '#'; };
	std::size_t rowCounter = 0;

	std::cout << "\nReading file " << filePath << " ...\n\n";
	while ( std::getline(fileToRead, line) ) {
		debugLog("Length: ", line.size(), "\t", "Line: ", line);

		if ( line.contains('E') ) {
			std::size_t rIndex = line.find('E');
			end.PosX = rIndex;
			end.PosY = rowCounter;
		} // if ( line.contains('E') )

		if ( line.contains('S') ) {
			std::size_t rIndex = line.find('S');
			start.PosX = rIndex;
			start.PosY = rowCounter;
		} // if ( line.contains('S') )

		for ( auto [rIndex, value] : line | std::ranges::views::enumerate | std::ranges::views::filter(findWall) ) {
			Wall wall{ static_cast<std::size_t>(rIndex), rowCounter };
			data.push_back(wall);
		} // for ( auto& [rIndex, value] : line | std::ranges::views::enumerate | std::ranges::views::filter(findWall) )
		rowCounter++;
	} // while ( std::getline(fileToRead, line) )
	mapSize.rows = --rowCounter;
	mapSize.columns = data.back().PosX;
	fileToRead.close();

	std::cout << "\nFinished reading file " << filePath << " ...\n\n";
	return true;
}

void printMap(const std::vector<Wall>& toPrint, const Node& start, const Node& end) {
	std::cout << "RaceTrack:\n";
	for ( std::size_t row = 0; row <= mapSize.rows; row++ ) {
		std::cout << "\n";
		for ( std::size_t column = 0; column <= mapSize.columns; column++ ) {
			if ( std::ranges::contains(toPrint, Wall{ column, row }) ) {
				std::cout << '#';
			}
			else if ( start.PosX == column && start.PosY == row ) {
				std::cout << 'S';
			}
			else if ( end.PosX == column && end.PosY == row ) {
				std::cout << 'E';
			}
			else {
				std::cout << '.';
			}
		}
	}
	std::cout << "\n\n";
}

std::int64_t manhattenDistance(std::int64_t x1, std::int64_t y1, std::int64_t x2, std::int64_t y2) {
	return std::abs(x1 - x2) + std::abs(y1 - y2);
}

auto operator<(const Node& a, const Node& b) {
	return a.TotalCost > b.TotalCost;
}

std::int64_t aStarPathfinding(const std::vector<Wall>& walls, Node& start, Node& end) {
	auto isValid = [&](std::size_t x, std::size_t y) -> bool {
		return x >= 0 && y >= 0 && x <= mapSize.columns && y <= mapSize.rows &&
			std::ranges::contains(walls, Wall{ x, y });
		};

	std::priority_queue<Node> openSet;
	std::unordered_map<std::int64_t, Node> cameFrom;
	std::unordered_map<std::int64_t, std::int64_t> costScore;

	auto index = [&](std::int64_t x, std::int64_t y) { return x * mapSize.columns + y; };

	costScore[index(start.PosX, start.PosY)] = 0;
	start.TotalCost = manhattenDistance(start.PosX, start.PosY, end.PosX, end.PosY);
	openSet.push(start);

	while ( !openSet.empty() ) {
		Node current = openSet.top();
		openSet.pop();

		if ( current.PosX == end.PosX && current.PosY == end.PosY ) {
			std::vector<Node> path;
			while ( cameFrom.contains(index(current.PosX, current.PosY)) ) {
				path.push_back(current);
				current = cameFrom[index(current.PosX, current.PosY)];
			} // while ( cameFrom.contains(index(current.PosX, current.PosY)) )

			path.push_back(start);
			return static_cast<std::int64_t>(path.size() - 1);
		} // if ( current.PosX == end.PosX && current.PosY == end.PosY )

		for ( const auto& [dx, dy] : { std::pair{-1, 0}, std::pair{1, 0}, std::pair{0, -1}, std::pair{0, 1} } ) {
			std::int64_t nx = current.PosX + dx, ny = current.PosY + dy;

			if ( !isValid(nx, ny) ) continue;

			std::int64_t tentativeCost = current.Cost + 1;
			std::int64_t neighbourIndex = index(nx, ny);

			if ( !costScore.contains(neighbourIndex) || tentativeCost < costScore[neighbourIndex] ) {
				costScore[neighbourIndex] = tentativeCost;

				Node neighbour = { static_cast<std::size_t>(nx), static_cast<std::size_t>(ny),
					tentativeCost, tentativeCost + manhattenDistance(nx, ny, end.PosX, end.PosY) };
				cameFrom[neighbourIndex] = current;
				openSet.push(neighbour);
			} // if ( !costScore.contains(neighbourIndex) || tentativeCost < costScore[neighbourIndex] )
		} // for ( const auto& [dx, dy] : { std::pair{-1, 0}, std::pair{1, 0}, std::pair{0, -1}, std::pair{0, 1} } )
	} // while ( !openSet.empty() )

	return 0;
}