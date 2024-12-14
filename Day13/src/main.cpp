#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <map>
#include <numeric>
#include <queue>
#include <ranges>
#include <stack>
#include <sstream>
#include <string>
#include <vector>
#include <utility>

#include "debug.hpp"

struct Button {
	std::int64_t ValueX;
	std::int64_t ValueY;
	std::int64_t TokenCost;
};

struct Prize {
	std::int64_t CostX;
	std::int64_t CostY;
};

struct ClawMachine {
	Button ButtonA{ 0, 0, 3 };
	Button ButtonB{ 0, 0, 1 };
	Prize PrizeToWin{ 0, 0 };
};

struct Node {
	std::int64_t PosX;
	std::int64_t PosY;
};

bool readFile(const std::string& filePath, std::map<std::int64_t, ClawMachine>& data);
bool isSolvable(const ClawMachine& machine);
std::int64_t calculateTokensForAllWinnablePrizes(const std::map<std::int64_t, ClawMachine>& clawMachines);
std::int64_t solveMachine(const ClawMachine& machine);
std::int64_t traverseGraph(const Node& start, const Node& end, const ClawMachine& machine,
						   std::int64_t currentCost = 0);

int main() {
	char fileToLoad;
	std::string file = "input\\";
	std::map<std::int64_t, ClawMachine> clawMachines;
	std::int64_t partI = 0, partII = 0;

	std::cout << "Loading (E)xample or (I)nput? ";
	fileToLoad = static_cast<char>(std::cin.get());

	if ( fileToLoad == 'E' || fileToLoad == 'e' )
		file += "example.txt";
	else if ( fileToLoad == 'I' || fileToLoad == 'i' )
		file += "input.txt";
	else
		std::cout << "No File found with that name\n";

	if ( !readFile(file, clawMachines) )
		std::cout << "Couldn't open file \"" << file << "\"!\n";

	const auto start = std::chrono::system_clock::now();
	partI = calculateTokensForAllWinnablePrizes(clawMachines);
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

bool readFile(const std::string& filePath, std::map<std::int64_t, ClawMachine>& data) {
	std::ifstream fileToRead(filePath);

	if ( !fileToRead.is_open() ) {
		std::cout << "Failed to open " << filePath << " ...\n";
		return false;
	}

	std::cout << "\nReading file " << filePath << " ...\n\n";
	std::string line = "";
	std::int64_t machineNumber = 0;
	std::size_t lineCounter = 1;
	ClawMachine machine;

	while ( std::getline(fileToRead, line) ) {
		if ( line.empty() ) {
			debugLog("");
			continue;
		} // if ( line.empty() )

		//debugLog("Length: ", line.size(), "\t", "Line: ", line);
		std::string prefix, xStr, yStr;

		if ( line.find("Button") != std::string::npos ) {
			line = line.substr(6);

			std::int64_t x, y;
			std::stringstream strStream(line);
			strStream >> prefix >> xStr >> yStr;

			x = std::stoll(xStr.substr(2, xStr.size() - 1));
			y = std::stoll(yStr.substr(2));

			if ( prefix.substr(0, 1) == "A" ) {
				machine.ButtonA.ValueX = x;
				machine.ButtonA.ValueY = y;
			} // if ( prefix.substr(0, 1) == "A" )
			else if ( prefix.substr(0, 1) == "B" ) {
				machine.ButtonB.ValueX = x;
				machine.ButtonB.ValueY = y;
			} // if ( prefix.substr(0, 1) == "B" )
			else {
				std::cout << "This is not a correct Button?! " << prefix << ": " << xStr << " " << yStr << "\n";
			} // else

			debugLog("Prefix: ", prefix.substr(0, 1), " X: ", x, " Y: ", y);
		} // if ( line.find("Button") != std::string::npos )
		if ( line.find("Prize") != std::string::npos ) {
			std::int64_t x, y;
			std::stringstream strStream(line);
			strStream >> prefix >> xStr >> yStr;

			x = std::stoll(xStr.substr(2, xStr.size() - 1));
			y = std::stoll(yStr.substr(2));

			machine.PrizeToWin.CostX = x;
			machine.PrizeToWin.CostY = y;
			debugLog("Prefix: ", prefix.substr(0, prefix.size() - 1), " X: ", x, " Y: ", y);
		} // if ( line.find("Prize") != std::string::npos )
		
		if ( lineCounter % 3 == 0 ) {
			data.insert({ machineNumber, machine });
			machineNumber++;
		}
		lineCounter++;
	} // while ( std::getline(fileToRead, line) )

	fileToRead.close();

	std::cout << "\nFinished reading file " << filePath << " ...\n\n";
	return true;
}

bool isSolvable(const ClawMachine& machine) {
	std::int64_t commonDividerX = std::gcd(machine.ButtonA.ValueX, machine.ButtonB.ValueX);
	std::int64_t commonDividerY = std::gcd(machine.ButtonA.ValueY, machine.ButtonB.ValueY);
	debugLog("GCD-X: ", commonDividerX, " GCD-Y: ", commonDividerY);
	return (machine.PrizeToWin.CostX % commonDividerX == 0) && (machine.PrizeToWin.CostY % commonDividerY == 0);
}

std::int64_t calculateTokensForAllWinnablePrizes(const std::map<std::int64_t, ClawMachine>& clawMachines) {
	std::int64_t result = 0;
	for ( const auto& machine : clawMachines ) {
		bool solvable = isSolvable(machine.second);
		debugLog("ClawMachine ", machine.first + 1, " is solvable? ", solvable);

		if ( !solvable ) {
			continue;
		} // if ( !solvable )

		debugLog("Beginning to solve machine ", machine.first + 1, "...");

		result += solveMachine(machine.second);
	} // for ( const auto& machine : clawMachines )
	return result;
}

std::int64_t solveMachine(const ClawMachine& machine) {
	Node start = { 0, 0 };
	Node end = { machine.PrizeToWin.CostX, machine.PrizeToWin.CostY };
	std::int64_t result = 0;

	// A*x + B*x = PrizeX > A*x = PrizeX - B*x
	// A*y + B*y = PrizeY > A*y = PrizeY - B*y

	/* 1st Example Machine:
	* 94*A = 8400 - 22*B
	* 34*A = 5400 - 67*B
	* Least Common Multiple? 1598
	* Scales : 17 and 47
	* 17 * ( 8400 - 22 * B ) = 47 * ( 5400 - 67 * B )
	* 142800 - 374 * B = 253800 - 3149 * B
	* -374 * B + 3149 * B = 253800 - 142800
	* 2775 * B = 111000
	* B = 40
	*/
	std::int64_t aLCM = std::lcm(machine.ButtonA.ValueX, machine.ButtonA.ValueY);
	std::int64_t scaleX = aLCM / machine.ButtonA.ValueX;
	std::int64_t scaleY = aLCM / machine.ButtonA.ValueY;
	debugLog("aLCM: ", aLCM, " scaleX: ", scaleX, " scaleY: ", scaleY);

	std::int64_t left, right, final;
	left = scaleX * -machine.ButtonB.ValueX + scaleY * machine.ButtonB.ValueY;
	right = scaleY * machine.PrizeToWin.CostY - scaleX * machine.PrizeToWin.CostX;
	final = right / left;
	debugLog("left: ", left, " right: ", right, " final: ", final);

	result += final * machine.ButtonB.TokenCost;
	debugLog("result: ", result);
	result += ((machine.PrizeToWin.CostX - machine.ButtonB.ValueX * final) / machine.ButtonA.ValueX) * machine.ButtonA.TokenCost;
	debugLog("result: ", result);

	return result;
}