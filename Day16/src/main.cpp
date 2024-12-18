#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <queue>
#include <ranges>
#include <stack>
#include <string>
#include <string_view>
#include <vector>

#include "debug.hpp"

using MazeMap = std::vector<std::string>;

enum class Direction {
	Up,
	Right,
	Down,
	Left
};

const std::vector<std::pair<std::int64_t, std::int64_t>> directions = {
	{ 0, -1 },
	{+1,  0 },
	{ 0, +1 },
	{-1,  0 }
};

struct Node {
	std::int64_t PosX = 0;
	std::int64_t PosY = 0;
	std::int64_t Costs = 0;

	bool operator==(const Node& other) const {
		return PosX == other.PosX && PosY == other.PosY;
	}
};


bool readFile(const std::string& filePath, MazeMap& maze);
void findStartAndEnd(Node& start, Node& end, const MazeMap& maze);
void printMap(const MazeMap& maze);

std::int64_t findShortestPath(const MazeMap& maze);
bool isValid(const Node& node, const MazeMap& maze, const std::vector<std::vector<bool>>& visited);

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

	const auto start = std::chrono::system_clock::now();
	partI = findShortestPath(maze);
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

std::int64_t findShortestPath(const MazeMap& maze) {
	Node start, end;
	findStartAndEnd(start, end, maze);
	std::int64_t result = 0;

	std::vector<std::vector<bool>> visitedNodes(maze.size(), std::vector<bool>(maze[0].size(), false));
	std::queue<Node> path;
	Direction currentDir = Direction::Right;

	visitedNodes[start.PosX][start.PosY] = true;
	path.push(start);

	while ( !path.empty() ) {
		Node node = path.front();
		path.pop();

		for ( std::size_t i = 0; i < 4; i++ ) {
			Node newNode = { node.PosX, node.PosY, node.Costs };

			switch ( i ) {
				case 0:
					switch ( currentDir ) {
						case Direction::Up:
							newNode.Costs++;
							break;
						case Direction::Right:
							currentDir = Direction::Up;
							newNode.Costs += 1000;
							newNode.Costs++;
							break;
						case Direction::Down:
							currentDir = Direction::Up;
							newNode.Costs += 2000;
							newNode.Costs++;
							break;
						case Direction::Left:
							currentDir = Direction::Up;
							newNode.Costs += 1000;
							newNode.Costs++;
							break;
						default:
							std::cout << "Oh no UP :(\n";
							break;
					} // switch ( currentDir )
					newNode.PosY--;
					break;
				case 1:
					switch ( currentDir ) {
						case Direction::Up: {
							currentDir = Direction::Right;
							newNode.Costs += 1000;
							newNode.Costs++;
							break;
						}
						case Direction::Right: {
							newNode.Costs++;
							break;
						}
						case Direction::Down: {
							currentDir = Direction::Right;
							newNode.Costs += 1000;
							newNode.Costs++;
							break;
						}
						case Direction::Left: {
							currentDir = Direction::Right;
							newNode.Costs += 2000;
							newNode.Costs++;
							break;
						}
						default:
							std::cout << "Oh no RIGHT :(\n";
							break;
					} // switch ( currentDir )
					newNode.PosX++;
					break;
				case 2:
					switch ( currentDir ) {
						case Direction::Up: {
							currentDir = Direction::Down;
							newNode.Costs += 2000;
							newNode.Costs++;
							break;
						}
						case Direction::Right: {
							currentDir = Direction::Down;
							newNode.Costs += 1000;
							newNode.Costs++;
							break;
						}
						case Direction::Down: {
							newNode.Costs++;
							break;
						}
						case Direction::Left: {
							currentDir = Direction::Down;
							newNode.Costs += 1000;
							newNode.Costs++;
							break;
						}
						default:
							std::cout << "Oh no DOWN :(\n";
							break;
					} // switch ( currentDir )
					newNode.PosY++;
					break;
				case 3:
					switch ( currentDir ) {
						case Direction::Up: {
							currentDir = Direction::Left;
							newNode.Costs += 1000;
							newNode.Costs++;
							break;
						}
						case Direction::Right: {
							currentDir = Direction::Left;
							newNode.Costs += 2000;
							newNode.Costs++;
							break;
						}
						case Direction::Down: {
							currentDir = Direction::Left;
							newNode.Costs += 1000;
							newNode.Costs++;
							break;
						}
						case Direction::Left: {
							newNode.Costs++;
							break;
						}
						default:
							std::cout << "Oh no :(\n";
							break;
					} // switch ( currentDir )
					newNode.PosX--;
					break;
				default:
					std::cout << "noooo nooo...\n";
					break;
			} // switch ( i )

			if ( isValid(newNode, maze, visitedNodes) ) {
				visitedNodes[newNode.PosY][newNode.PosX] = true;

				path.push(newNode);
			} // if ( isValid(newNode, maze, visitedNodes) ) {

			if ( node == end ) {
				return node.Costs;
			} // if ( node.PosX == end.PosX && node.PosY == end.PosY )
		} // for ( std::size_t i = 0; i < 4; i++ )
	} // while ( !path.empty() )

	return -1;
}

bool isValid(const Node& node, const MazeMap& maze, const std::vector<std::vector<bool>>& visited) {
	return maze[node.PosY][node.PosX] != '#' && !visited[node.PosY][node.PosX];
}