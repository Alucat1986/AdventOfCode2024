#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <ranges>
#include <unordered_map>
#include <sstream>
#include <string>
#include <vector>

#include "debug.hpp"

static constexpr std::int64_t BathroomWidth = 101;
static constexpr std::int64_t BathroomHeight = 103;
static constexpr std::int64_t RobotMotionTimer = 100;

struct Velocity {
	std::int64_t VelX;
	std::int64_t VelY;

	Velocity operator*(std::int64_t scalar) const {
		return { VelX * scalar, VelY * scalar };
	}
};

struct Position {
	std::int64_t PosX;
	std::int64_t PosY;

	Position& operator+=(const Velocity& vel) {
		PosX += vel.VelX;
		PosY += vel.VelY;
		return *this;
	}
};

struct Robot {
	Position CurrentPos;
	Velocity Speed;
	Robot(Position& pos, Velocity& vel) : CurrentPos(pos), Speed(vel) {};
};

bool readFile(const std::string& filePath, std::vector<Robot>& robots);
std::int64_t calculateSafetyFactor(std::vector<Robot>& robots);
void moveRobot(Robot& robot);
void fitToMap(Robot& robot);

int main() {
	char fileToLoad;
	std::string file = "input\\";
	std::vector<Robot> robots;
	std::int64_t partI = 0, partII = 0;

	std::cout << "Loading (E)xample or (I)nput? ";
	fileToLoad = std::cin.get();

	if ( fileToLoad == 'E' || fileToLoad == 'e' )
		file += "example.txt";
	else if ( fileToLoad == 'I' || fileToLoad == 'i' )
		file += "input.txt";
	else
		std::cout << "No File found with that name\n";

	if ( !readFile(file, robots) )
		std::cout << "Couldn't open file \"" << file << "\"!\n";

	const auto start = std::chrono::system_clock::now();
	partI = calculateSafetyFactor(robots);
	const auto end = std::chrono::system_clock::now();

	const auto startt = std::chrono::system_clock::now();
	//partII = plutonianPebblesPartII(pebbles);
	const auto endd = std::chrono::system_clock::now();

	std::cout << "\n===========================================================\n";
	std::cout << "PartI:\t" << partI << "\tTime of execution:\t";
	std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - start) << "\n";
	std::cout << "PartII:\t" << partII << "\tTime of execution:\t";
	std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(endd - startt) << "\n";
	std::cout << "===========================================================\n";

	std::cin.get();
}

bool readFile(const std::string& filePath, std::vector<Robot>& robots) {
	std::ifstream fileToRead(filePath);
	std::string line = "";

	if ( !fileToRead.is_open() ) {
		std::cout << "Failed to open " << filePath << " ...\n";
		return false;
	}
	std::cout << "\nReading file " << filePath << " ...\n\n";

	while ( std::getline(fileToRead, line) ) {
		debugLog("Length: ", line.size(), "\t", "Line: ", line);

		std::stringstream sStream(line);
		std::string point, velocity;
		sStream >> point >> velocity;

		point = point.substr(2);
		velocity = velocity.substr(2);

		std::size_t comma = point.find(",");
		Position pos = { std::stoll(point.substr(0, comma)), std::stoll(point.substr(comma + 1)) };
		
		comma = velocity.find(",");
		Velocity vel = { std::stoll(velocity.substr(0, comma)), std::stoll(velocity.substr(comma+1)) };
		
		debugLog("P(", pos.PosX, "/", pos.PosY, "), V(", vel.VelX, "/", vel.VelY, ")");

		robots.emplace_back(Robot{ pos, vel });
	} // while ( std::getline(fileToRead, line) )
	
	fileToRead.close();
	std::cout << "\nFinished reading file " << filePath << " ...\n\n";

	return true;
}

std::int64_t calculateSafetyFactor(std::vector<Robot>& robots) {
	std::int64_t safetyFactor = 1;

	for ( auto& robot : robots ) {
		moveRobot(robot);
	}

	std::erase_if(robots, [](Robot robot) { return (robot.CurrentPos.PosX == BathroomWidth / 2) ||
				  (robot.CurrentPos.PosY == BathroomHeight / 2); });

	// Not nice but maybe works?
	std::vector<Robot> quadrantOne, quadrantTwo, quadrantThree, quadrantFour;
	for ( auto& robot : robots ) {
		if ( robot.CurrentPos.PosX < BathroomWidth / 2 ) {
			if ( robot.CurrentPos.PosY < BathroomHeight / 2 )
				quadrantOne.push_back(robot);
			else if ( robot.CurrentPos.PosY > BathroomHeight / 2 )
				quadrantThree.push_back(robot);
		} // if ( robot.CurrentPos.PosX < BathroomWidth / 2 )
		else if ( robot.CurrentPos.PosX > BathroomWidth / 2 ) {
			if ( robot.CurrentPos.PosY < BathroomHeight / 2 )
				quadrantTwo.push_back(robot);
			else if ( robot.CurrentPos.PosY > BathroomHeight / 2 )
				quadrantFour.push_back(robot);
		} // else if ( robot.CurrentPos.PosX > BathroomWidth / 2 )
	} // for ( auto& robot : robots )
	
	safetyFactor *= quadrantOne.size();
	safetyFactor *= quadrantTwo.size();
	safetyFactor *= quadrantThree.size();
	safetyFactor *= quadrantFour.size();

	return safetyFactor;
}

void moveRobot(Robot& robot) {
	robot.CurrentPos += robot.Speed * RobotMotionTimer;
	fitToMap(robot);
}

void fitToMap(Robot& robot) {
	robot.CurrentPos.PosX = (robot.CurrentPos.PosX % BathroomWidth + BathroomWidth) % BathroomWidth;
	robot.CurrentPos.PosY = (robot.CurrentPos.PosY % BathroomHeight + BathroomHeight) % BathroomHeight;
	/*
	if ( robot.CurrentPos.PosX < 0 ) {
		while ( robot.CurrentPos.PosX < 0 ) {
			robot.CurrentPos.PosX += BathroomWidth;
		} // while ( robot.CurrentPos.PosX < 0 )
	} // if ( robot.CurrentPos.PosX < 0 ) {
	else if ( robot.CurrentPos.PosX >= BathroomWidth ) {
		while ( robot.CurrentPos.PosX >= BathroomWidth ) {
			robot.CurrentPos.PosX -= BathroomWidth;
		} // while ( robot.CurrentPos.PosX < 0 )
	} // else if ( robot.CurrentPos.PosX > BathroomWidth )
	
	if ( robot.CurrentPos.PosY < 0 ) {
		while ( robot.CurrentPos.PosY < 0 ) {
			robot.CurrentPos.PosY += BathroomHeight;
		} // while ( robot.CurrentPos.PosX < 0 )
	} // if ( robot.CurrentPos.PosY < 0 )
	else if ( robot.CurrentPos.PosY >= BathroomHeight ) {
		while ( robot.CurrentPos.PosY >= BathroomHeight ) {
			robot.CurrentPos.PosY -= BathroomHeight;
		} // while ( robot.CurrentPos.PosX < 0 )
	} // else if ( robot.CurrentPos.PosY > BathroomHeight ) {
	*/
	debugLog("R(", robot.CurrentPos.PosX, "/", robot.CurrentPos.PosY, ") -> Robot fits now into M(",
			 BathroomWidth, "/", BathroomHeight, ").");
}