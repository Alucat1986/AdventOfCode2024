#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <ranges>
#include <stack>
#include <string>
#include <vector>

using Point = std::pair<std::size_t, std::size_t>;

bool readFile(const std::string& filePath, std::vector<std::vector<int>>& data);
bool isInsideMap(const Point& node, const std::vector<std::vector<int>>& data);
int sumOfTrailheadScoresPartI(const std::vector<std::vector<int>>& data);
int sumOfTrailheadScoresPartII(const std::vector<std::vector<int>>& data);
std::vector<Point> findStartingLocations(const std::vector<std::vector<int>>& data);

int main() {
	char fileToLoad;
	std::string file = "input\\";
	std::vector<std::vector<int>> hikingMap;
	int partI = 0, partII = 0;

	std::cout << "Loading (E)xample or (I)nput? ";
	fileToLoad = std::cin.get();

	if ( fileToLoad == 'E' || fileToLoad == 'e' )
		file += "example.txt";
	else if ( fileToLoad == 'I' || fileToLoad == 'i' )
		file += "input.txt";
	else
		std::cout << "No File found with that name\n";

	if ( !readFile(file, hikingMap) )
		std::cout << "Couldn't open file \"" << file << "\"!\n";

	const auto start = std::chrono::system_clock::now();
	partI = sumOfTrailheadScoresPartI(hikingMap);
	const auto end = std::chrono::system_clock::now();
	
	const auto startt = std::chrono::system_clock::now();
	partII = sumOfTrailheadScoresPartII(hikingMap);
	const auto endd = std::chrono::system_clock::now();

	std::cout << "PartI:\t" << partI << "\tTime of execution:\t";
	std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - start) << "\n";
	std::cout << "PartII:\t" << partII << "\tTime of execution:\t";
	std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(endd - startt) << "\n";

	std::cin.get();
}

bool readFile(const std::string& filePath, std::vector<std::vector<int>>& data) {
	std::ifstream fileToRead(filePath);
	std::string line;

	if ( !fileToRead.is_open() ) {
		std::cout << "Failed to open " << filePath << " ...\n";
		return false;
	}

	std::cout << "\nReading file " << filePath << " ...\n\n";
	while ( std::getline(fileToRead, line) ) {
		std::vector<int> lineVector;

#ifdef DEBUG
		std::cout << "Length: " << line.size() << "\t";
		std::cout << "Line: " << line;
		std::cout << "\t\t|\t";
#endif

		for ( int pos = 0; pos < line.size(); pos++ ) {
			lineVector.push_back(line[pos] - '0');
		} // for ( int pos = 0; pos < line.size(); pos++ )

#ifdef DEBUG
		std::cout << "Vector->" << "Values:\t";
		for ( const auto& element : lineVector ) {
			std::cout << element << " ";
		} // for ( const auto& element : lineVector )
		std::cout << "\n";
#endif
		data.push_back(lineVector);
		lineVector.clear();
	} // while ( std::getline(fileToRead, line) )

	fileToRead.close();
	std::cout << "\nFinished reading file " << filePath << " ...\n\n";
	return true;
}

bool isInsideMap(const Point& node, const std::vector<std::vector<int>>& data) {
	return node.first >= 0 && node.first < data[0].size() &&
		node.second >= 0 && node.second < data.size();
}

int sumOfTrailheadScoresPartI(const std::vector<std::vector<int>>& data) {
	std::vector<Point> trailHeads = findStartingLocations(data);
	std::vector<Point> visitedPoints;
	std::stack<Point> depthSearchStack;
	int trailHeadScores = 0;
#ifdef DEBUG
	std::cout << "\nTrailHeads Part I\n";
#endif
	for ( std::size_t pos = 0; pos < trailHeads.size(); pos++ ) {
		depthSearchStack.push(trailHeads[pos]);
#ifdef DEBUG
		std::cout << "\nPath " << pos << ":\n\n";
#endif
		while ( !depthSearchStack.empty() ) {
			Point currentPoint = depthSearchStack.top();
			std::size_t posX = currentPoint.first;
			std::size_t posY = currentPoint.second;
			depthSearchStack.pop();

			if ( std::ranges::find(visitedPoints, currentPoint) == visitedPoints.end() ) {
				visitedPoints.push_back(currentPoint);
				Point up	= { posX + 0, posY + 1 };
				Point down	= { posX + 0, posY - 1 };
				Point right	= { posX + 1, posY + 0 };
				Point left	= { posX - 1, posY + 0 };

				if ( data[posY][posX] == 9 ) {
					trailHeadScores++;
					continue;
				}
#ifdef DEBUG
				std::cout << "Visited: " << data[posY][posX] << "(" << posX << " / " << posY << ")\n";
#endif
				if ( isInsideMap(up, data) && data[up.second][up.first] == (data[posY][posX] + 1) )
					if ( std::ranges::find(visitedPoints, up) == visitedPoints.end() )
						depthSearchStack.push(up);
				
				if ( isInsideMap(down, data) && data[down.second][down.first] == (data[posY][posX] + 1) )
					if ( std::ranges::find(visitedPoints, down) == visitedPoints.end() )
						depthSearchStack.push(down);

				if ( isInsideMap(right, data) && data[right.second][right.first] == (data[posY][posX] + 1) )
					if ( std::ranges::find(visitedPoints, right) == visitedPoints.end() )
						depthSearchStack.push(right);

				if ( isInsideMap(left, data) && data[left.second][left.first] == (data[posY][posX] + 1) )
					if ( std::ranges::find(visitedPoints, left) == visitedPoints.end() )
						depthSearchStack.push(left);
			} // if ( visitedPoints.find(currentPoint) == visitedPoints.end() )
		} // while ( !depthSearchStack.empty() )
		visitedPoints.clear();
	} // for ( std::size_t pos = 0; pos < trailHeads.size(); pos++ )

	return trailHeadScores;
}

int sumOfTrailheadScoresPartII(const std::vector<std::vector<int>>& data) {
	std::vector<Point> trailHeads = findStartingLocations(data);
	std::stack<Point> depthSearchStack;
	int trailHeadScores = 0;
#ifdef DEBUG
	std::cout << "\nTrailHeads Part II\n";
#endif
	for ( std::size_t pos = 0; pos < trailHeads.size(); pos++ ) {
		depthSearchStack.push(trailHeads[pos]);
#ifdef DEBUG
		std::cout << "\nPath " << pos << ":\n\n";
#endif
		while ( !depthSearchStack.empty() ) {
			Point currentPoint = depthSearchStack.top();
			std::size_t posX = currentPoint.first;
			std::size_t posY = currentPoint.second;
			depthSearchStack.pop();

			Point up	= { posX + 0, posY + 1 };
			Point down	= { posX + 0, posY - 1 };
			Point right	= { posX + 1, posY + 0 };
			Point left	= { posX - 1, posY + 0 };
				
			if ( data[posY][posX] == 9 ) {
				trailHeadScores++;
			}
#ifdef DEBUG
			std::cout << "Visited: " << data[posY][posX] << "(" << posX << " / " << posY << ")\n";
#endif
			if ( isInsideMap(up, data) && data[up.second][up.first] == (data[posY][posX] + 1) )
				depthSearchStack.push(up);

			if ( isInsideMap(down, data) && data[down.second][down.first] == (data[posY][posX] + 1) )
				depthSearchStack.push(down);

			if ( isInsideMap(right, data) && data[right.second][right.first] == (data[posY][posX] + 1) )
				depthSearchStack.push(right);

			if ( isInsideMap(left, data) && data[left.second][left.first] == (data[posY][posX] + 1) )
				depthSearchStack.push(left);
		} // while ( !depthSearchStack.empty() )
	} // for ( std::size_t pos = 0; pos < trailHeads.size(); pos++ )

	return trailHeadScores;
}

std::vector<Point> findStartingLocations(const std::vector<std::vector<int>>& data) {
	std::vector<Point> startingLocations;
	auto startingPoint = [](auto columnAndValue) { return std::get<1>(columnAndValue) == 0; };
#ifdef DEBUG
	std::cout << "\nSearching starting locations...\n\n";
#endif
	for ( auto& row : data ) {
		for ( auto [column, value] : row | std::ranges::views::enumerate | std::ranges::views::filter(startingPoint) ) {
			startingLocations.emplace_back(std::make_pair(column, &row - &data[0]));
#ifdef DEBUG
			std::cout << "New starting location: (" << column << "/" << &row - &data[0] << ")\n";
#endif
		} // for ( auto [column, value] : row | std::ranges::views::enumerate | std::ranges::views::filter(startingPoint) )
	} // for ( const auto& row : data )
#ifdef DEBUG
	std::cout << "\nFound starting locations...\n\n";
#endif
	return startingLocations;
}