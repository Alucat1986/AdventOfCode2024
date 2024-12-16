#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <ranges>
#include <unordered_map>
#include <stack>
#include <string>
#include <string_view>

#include "debug.hpp"

using WarehouseMap = std::unordered_map<std::int64_t, std::string>;

enum class Direction {
	Up,
	Right,
	Down,
	Left
};

struct Velocity {
	std::int64_t VelX;
	std::int64_t VelY;
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
};

bool readFile(const std::string& filePath, Robot& robot, std::stack<Direction>& moveCommands, WarehouseMap& warehouse);
void parseCommands(std::string& commands, std::stack<Direction>& moveCommands);
void printMap(const WarehouseMap& warehouse);
void printRobot(const Robot& robot);

std::int64_t sumAllGPSLocations(const WarehouseMap& warehouse);
void moveRobot(WarehouseMap& warehouse, Robot& robot, std::stack<Direction>& moveCommands);
void moveBoxes(WarehouseMap& warehouse, const Robot& robot, const Direction& moveCommand);
bool isColliding(WarehouseMap& warehouse, const Robot& robot, Direction dirToMoveTo);

void modifyMapforPartII(const WarehouseMap& warehouse, WarehouseMap& widerWarehouse);
void findRobotII(const WarehouseMap& warehouse, Robot& robot);
/*
void moveRobotII(WarehouseMap& warehouse, Robot& robot, std::stack<Direction>& moveCommands);
void moveBoxesII(WarehouseMap& warehouse, const Robot& robot, const Direction& moveCommand);
bool isCollidingPartII(WarehouseMap& warehouse, const Robot& robot, Direction dirToMoveTo);
*/

int main() {
	char fileToLoad;
	std::string file = "input\\";
	std::int64_t partI = 0, partII = 0;

	Robot robot;
	std::stack<Direction> moveCommands;
	WarehouseMap warehouse;

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

	if ( !readFile(file, robot, moveCommands, warehouse) )
		std::cout << "Couldn't open file \"" << file << "\"!\n";

	printMap(warehouse);
	printRobot(robot);

	std::stack<Direction> moveCommandsPartII = moveCommands;
	Robot robotII;
	WarehouseMap warehousePartII;
	modifyMapforPartII(warehouse, warehousePartII);
	findRobotII(warehousePartII, robotII);

	printMap(warehousePartII);
	printRobot(robotII);

	const auto start = std::chrono::system_clock::now();
	moveRobot(warehouse, robot, moveCommands);
	partI = sumAllGPSLocations(warehouse);
	const auto end = std::chrono::system_clock::now();

	const auto startt = std::chrono::system_clock::now();
	//moveRobot(warehousePartII, robot, moveCommandsPartII);
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

bool readFile(const std::string& filePath, Robot& robot, std::stack<Direction>& moveCommands, WarehouseMap& warehouse) {
	std::ifstream fileToRead(filePath);
	std::string line = "";

	if ( !fileToRead.is_open() ) {
		std::cout << "Failed to open " << filePath << " ...\n";
		return false;
	}
	std::cout << "\nReading file " << filePath << " ...\n\n";

	std::int64_t row = 0;
	std::string commands = "";
	bool mapAndCommandsSwitch = false; // false = map time
	while ( std::getline(fileToRead, line) ) {
		debugLog("Length: ", line.size(), "\t", "Line: ", line);
		if ( line.empty() ) {
			mapAndCommandsSwitch = true;
			continue;
		} // if ( line.empty() )

		if ( !mapAndCommandsSwitch ) {
			warehouse.insert({ row, line });

			std::size_t pos = line.find("@");
			if ( pos != std::string::npos ) {
				robot.CurrentPos.PosX = pos;
				robot.CurrentPos.PosY = row;
			} // if ( pos != std::string::npos ) {

			row++;
		} // if ( !mapAndCommandsSwitch )
		else if ( mapAndCommandsSwitch ) {
			commands += line;
		} // else if ( mapAndCommandsSwitch )
	} // while ( std::getline(fileToRead, line) )

	parseCommands(commands, moveCommands);
	
	fileToRead.close();
	std::cout << "\nFinished reading file " << filePath << " ...\n\n";

	return true;
}

void parseCommands(std::string& commands, std::stack<Direction>& moveCommands) {
	for ( auto revIter = commands.rbegin(); revIter != commands.rend(); revIter++ ) {
		if ( *revIter == '^' )
			moveCommands.push(Direction::Up);
		else if ( *revIter == '>' )
			moveCommands.push(Direction::Right);
		else if ( *revIter == 'v' )
			moveCommands.push(Direction::Down);
		else if ( *revIter == '<' )
			moveCommands.push(Direction::Left);
		else
			std::cout << "Unknown Move Command: " << *revIter << ".\n";
	} // for ( auto revIter = commands.rbegin(); revIter != commands.rend(); revIter++ )
}

void printMap(const WarehouseMap& warehouse) {
	std::cout << "\n";
	for ( const auto& row : warehouse ) {
		std::cout << row.first << ":\t" << row.second << "\n";
	} // for ( const auto& row : warehouse )
}

void printRobot(const Robot& robot) {
	std::cout << "\nRobot is at R(" << robot.CurrentPos.PosX << "/" << robot.CurrentPos.PosY << ")\n";
}

std::int64_t sumAllGPSLocations(const WarehouseMap& warehouse) {
	auto result = 0;
	auto boxes = [](auto colAndSym) { return std::get<1>(colAndSym) == 'O'; };
	for ( const auto& row : warehouse ) {
		if ( std::get<1>(row).find('O') == std::string::npos )
			continue;
		for ( auto [column, symbol] : std::get<1>(row) | std::ranges::views::enumerate 
													   | std::ranges::views::filter(boxes) ) {
			result += 100 * std::get<0>(row) + column;
		} // for ( auto [column, symbol] : row | std::ranges::views::enumerate )

//		auto [column, symbol] = row | std::ranges::views::enumerate
//									| std::ranges::views::filter(boxes)
//									| std::ranges::views::transform();
	} // for ( const auto& row : warehouse )

	return result;
}

void moveRobot(WarehouseMap& warehouse, Robot& robot, std::stack<Direction>& moveCommands) {
	Direction moveDirection;
	while ( !moveCommands.empty() ) {
		moveDirection = moveCommands.top();
		if ( !isColliding(warehouse, robot, moveDirection) ) {
			moveBoxes(warehouse, robot, moveDirection);
			auto row = robot.CurrentPos.PosY;
			auto column = robot.CurrentPos.PosX;
			switch ( moveDirection ) {
				case Direction::Up:
					warehouse[row][column] = '.';
					warehouse[row - 1][column] = '@';
					robot.CurrentPos.PosY--;
					break;
				case Direction::Right:
					warehouse[row][column] = '.';
					warehouse[row][column + 1] = '@';
					robot.CurrentPos.PosX++;
					break;
				case Direction::Down:
					warehouse[row][column] = '.';
					warehouse[row + 1][column] = '@';
					robot.CurrentPos.PosY++;
					break;
				case Direction::Left:
					warehouse[row][column] = '.';
					warehouse[row][column - 1] = '@';
					robot.CurrentPos.PosX--;
					break;
				default:
					std::cout << "Shouldn't be here either I guess!\n";
					break;
			} // switch ( moveDirection )
		} // if ( !isColliding(warehouse, robot, moveDirection) ) {
		moveCommands.pop();
	} // while ( !moveCommands.empty() ) {
}

void moveBoxes(WarehouseMap& warehouse, const Robot& robot, const Direction& moveCommand) {
	auto row = robot.CurrentPos.PosY;
	auto column = robot.CurrentPos.PosX;

	switch ( moveCommand ) {
		case Direction::Up:
			if ( warehouse[row - 1][column] == '.' ) {
				return;
			} // if ( warehouse[row - 1][column] )
			else if ( warehouse[row - 1][column] == 'O' ) {
				do {
					row--;
				} while ( warehouse[row][column] == 'O' );
				warehouse[row][column] = 'O';
			} // else if ( warehouse[row - 1] == 'O' )
			break;
		case Direction::Right:
			if ( warehouse[row][column + 1] == '.' ) {
				return;
			} // if ( warehouse[row][column + 1] )
			else if ( warehouse[row][column + 1] == 'O' ) {
				do {
					column++;
				} while ( warehouse[row][column] == 'O' );
				warehouse[row][column] = 'O';
			} // else if ( warehouse[row - 1] == 'O' )
			break;
		case Direction::Down:
			if ( warehouse[row + 1][column] == '.' ) {
				return;
			} // if ( warehouse[row + 1][column] )
			else if ( warehouse[row + 1][column] == 'O' ) {
				do {
					row++;
				} while ( warehouse[row][column] == 'O' );
				warehouse[row][column] = 'O';
			} // else if ( warehouse[row + 1] == 'O' )
			break;
		case Direction::Left:
			if ( warehouse[row][column - 1] == '.' ) {
				return;
			} // if ( warehouse[row][column - 1] )
			else if ( warehouse[row][column - 1] == 'O' ) {
				do {
					column--;
				} while ( warehouse[row][column] == 'O' );
				warehouse[row][column] = 'O';
			} // else if ( warehouse[row - 1] == 'O' )
			break;
		default:
			break;
	} // switch ( moveCommand ) {
}

// Does not only check if the Robot is collding but also if all boxes in a line do
bool isColliding(WarehouseMap& warehouse, const Robot& robot, Direction dirToMoveTo) {
	switch ( dirToMoveTo ) {
		case Direction::Up: {
			auto column = static_cast<std::size_t>(robot.CurrentPos.PosX);
			for ( auto row = robot.CurrentPos.PosY - 1; ; row-- ) {
				std::string_view symbol = warehouse[row];
				if ( symbol[column] == '.' ) {
					return false;
				} // if ( symbol == '.' )
				else if ( symbol[column] == '#' ) {
					return true;
				} // else if ( symbol == '#' )
				else {
					continue;
				} // else
			} // for ( auto row = robot.CurrentPos.PosY - 1; ; row-- )
			break;
		}
		case Direction::Right: {
			auto row = robot.CurrentPos.PosY;
			std::string_view symbol = warehouse[row];
			for ( auto column = static_cast<std::size_t>(robot.CurrentPos.PosX) + 1; ; column++ ) {
				if ( symbol[column] == '.' ) {
					return false;
				} // if ( symbol[column] == '.' )
				else if ( symbol[column] == '#' ) {
					return true;
				} // else if ( symbol[column] == '#' )
				else {
					continue;
				} // else
			} // for ( auto column = robot.CurrentPos.PosX + 1; ; column++ )
			break;
		}
		case Direction::Down: {
			auto column = static_cast<std::size_t>(robot.CurrentPos.PosX);
			for ( auto row = robot.CurrentPos.PosY + 1; ; row++ ) {
				std::string_view symbol = warehouse[row];
				if ( symbol[column] == '.' ) {
					return false;
				} // if ( symbol[column] == '.' )
				else if ( symbol[column] == '#' ) {
					return true;
				} // else if ( symbol[column] == '#' )
				else {
					continue;
				} // else
			} // for ( auto row = static_cast<std::size_t>(robot.CurrentPos.PosY) + 1; ; row++ )
			break;
		}
		case Direction::Left: {
			auto row = robot.CurrentPos.PosY;
			std::string_view symbol = warehouse[row];
			for ( auto column = static_cast<std::size_t>(robot.CurrentPos.PosX) - 1; ; column-- ) {
				if ( symbol[column] == '.' ) {
					return false;
				} // if ( symbol[column] == '.' )
				else if ( symbol[column] == '#' ) {
					return true;
				} // else if ( symbol[column] == '#' )
				else {
					continue;
				} // else
			} // for ( auto column = robot.CurrentPos.PosX - 1; ; column-- )
			break;
		}
		default:
			std::cout << "I should not be here... weird move!\n";
			break;
	} // switch ( dirToMoveTo ) {
	return false;
}

void modifyMapforPartII(const WarehouseMap& warehouse, WarehouseMap& widerWarehouse) {
	for ( const auto& [rIndex, row] : warehouse ) {
		std::string newRow = "";
		for ( const auto& symbol : row ) {
			if ( symbol == '#' )
				newRow += "##";
			else if ( symbol == '.' )
				newRow += "..";
			else if ( symbol == 'O' )
				newRow += "[]";
			else if ( symbol == '@' )
				newRow += "@.";
		} // for ( const auto& symbol : std::get<1>(row) )
		widerWarehouse.insert({ rIndex, newRow });
	} // for ( const auto& row : warehouse )
}

void findRobotII(const WarehouseMap& warehouse, Robot& robot) {
	for ( const auto& [rIndex, row] : warehouse ) {
		if ( row.find('@') == std::string::npos )
			continue;
		else {
			std::int64_t pos = row.find('@');
			robot.CurrentPos.PosX = pos;
			robot.CurrentPos.PosY = rIndex;
		}
	}
}
/*
void moveRobotII(WarehouseMap& warehouse, Robot& robot, std::stack<Direction>& moveCommands) {
	Direction moveDirection;
	while ( !moveCommands.empty() ) {
		moveDirection = moveCommands.top();
		if ( !isCollidingPartII(warehouse, robot, moveDirection) ) {
			moveBoxesPartII(warehouse, robot, moveDirection);
			auto row = robot.CurrentPos.PosY;
			auto column = robot.CurrentPos.PosX;
			switch ( moveDirection ) {
			case Direction::Up:
				warehouse[row][column] = '.';
				warehouse[row - 1][column] = '@';
				robot.CurrentPos.PosY--;
				break;
			case Direction::Right:
				warehouse[row][column] = '.';
				warehouse[row][column + 1] = '@';
				robot.CurrentPos.PosX++;
				break;
			case Direction::Down:
				warehouse[row][column] = '.';
				warehouse[row + 1][column] = '@';
				robot.CurrentPos.PosY++;
				break;
			case Direction::Left:
				warehouse[row][column] = '.';
				warehouse[row][column - 1] = '@';
				robot.CurrentPos.PosX--;
				break;
			default:
				std::cout << "Shouldn't be here either I guess!\n";
				break;
			} // switch ( moveDirection )
		} // if ( !isColliding(warehouse, robot, moveDirection) ) {
		moveCommands.pop();
	} // while ( !moveCommands.empty() ) {
}

void moveBoxesII(WarehouseMap& warehouse, const Robot& robot, const Direction& moveCommand) {
	auto row = robot.CurrentPos.PosY;
	auto column = robot.CurrentPos.PosX;

	switch ( moveCommand ) {
	case Direction::Up:
		row--;
		break;
	case Direction::Right:
		column++;
		break;
	case Direction::Down:
		row++;
		break;
	case Direction::Left:
		column--;
		break;
	default:
		break;
	} // switch ( moveCommand ) {
}

// Does not only check if the Robot is collding but also if all boxes in a line do
bool isCollidingPartII(WarehouseMap& warehouse, const Robot& robot, Direction dirToMoveTo) {
	switch ( dirToMoveTo ) {
	case Direction::Up:
	{
		auto column = static_cast<std::size_t>(robot.CurrentPos.PosX);
		for ( auto row = robot.CurrentPos.PosY - 1; ; row-- ) {
			std::string_view symbol = warehouse[row];
			if ( symbol[column] == '.' ) {
				return false;
			} // if ( symbol == '.' )
			else if ( symbol[column] == '#' ) {
				return true;
			} // else if ( symbol == '#' )
			else {
				continue;
			} // else
		} // for ( auto row = robot.CurrentPos.PosY - 1; ; row-- )
		break;
	}
	case Direction::Right:
	{
		auto row = robot.CurrentPos.PosY;
		std::string_view symbol = warehouse[row];
		for ( auto column = static_cast<std::size_t>(robot.CurrentPos.PosX) + 1; ; column++ ) {
			if ( symbol[column] == '.' ) {
				return false;
			} // if ( symbol[column] == '.' )
			else if ( symbol[column] == '#' ) {
				return true;
			} // else if ( symbol[column] == '#' )
			else {
				continue;
			} // else
		} // for ( auto column = robot.CurrentPos.PosX + 1; ; column++ )
		break;
	}
	case Direction::Down:
	{
		auto column = static_cast<std::size_t>(robot.CurrentPos.PosX);
		for ( auto row = robot.CurrentPos.PosY + 1; ; row++ ) {
			std::string_view symbol = warehouse[row];
			if ( symbol[column] == '.' ) {
				return false;
			} // if ( symbol[column] == '.' )
			else if ( symbol[column] == '#' ) {
				return true;
			} // else if ( symbol[column] == '#' )
			else {
				continue;
			} // else
		} // for ( auto row = static_cast<std::size_t>(robot.CurrentPos.PosY) + 1; ; row++ )
		break;
	}
	case Direction::Left:
	{
		auto row = robot.CurrentPos.PosY;
		std::string_view symbol = warehouse[row];
		for ( auto column = static_cast<std::size_t>(robot.CurrentPos.PosX) - 1; ; column-- ) {
			if ( symbol[column] == '.' ) {
				return false;
			} // if ( symbol[column] == '.' )
			else if ( symbol[column] == '#' ) {
				return true;
			} // else if ( symbol[column] == '#' )
			else {
				continue;
			} // else
		} // for ( auto column = robot.CurrentPos.PosX - 1; ; column-- )
		break;
	}
	default:
		std::cout << "I should not be here... weird move!\n";
		break;
	} // switch ( dirToMoveTo ) {
	return false;
}*/