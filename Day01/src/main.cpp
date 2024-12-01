#include <algorithm>
//#include <array>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

//bool readFile(const std::string& filePath, std::array<int, 200>& leftSide, std::array<int, 200>& rightSide);
//void printArray(const std::array<int, 200>& arrayToPrint);
bool readFile(const std::string& filePath, std::vector<int>& leftSide, std::vector<int>& rightSide);
void printVector(const std::vector<int>& vectorToPrint);

int getDistanceBetweenVectors(const std::vector<int>& leftSide, const std::vector<int>& rightSide);
int getSimilarityScore(const std::vector<int>& leftSide, const std::vector<int>& rightSide);

int main() {
	std::string file = "input\\input.txt";
	//std::array<int, 200> leftArray, rightArray;
	std::vector<int> leftVector, rightVector;
	
	//readFile(file, leftArray, rightArray);
	readFile(file, leftVector, rightVector);

	//std::ranges::sort(leftArray);
	//std::ranges::sort(rightArray);
	//printArray(leftArray);
	//printArray(rightArray);

	std::ranges::sort(leftVector);
	std::ranges::sort(rightVector);
	//printVector(leftVector);
	//printVector(rightVector);

	std::cout << "Summed Distance: ";
	std::cout << getDistanceBetweenVectors(leftVector, rightVector) << "\n";

	std::cout << "Summed Similarity: ";
	std::cout << getSimilarityScore(leftVector, rightVector) << "\n";

	std::cin.get();
}

/*
bool readFile(const std::string& filePath, std::array<int, 200>& leftSide, std::array<int, 200>& rightSide)
{
	std::ifstream fileToRead(filePath);
	std::string line = "", tmpSubString = "";
	size_t iterator = 0;
	size_t position = 0;

	if ( !fileToRead.is_open() )
	{
		std::cout << "Failed to open " << filePath << " ...\n";
		return false;
	}

	std::cout << "Reading file " << filePath << " ...\n";
	while ( std::getline(fileToRead, line) )
	{
		std::cout << "Line read: " << line << " Length(" << line.size() << "): ";

		position = line.find_first_of(' ');
		tmpSubString = line.substr(0, position);
		leftSide[iterator] = std::stoi(tmpSubString);
		std::cout << std::stoi(tmpSubString) << " / ";

		position = line.find_last_of(' ');
		tmpSubString = line.substr(position, line.size());
		rightSide[iterator] = std::stoi(tmpSubString);
		std::cout << std::stoi(tmpSubString) << "\n";
		iterator++;
	} // while ( std::getline(fileToRead, line) )

}
*/

/*
void printArray(const std::array<int, 200>& arrayToPrint)
{
	for ( std::cout << "Array: "; auto const& element : arrayToPrint )
	{
		std::cout << element << ' ';
	}
	std::cout << "\n";
}
*/

bool readFile(const std::string& filePath, std::vector<int>& leftSide, std::vector<int>& rightSide)
{
	std::ifstream fileToRead(filePath);
	std::string line = "", tmpSubString = "";
	size_t position = 0;

	if ( !fileToRead.is_open() )
	{
		std::cout << "Failed to open " << filePath << " ...\n";
		return false;
	}

	std::cout << "Reading file " << filePath << " ...\n";
	while ( std::getline(fileToRead, line) )
	{
		std::cout << "Line read: " << line << " Length(" << line.size() << "): ";

		position = line.find_first_of(' ');
		tmpSubString = line.substr(0, position);
		leftSide.emplace_back(std::stoi(tmpSubString));
		std::cout << std::stoi(tmpSubString) << " / ";

		position = line.find_last_of(' ');
		tmpSubString = line.substr(position, line.size());
		rightSide.emplace_back(std::stoi(tmpSubString));
		std::cout << std::stoi(tmpSubString) << "\n";
	} // while ( std::getline(fileToRead, line) )

}

void printVector(const std::vector<int>& vectorToPrint)
{
	for ( std::cout << "Vector: "; auto const& element : vectorToPrint )
	{
		std::cout << element << ' ';
	}
	std::cout << "\n";
}

int getDistanceBetweenVectors(const std::vector<int>& leftSide, const std::vector<int>& rightSide)
{
	size_t iterator = 0;
	int distance = 0;

	for ( int element : leftSide )
	{
		distance += std::abs(element - rightSide.at(iterator));
		iterator++;
	}
	return distance;
}

int getSimilarityScore(const std::vector<int>& leftSide, const std::vector<int>& rightSide)
{
	size_t iterator = 0;
	int similarity = 0;

	for ( int element : leftSide )
	{
		similarity += element * std::ranges::count(rightSide, element);
	} // for ( int element : leftSide )
	return similarity;
}