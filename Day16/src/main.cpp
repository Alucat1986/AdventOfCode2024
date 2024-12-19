#include <algorithm>
#include <chrono>
#include <cmath>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <queue>
#include <ranges>
#include <stack>
#include <string>
#include <string_view>
#include <vector>

#include "debug.hpp"

using MazeMap = std::vector<std::string>;

struct Node {
	std::int64_t PosX = 0;
	std::int64_t PosY = 0;
	std::int64_t Cost = 0;
	std::int64_t TotalCost = 0;
};


bool readFile(const std::string& filePath, MazeMap& maze);
void findStartAndEnd(Node& start, Node& end, const MazeMap& maze);
void printMap(const MazeMap& maze);

std::int64_t manhattenDistance(std::int64_t x1, std::int64_t y1, std::int64_t x2, std::int64_t y2);
auto operator<(const Node& a, const Node& b);
std::int64_t aStarPathfinding(const MazeMap& maze, Node& start, Node& end);

int main() {
	char fileToLoad;
	std::string file = "input\\";
	std::int64_t partI = 0, partII = 0;

	MazeMap maze;

	std::cout << "Loading (S)mallExample, (E)xample or (I)nput? ";
	fileToLoad = std::cin.get();

	if ( fileToLoad == 'E' || fileToLoad == 'e' )
		file += "example.txt";
	else if ( fileToLoad == 'I' || fileToLoad == 'i' )
		file += "input.txt";
	else if ( fileToLoad == 'S' || fileToLoad == 's' )
		file += "smallexample.txt";
	else
		std::cout << "No File found with that name\n";

	if ( !readFile(file, maze) )
		std::cout << "Couldn't open file \"" << file << "\"!\n";
	
	printMap(maze);

	Node startNode, endNode;
	findStartAndEnd(startNode, endNode, maze);

	const auto start = std::chrono::system_clock::now();
	partI = aStarPathfinding(maze, startNode, endNode);
	const auto end = std::chrono::system_clock::now();

	const auto startt = std::chrono::system_clock::now();
	//partII = sumAllGPSLocations(warehousePartII);
	const auto endd = std::chrono::system_clock::now();

	std::cout << "\n===========================================================\n";
	std::cout << "PartI:\t" << partI << "\tTime of execution:\t";
	std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - start) << "\n";
	std::cout << "PartII:\t" << partII << "\tTime of execution:\t";
	std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(endd - startt) << "\n";
	std::cout << "===========================================================\n";

	std::cin.get();
}

bool readFile(const std::string& filePath, MazeMap& maze) {
	std::ifstream fileToRead(filePath);
	std::string line = "";

	if ( !fileToRead.is_open() ) {
		std::cout << "Failed to open " << filePath << " ...\n";
		return false;
	}
	std::cout << "\nReading file " << filePath << " ...\n\n";

	while ( std::getline(fileToRead, line) ) {
		debugLog("Length: ", line.size(), "\t", "Line: ", line);

		maze.push_back(line);
	} // while ( std::getline(fileToRead, line) )

	fileToRead.close();
	std::cout << "\nFinished reading file " << filePath << " ...\n\n";

	return true;
}

void findStartAndEnd(Node& start, Node& end, const MazeMap& maze) {
	for ( const auto& [rIndex, row] : maze | std::ranges::views::enumerate ) {
		if ( maze[rIndex].find("E") != std::string::npos) {
			end.PosX = static_cast<std::int64_t>(maze[rIndex].find("E"));
			end.PosY = rIndex;
		} // if ( pos != std::string::npos )
		else if ( maze[rIndex].find("S") != std::string::npos ) {
			start.PosX = static_cast<std::int64_t>(maze[rIndex].find("S"));
			start.PosY = rIndex;
		} // else if ( maze[rIndex].find("S") != std::string::npos )
	}
}

void printMap(const MazeMap& maze) {
	std::cout << "\n";
	for ( const auto& [rIndex, row] : maze | std::ranges::views::enumerate ) {
		std::cout << rIndex << ":\t" << row << "\n";
	} // for ( const auto& row : warehouse )
	std::cout << "\n";
}

std::int64_t manhattenDistance(std::int64_t x1, std::int64_t y1, std::int64_t x2, std::int64_t y2) {
	return std::abs(x1 - x2) + std::abs(y1 - y2);
}

auto operator<(const Node& a, const Node& b) {
	return a.TotalCost > b.TotalCost;
}

std::int64_t aStarPathfinding(const MazeMap& maze, Node& start, Node& end) {
	std::size_t rows = maze.size();
	std::size_t columns = maze[0].size();

	auto isValid = [&](std::int64_t x, std::int64_t y) -> bool {
		return x >= 0 && y >= 0 && x < rows && y < columns && maze[x][y] != '#';
		};

	std::priority_queue<Node> openSet;
	std::unordered_map<std::int64_t, Node> cameFrom;
	std::unordered_map<std::int64_t, std::int64_t> costScore;

	auto index = [&](std::int64_t x, std::int64_t y) { return x * columns + y; };

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
			std::ranges::reverse(path);

			for ( auto& element : path ) {
				std::cout << "(" << element.PosX << "," << element.PosY << ") ->";
			}
			return static_cast<std::int64_t>(path.size() - 1);
		} // if ( current.PosX == end.PosX && current.PosY == end.PosY )

		for ( const auto& [dx, dy] : { std::pair{-1, 0}, std::pair{1, 0}, std::pair{0, -1}, std::pair{0, 1} } ) {
			std::int64_t nx = current.PosX + dx, ny = current.PosY + dy;

			if ( !isValid(nx, ny) ) continue;

			std::int64_t tentativeCost = current.Cost + 1;
			std::int64_t neighbourIndex = index(nx, ny);

			if ( !costScore.contains(neighbourIndex) || tentativeCost < costScore[neighbourIndex] ) {
				costScore[neighbourIndex] = tentativeCost;

				Node neighbour = { nx, ny, tentativeCost, tentativeCost + manhattenDistance(nx, ny, end.PosX, end.PosY) };
				cameFrom[neighbourIndex] = current;
				openSet.push(neighbour);
			} // if ( !costScore.contains(neighbourIndex) || tentativeCost < costScore[neighbourIndex] )
		} // for ( const auto& [dx, dy] : { std::pair{-1, 0}, std::pair{1, 0}, std::pair{0, -1}, std::pair{0, 1} } )
	} // while ( !openSet.empty() )

	return 0;
}