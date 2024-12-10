#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <iterator>
#include <ranges>
#include <string>
#include <vector>

bool readFile(const std::string& filePath, std::string& data);
int calculateChecksumPartI(const std::string& data);
std::string constructFilesystem(const std::string& data);

int main() {
	char fileToLoad;
	std::string file = "input\\";
	std::string filesystem = "";
	int checksumPartI = 0, checksumPartII = 0;

	std::cout << "Loading (E)xample or (I)nput? ";
	fileToLoad = std::cin.get();

	if ( fileToLoad == 'E' || fileToLoad == 'e' )
		file += "example.txt";
	else if ( fileToLoad == 'I' || fileToLoad == 'i' )
		file += "input.txt";
	else
		std::cout << "No File found with that name\n";

	if ( !readFile(file, filesystem) )
		std::cout << "Couldn't open file \"" << file << "\"!\n";

	const auto start = std::chrono::system_clock::now();
	checksumPartI = calculateChecksumPartI(filesystem);
	const auto end = std::chrono::system_clock::now();

	const auto startPII = std::chrono::system_clock::now();
	//checksumPartII = countUniqueAntiNodeLocationsPartII(radarMap);
	const auto endPII = std::chrono::system_clock::now();

	std::cout << "Unique Locations of Antinodes: " << checksumPartI << " : " /* << checksumPartII*/ << "\n";
	std::cout << "Time of execution(P1): " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start) << "\n";
	std::cout << "Time of execution(P2): " << std::chrono::duration_cast<std::chrono::milliseconds>(endPII - startPII) << "\n";

	std::cin.get();
}

bool readFile(const std::string& filePath, std::string& data) {
	std::ifstream fileToRead(filePath);

	if ( !fileToRead.is_open() ) {
		std::cout << "Failed to open " << filePath << " ...\n";
		return false;
	} // if ( !fileToRead.is_open() )

	std::cout << "Reading file " << filePath << " ...\n";
	std::getline(fileToRead, data);

#ifdef DEBUG
	std::cout << "Length: " << data.size() << "\n";
	std::cout << "Line: " << data << "\n";
#endif

	fileToRead.close();
	std::cout << "Finished reading file " << filePath << " ...\n";
	return true;
}

int calculateChecksumPartI(const std::string& data) {
	int leftID = 0, rightID = static_cast<int>(data.size() / 2);
	int checksum = 0;

	std::cout << "Calculating Checksum for filesystem: " << data << "\n";
#ifdef DEBUG
	std::cout << "Data Length: " << data.size() << ". ";
	std::cout << "Max FileID: " << rightID << "\n";
	//std::cout << "Filesystem:\n" << filesystem << "\n";
#endif
	for ( auto [pos, symbol] : data | std::ranges::views::enumerate ) {
		int forwardSpaceCount = symbol - '0';
		int backwardSpaceCount = 0;

		if ( pos % 2 == 0 ) {
			for ( int forwardIndex = 0; forwardIndex < forwardSpaceCount; forwardIndex++ ) {
				checksum += leftID * pos;
			} // for ( int forwardIndex = 0; forwardIndex < forwardSpaceCount; forwardIndex++ )
			leftID++;
		} // if ( pos % 2 == 0 )
		else {
			for ( int reverseIndex = 0; reverseIndex < backwardSpaceCount; reverseIndex++ ) {
				checksum += rightID * pos;
			} // for ( int reverseIndex = 0; reverseIndex < backwardSpaceCount; reverseIndex++ )
		} // else
	} // for ( auto [pos, symbol] : data | std::ranges::views::enumerate )
	
	return 0;
}

std::string constructFilesystem(const std ::string& data) {
	std::string filesystem = "";
	bool file = true;
	int id = 0;
	for ( const auto& number : data ) {
		if ( file ) {
			for ( int num = number - '0'; num > 0; num-- ) {
				filesystem.append(std::to_string(id));
			} // for ( int num = number - '0'; num > 0; num-- )
			file = false;
			id++;
		} // if ( file )
		else if ( !file ) {
			for ( int num = number - '0'; num > 0; num-- ) {
				filesystem.append(".");
			} // for ( int num = number - '0'; num > 0; num-- )
			file = true;
		} // else if ( !file )
	} // for ( const auto& number : data )
	return filesystem;
}