#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <ranges>
#include <set>
#include <stack>
#include <string>
#include <vector>
#include <utility>

#include "debug.hpp"

using PerimeterAndPlantCount = std::pair<std::int64_t, std::int64_t>;

bool readFile(const std::string& filePath, std::vector<std::vector<char>>& data);
std::int64_t calculateSumOfRegionalFencePrices(const std::vector<std::vector<char>>& garden);
PerimeterAndPlantCount lookForRegionAndPerimeter(const std::vector<std::vector<char>>& garden, std::vector<std::vector<bool>>& visited,
												 std::size_t row, std::size_t column, char plantType);
/*
// Part 2 done by ChatGPT, I have no clue :(
std::int64_t calculateSumOfRegionalSideFencePrices(const std::vector<std::vector<char>>& garden);
PerimeterAndPlantCount calculateSidesAndPrice(const std::vector<std::vector<char>>& garden,
											  std::vector<std::vector<bool>>& visited,
											  std::size_t row, std::size_t column, char plantType);
*/
int main() {
	char fileToLoad;
	std::string file = "input\\";
	std::vector<std::vector<char>> gardenMap;
	std::int64_t partI, partII;

	std::cout << "Loading (E)xample or (I)nput? ";
	fileToLoad = static_cast<char>(std::cin.get());

	if ( fileToLoad == 'E' || fileToLoad == 'e' )
		file += "example.txt";
	else if ( fileToLoad == 'I' || fileToLoad == 'i' )
		file += "input.txt";
	else
		std::cout << "No File found with that name\n";

	if ( !readFile(file, gardenMap) )
		std::cout << "Couldn't open file \"" << file << "\"!\n";

	const auto start = std::chrono::system_clock::now();
	partI = calculateSumOfRegionalFencePrices(gardenMap);
	const auto end = std::chrono::system_clock::now();

	const auto startt = std::chrono::system_clock::now();
	//partII = calculateSumOfRegionalSideFencePrices(gardenMap);
	const auto endd = std::chrono::system_clock::now();

	std::cout << "\n===========================================================\n";
	std::cout << "PartI:\t" << partI << "\tTime of execution:\t";
	std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - start) << "\n";
	std::cout << "PartII:\t" << partII << "\tTime of execution:\t";
	std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(endd - startt) << "\n";
	std::cout << "===========================================================\n";

	std::cin.get();
}

bool readFile(const std::string& filePath, std::vector<std::vector<char>>& data) {
	std::ifstream fileToRead(filePath);
	std::string line = "";

	if ( !fileToRead.is_open() ) {
		std::cout << "Failed to open " << filePath << " ...\n";
		return false;
	}

	std::cout << "\nReading file " << filePath << " ...\n\n";
	while ( std::getline(fileToRead, line) ) {
		debugLog("Length: ", line.size(), "\t", "Line: ", line);
		data.push_back(std::vector<char>(line.begin(), line.end()));
	} // while ( std::getline(fileToRead, line) )

	fileToRead.close();

	std::cout << "\nFinished reading file " << filePath << " ...\n\n";
	return true;
}

std::int64_t calculateSumOfRegionalFencePrices(const std::vector<std::vector<char>>& garden) {
	std::int64_t regionIndex = 0, sum = 0;
	std::size_t rows = garden.size(), columns = garden[0].size();
	std::vector<std::vector<bool>> visited(rows, std::vector<bool>(columns, false));
	std::vector<PerimeterAndPlantCount> sums;

	for ( std::size_t row = 0; row < rows; row++ ) {
		for ( std::size_t column = 0; column < columns; column++ ) {
			if ( !visited[row][column] ) {
				sums.push_back(lookForRegionAndPerimeter(garden, visited, row, column, garden[row][column]));
				debugLog("Region ", regionIndex, " -> PlantCount: ", sums.back().second, " -> PerimeterSum: ", sums.back().first);
				regionIndex++;
			} // if ( !visited[row][column] )
		} // for ( int column = 0; column < columns; column++ )
	} // for ( int row = 0; row < rows; row++ )

	for ( auto& product : sums ) {
		sum += product.first * product.second;
	}

	return sum;
}

PerimeterAndPlantCount lookForRegionAndPerimeter(const std::vector<std::vector<char>>& garden, std::vector<std::vector<bool>>& visited,
												 std::size_t row, std::size_t column, char plantType) {
	std::size_t rows = garden.size(), columns = garden[0].size();
	PerimeterAndPlantCount counter{ 0, 0 };
	
	if ( /*row < 0 || column < 0 ||*/ row >= rows || column >= columns )
		return { 1, 0 };

	if ( garden[row][column] != plantType )
		return { 1, 0 };

	if ( visited[row][column] )
		return { 0, 0 };

	visited[row][column] = true;
	counter.second++;

	auto [upFirst, upSecond] = lookForRegionAndPerimeter(garden, visited, row - 1, column, plantType);
	auto [downFirst, downSecond] = lookForRegionAndPerimeter(garden, visited, row + 1, column, plantType);
	auto [leftFirst, leftSecond] = lookForRegionAndPerimeter(garden, visited, row, column - 1, plantType);
	auto [rightFirst, rightSecond] = lookForRegionAndPerimeter(garden, visited, row, column + 1, plantType);

	counter.first += upFirst + downFirst + leftFirst + rightFirst;
	counter.second += upSecond + downSecond + leftSecond + rightSecond;

	return counter;
}

/*
std::int64_t calculateSumOfRegionalSideFencePrices(const std::vector<std::vector<char>>& garden) {
	std::size_t rows = garden.size(), columns = garden[0].size();
	std::vector<std::vector<bool>> visited(rows, std::vector<bool>(columns, false));
	std::int64_t totalPrice = 0;

	for ( std::size_t row = 0; row < rows; ++row ) {
		for ( std::size_t column = 0; column < columns; ++column ) {
			if ( !visited[row][column] ) {
				char plantType = garden[row][column];
				auto [sides, price] = calculateSidesAndPrice(garden, visited, row, column, plantType);
				totalPrice += price;

				// Debug output for each region
				std::cout << "Region (" << plantType << ") -> Sides: " << sides
					<< ", Price: " << price << '\n';
			}
		}
	}

	return totalPrice;
}

PerimeterAndPlantCount calculateSidesAndPrice( const std::vector<std::vector<char>>&garden,
											  std::vector<std::vector<bool>>&visited,
											  std::size_t row, std::size_t column, char plantType) {
	std::size_t rows = garden.size(), columns = garden[0].size();
	std::set<std::pair<int, int>> boundary; // Tracks boundary cells
	std::int64_t area = 0;

	// Define movement directions (up, right, down, left)
	const std::vector<std::pair<int, int>> directions = { {-1, 0}, {0, 1}, {1, 0}, {0, -1} };

	// Use a stack for iterative flood-fill (avoiding recursion depth issues)
	std::stack<std::pair<int, int>> stack;
	stack.push({ row, column });

	while ( !stack.empty() ) {
		auto [r, c] = stack.top();
		stack.pop();

		// Skip if out of bounds or already visited
		if ( r < 0 || c < 0 || r >= rows || c >= columns || visited[r][c] || garden[r][c] != plantType ) {
			continue;
		}

		// Mark as visited and increase area count
		visited[r][c] = true;
		area++;

		// Check neighbors for boundary
		for ( const auto& [dr, dc] : directions ) {
			int nr = r + dr, nc = c + dc;
			if ( nr < 0 || nc < 0 || nr >= rows || nc >= columns || garden[nr][nc] != plantType ) {
				boundary.insert({ r, c }); // Current cell is on the boundary
			} else if ( !visited[nr][nc] ) {
				stack.push({ nr, nc }); // Add neighbor to stack
			}
		}
	}

	// Calculate the number of sides
	std::int64_t sides = 0;
	for ( const auto& [r, c] : boundary ) {
		for ( const auto& [dr, dc] : directions ) {
			int nr = r + dr, nc = c + dc;
			if ( nr < 0 || nc < 0 || nr >= rows || nc >= columns || garden[nr][nc] != plantType ) {
				sides++;
			}
		}
	}

		// Return total price and area
	return { sides, area * sides };
}
*/









/*
std::int64_t calculateSumOfRegionalFencePrices(const std::vector<std::vector<char>>& garden) {
	std::int64_t regionIndex = 0;
	std::vector<GardenPlot> regions;

	for ( const auto& [rowIndex, row] : garden | std::ranges::views::enumerate ) {
		for ( const auto& [columnIndex, column] : row | std::ranges::views::enumerate ) {
			regions.push_back(getGardenPlot(garden, regions, rowIndex, columnIndex, regionIndex));
		} // for ( const auto& [columnIndex, column] : row | std::ranges::views::enumerate )
	} // for ( const auto& [rowIndex, row] : garden | std::ranges::views::enumerate )

	// FOr Debug printing the map as a region Indexed:
	for ( const auto& [cell, plot] : regions | std::ranges::views::enumerate ) {
		if ( cell % 10 == 0 )
			std::cout << "\n";
		std::cout << plot.Region;
	}

	return 0;
}

GardenPlot getGardenPlot(const std::vector<std::vector<char>>& garden, const std::vector<GardenPlot>& regions,
						 std::size_t plotY, std::size_t plotX, std::int64_t& currentRegionIndex) {
	std::int64_t perimeter = getPerimeter(garden, plotY, plotX);
	std::int64_t regionIndex = getRegionIndex(garden, regions, plotY, plotX, currentRegionIndex);
	char plantType = garden[plotY][plotX];

	if ( regionIndex == -1 )
		regionIndex = ++currentRegionIndex;
	GardenPlot plot{ plantType, regionIndex, perimeter, GridPosition{ plotY, plotX } };

	debugLog("Gardenplot(", plot.Position.PosRow, "/", plot.Position.PosCol, "): Plant: ", plot.PlantType, " Region: ", plot.Region, " Perimeter : ", plot.Perimeter);

	return plot;
}

std::int64_t getPerimeter(const std::vector<std::vector<char>>& garden, std::size_t plotY, std::size_t plotX) {
	std::int64_t perimeter = 0;
	char plantType = garden[plotY][plotX];

	if ( !plotX ) {
		perimeter++;
		if ( plantType != garden[plotY][plotX + 1] )
			perimeter++;
	} // if ( !plotX )

	if ( !plotY ) {
		perimeter++;
		if ( plantType != garden[plotY + 1][plotX] )
			perimeter++;
	} // if ( !plotY )

	if ( plotX == garden[plotY].size() - 1 ) {
		perimeter++;
		if ( plantType != garden[plotY][plotX - 1] )
			perimeter++;
	} // if ( plotX == garden[plotY].size() - 1 ) {

	if ( plotY == garden.size() - 1 ) {
		perimeter++;
		if ( plantType != garden[plotY - 1][plotX] )
			perimeter++;
	} // if ( plotY == garden.size() - 1 )

	if ( plotX && plotY && plotX != garden[plotY].size() - 1 && plotY != garden.size() - 1 ) {
		if ( plantType != garden[plotY][plotX - 1] )
			perimeter++;
		if ( plantType != garden[plotY][plotX + 1] )
			perimeter++;
		if ( plantType != garden[plotY - 1][plotX] )
			perimeter++;
		if ( plantType != garden[plotY + 1][plotX] )
			perimeter++;
	} // if ( plotX && plotY && plotX != garden[plotY].size() - 1 && plotY != garden.size() - 1 )

	return perimeter;
}

std::int64_t getRegionIndex(const std::vector<std::vector<char>>& garden, const std::vector<GardenPlot>& plots,
							std::size_t plotY, std::size_t plotX, std::int64_t& currentRegionIndex) {
	if ( plots.empty() )
		return -1;
	else {
		for ( const auto& singlePlot : plots ) {
			if ( singlePlot.PlantType == garden[plotY][plotX] ) {
				if ( singlePlot.Position.PosRow == plotY &&
					(singlePlot.Position.PosCol == plotX + 1 || singlePlot.Position.PosCol == plotX - 1) ) {
					return singlePlot.Region;
				} else if ( singlePlot.Position.PosCol == plotX &&
						   (singlePlot.Position.PosRow == plotY + 1 || singlePlot.Position.PosRow == plotY - 1) ) {
					return singlePlot.Region;
				}
			} // if ( singlePlot.PlantType == garden[plotY][plotX] )
		} // for ( const auto& singlePlot : plots )

	} // else ... if ( plots.empty() )

	return -1;
}*/