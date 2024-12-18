#include <algorithm>
#include <array>
#include <chrono>
#include <cmath>
#include <fstream>
#include <iostream>
#include <queue>
#include <ranges>
#include <unordered_map>
#include <string>

#include "debug.hpp"

static bool isExample = false;

struct FallingByte {
	std::size_t PosX = 0;
	std::size_t PosY = 0;
};

struct Node {
	std::size_t PosX = 0;
	std::size_t PosY = 0;
	std::int64_t Cost = 0; // Cost from start to this node
	std::int64_t TotalCost = 0; // cost + h
};

using MemorySpaceExample = std::array<std::array<char, 7>, 7>;
using MemorySpaceInput = std::array<std::array<char, 71>, 71>;
using FallingBytesQueue = std::queue<FallingByte>;

bool readFile(const std::string& filePath, FallingBytesQueue& data);
void makeBytesFall(MemorySpaceExample& memory, FallingBytesQueue& bytes, std::int64_t fallCounter = 12);
void makeBytesFall(MemorySpaceInput& memory, FallingBytesQueue& bytes, std::int64_t fallCounter = 1024);

void setStartAndEnd(Node& start, Node& end);
std::int64_t manhattenDistance(std::int64_t x1, std::int64_t y1, std::int64_t x2, std::int64_t y2);
auto operator<(const Node& a, const Node& b);
std::int64_t aStarPathfinding(const MemorySpaceExample& memory, Node& start, Node& end);

std::int64_t aStarPathfinding(const MemorySpaceInput& memory, Node& start, Node& end);


int main() {
	char fileToLoad;
	std::string file = "input\\";

	std::cout << "Loading (E)xample or (I)nput? ";
	fileToLoad = std::cin.get();

	if ( fileToLoad == 'E' || fileToLoad == 'e' ) {
		isExample = true;
		file += "example.txt";
	}
	else if ( fileToLoad == 'I' || fileToLoad == 'i' )
		file += "input.txt";
	else
		std::cout << "No File found with that name\n";

	std::int64_t partI = 0, partII = 0;
	FallingBytesQueue bytesToFall;
	if ( !readFile(file, bytesToFall) )
		std::cout << "Couldn't open file \"" << file << "\"!\n";

	Node startNode;
	Node endNode;
	setStartAndEnd(startNode, endNode);
	FallingByte lastByte;

	const auto start = std::chrono::system_clock::now();
	const auto startt = std::chrono::system_clock::now();
	const auto endd = std::chrono::system_clock::now();
	if ( isExample ) {
		MemorySpaceExample memorySpace;
		std::ranges::for_each(memorySpace, [](auto& row) { std::ranges::fill(row, '.'); });
		makeBytesFall(memorySpace, bytesToFall);
		partI = aStarPathfinding(memorySpace, startNode, endNode);
		partII = aStarPathfinding(memorySpace, startNode, endNode);
		while ( partII && !bytesToFall.empty() ) {
			lastByte = bytesToFall.front();
			makeBytesFall(memorySpace, bytesToFall, 1);
			partII = aStarPathfinding(memorySpace, startNode, endNode);
		} // while ( partII )
	} // if ( isExample )
	else if ( !isExample ) {
		MemorySpaceInput memorySpace;
		std::ranges::for_each(memorySpace, [](auto& row) { std::ranges::fill(row, '.'); });
		makeBytesFall(memorySpace, bytesToFall);
		partI = aStarPathfinding(memorySpace, startNode, endNode);
		partII = aStarPathfinding(memorySpace, startNode, endNode);
		while ( partII && !bytesToFall.empty() ) {
			lastByte = bytesToFall.front();
			makeBytesFall(memorySpace, bytesToFall, 1);
			partII = aStarPathfinding(memorySpace, startNode, endNode);
		} // while ( partII )
	} // else if ( !isExample )
	const auto end = std::chrono::system_clock::now();

	std::cout << "\n===========================================================\n";
	std::cout << "PartI:\t" << partI << "\tTime of execution:\t";
	std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - start) << "\n";
	std::cout << "PartII:\t" << lastByte.PosX << ", " << lastByte.PosY << "\tTime of execution:\t";
	std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(endd - startt) << "\n";
	std::cout << "===========================================================\n";

	std::cin.get();
}

bool readFile(const std::string& filePath, FallingBytesQueue& data) {
	std::ifstream fileToRead(filePath);
	std::string line = "";

	if ( !fileToRead.is_open() ) {
		std::cout << "Failed to open " << filePath << " ...\n";
		return false;
	}

	std::cout << "\nReading file " << filePath << " ...\n\n";
	while ( std::getline(fileToRead, line) ) {
		debugLog("Length: ", line.size(), "\t", "Line: ", line);

		FallingByte byte;
		byte.PosX = static_cast<std::size_t>(std::stoi(line.substr(0, line.find(','))));
		byte.PosY = static_cast<std::size_t>(std::stoi(line.substr(line.find(',') + 1)));
		data.push(byte);
	} // while ( std::getline(fileToRead, line) )

	fileToRead.close();

	std::cout << "\nFinished reading file " << filePath << " ...\n\n";
	return true;
}

void makeBytesFall(MemorySpaceExample& memory, FallingBytesQueue& bytes, std::int64_t fallCounter) {
	for ( std::size_t counter = 0; counter < fallCounter; counter++ ) {
		FallingByte byte = bytes.front();
		bytes.pop();
		memory[byte.PosY][byte.PosX] = '#';
		debugLog("Byte ", byte.PosX, ",", byte.PosY, " fell!");
	} // for ( std::size_t counter = 0; counter < 12; counter++ )
}

void makeBytesFall(MemorySpaceInput& memory, FallingBytesQueue& bytes, std::int64_t fallCounter) {
	for ( std::size_t counter = 0; counter < fallCounter; counter++ ) {
		FallingByte byte = bytes.front();
		bytes.pop();
		memory[byte.PosY][byte.PosX] = '#';
		debugLog("Byte ", byte.PosX, ",", byte.PosY, " fell!");
	} // for ( std::size_t counter = 0; counter < 1024; counter++ )
}

void setStartAndEnd(Node& start, Node& end) {
	if ( isExample ) {
		end.PosX = 6;
		end.PosY = 6;
	}
	else if ( !isExample ) {
		end.PosX = 70;
		end.PosY = 70;
	}
}

std::int64_t manhattenDistance(std::int64_t x1, std::int64_t y1, std::int64_t x2, std::int64_t y2) {
	return std::abs(x1 - x2) + std::abs(y1 - y2);
}

auto operator<(const Node& a, const Node& b) {
	return a.TotalCost > b.TotalCost;
}

std::int64_t aStarPathfinding(const MemorySpaceExample& memory, Node& start, Node& end) {
	std::size_t rows = memory.size();
	std::size_t columns = memory[0].size();

	auto isValid = [&](std::int64_t x, std::int64_t y) -> bool {
		return x >= 0 && y >= 0 && x < rows && y < columns && memory[x][y] != '#';
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

std::int64_t aStarPathfinding(const MemorySpaceInput& memory, Node& start, Node& end) {
	std::size_t rows = memory.size();
	std::size_t columns = memory[0].size();

	auto isValid = [&](std::int64_t x, std::int64_t y) -> bool {
		return x >= 0 && y >= 0 && x < rows && y < columns && memory[x][y] != '#';
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