#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

bool readFile(const std::string& filePath, std::multimap<int, int>& rules, std::vector<std::vector<int>>& data);
void checkRules(const std::multimap<int, int>& rules, std::vector<std::vector<int>>& data, std::vector<std::vector<int>>& ruleBreakers);
void sumMiddlePages(const std::vector<std::vector<int>>& data);
void enforceRules(const std::multimap<int, int>& rules, std::vector<std::vector<int>>& ruleBreakers);
void printRules(const std::multimap<int, int>& rules);
void printManuals(const std::vector<std::vector<int>>& manuals);

int main() {
	char fileToLoad;
	std::string file = "input\\";
	std::multimap<int, int> rulesMap;
	std::vector<std::vector<int>> safetyManuals;
	std::vector<std::vector<int>> wrongSafetyManuals;
	std::vector<std::vector<int>> stillWrongSafetyManuals;
	std::vector<std::vector<int>> fixedSafetyManuals;

	std::cout << "Loading (E)xample or (I)nput? ";
	fileToLoad = std::cin.get();

	if ( fileToLoad == 'E' || fileToLoad == 'e' )
		file += "example.txt";
	else if ( fileToLoad == 'I' || fileToLoad == 'i' )
		file += "input.txt";
	else
		std::cout << "No File found with that name\n";

	if ( !readFile(file, rulesMap, safetyManuals) )
		std::cout << "Couldn't open file \"" << file << "\"!\n";

	printRules(rulesMap);
	printManuals(safetyManuals);
	checkRules(rulesMap, safetyManuals, wrongSafetyManuals);
	std::cout << "\nAfter checking with the Rules:\n";
	printManuals(safetyManuals);
	std::cout << "\nRule Breakers:\n";
	printManuals(wrongSafetyManuals);
	std::cout << "\nFixing Manuals...\n";
	do {
		stillWrongSafetyManuals.clear();
		enforceRules(rulesMap, wrongSafetyManuals);
		checkRules(rulesMap, wrongSafetyManuals, stillWrongSafetyManuals);
		fixedSafetyManuals.append_range(wrongSafetyManuals);
		wrongSafetyManuals.swap(stillWrongSafetyManuals);
		std::cout << "fix...\t";
		/*
		for ( size_t count = wrongSafetyManuals.size(); count >= 0; count-- ) {
			fixedSafetyManuals.emplace_back(wrongSafetyManuals[count]);
			wrongSafetyManuals.swap(stillWrongSafetyManuals);
			stillWrongSafetyManuals.clear();
		}*/
	} while ( !stillWrongSafetyManuals.empty() );
	std::cout << "\n";
	sumMiddlePages(safetyManuals);
	sumMiddlePages(fixedSafetyManuals);
	std::cout << "Fixed:\n";
	printManuals(fixedSafetyManuals);

	std::cin.get();
}

bool readFile(const std::string& filePath, std::multimap<int, int>& rules, std::vector<std::vector<int>>& data) {
	std::ifstream fileToRead(filePath);
	std::vector<int> lineVector;
	std::string line;

	if ( !fileToRead.is_open() ) {
		std::cout << "Failed to open " << filePath << " ...\n";
		return false;
	}

	std::cout << "Reading rules " << filePath << " ...\n";
	while ( std::getline(fileToRead, line) ) {
		if ( line.empty() )
			break;
		std::cout << "Length: " << line.size() << "\t";
		std::cout << "Line: " << line;
		std::cout << "\n";

		size_t pos = line.find('|');
		rules.emplace(std::stoi(line.substr(0, pos)), std::stoi(line.substr(pos + 1, line.size())));
	} // while ( std::getline(fileToRead, line) )
	
	std::cout << "Reading manuals " << filePath << " ...\n";
	while ( std::getline(fileToRead, line) ) {
		std::cout << "Length: " << line.size() << "\t";
		std::cout << "Line: " << line;
		std::cout << "\n";

		size_t pos = line.find(',');
		for ( ; pos != line.npos; pos = line.find(',') ) {
			lineVector.emplace_back(std::stoi(line.substr(0, pos)));
			line = line.substr(pos+1);
		}
		lineVector.emplace_back(std::stoi(line));
		data.emplace_back(lineVector);
		lineVector.clear();
	} // while ( std::getline(fileToRead, line) )

	fileToRead.close();
	return true;
}

// ChatGPT helped a bit here, i was very much on the right way but I had one lil thought error
void checkRules(const std::multimap<int, int>& rules, std::vector<std::vector<int>>& data, std::vector<std::vector<int>>& ruleBreakers) {
	std::vector<size_t> toDelete;

	// Iterate through each manual in 'data'
	for ( size_t i = 0; i < data.size(); i++ ) {
		auto& vector = data[i];
		bool breaksRule = false;

		// Traverse the vector in reverse order
		for ( auto iter = vector.rbegin(); iter != vector.rend(); iter++ ) {
			int currentPage = *iter;

			// Get all rules where 'currentPage' is the "follower"
			auto [ruleBegin, ruleEnd] = rules.equal_range(currentPage);

			// Check if any of the required predecessors are in the wrong order
			for ( auto rulesIter = ruleBegin; rulesIter != ruleEnd; rulesIter++ ) {
				int requiredPredecessor = rulesIter->second;

				// Check if 'requiredPredecessor' appears *after* 'currentPage' in the vector
				if ( std::find(iter + 1, vector.rend(), requiredPredecessor) != vector.rend() ) {
					breaksRule = true;
					break;
				}
			}

			if ( breaksRule ) {
				toDelete.push_back(i); // Mark the manual as invalid
				break;
			}
		}
	}

	// Remove invalid manuals from 'data'
	for ( auto iter = toDelete.rbegin(); iter != toDelete.rend(); iter++ ) {
		ruleBreakers.emplace_back(*(data.begin() + *iter));
		data.erase(data.begin() + *iter);
	}
}

void sumMiddlePages(const std::vector<std::vector<int>>& data) {
	unsigned int result = 0;

	for ( const auto& vector : data ) {
		size_t pos = (vector.size() / 2 );
		result += vector[pos];
	}
	std::cout << "\nSum of all middle Pages: " << result << "\n";
}

void enforceRules(const std::multimap<int, int>& rules, std::vector<std::vector<int>>& ruleBreakers) {
	// Iterate through each manual in 'ruleBreakers'
	for ( size_t i = 0; i < ruleBreakers.size(); i++ ) {
		auto& vector = ruleBreakers[i];

		// Traverse the vector in reverse order
		for ( auto iter = vector.rbegin(); iter != vector.rend(); iter++ ) {
			int currentPage = *iter;

			// Get all rules where 'currentPage' is the "predecessor"
			auto [rulesBegin, rulesEnd] = rules.equal_range(currentPage);

			// Check if any of the required successors are in the wrong order
			for ( auto rulesIter = rulesBegin; rulesIter != rulesEnd; rulesIter++ ) {
				int requiredSuccessor = rulesIter->second;

				// Check if 'requiredSuccessor' appears *after* 'currentPage' in the vector
				std::vector<int>::reverse_iterator element = std::find(iter + 1, vector.rend(), requiredSuccessor);
				if ( element != vector.rend() ) {
					std::swap(*iter, *element);
				}
			}
		}
	}
}

void printRules(const std::multimap<int, int>& rules) {
	int count = 0;
	std::cout << "Rules:\n";
	for ( const auto& elements : rules ) {
		std::cout << " [" << elements.first << "|" << elements.second << "]";
		count++;

		if ( count % 3 == 0 ) {
			std::cout << "\n";
		}
	}
	std::cout << "\n";
}

// printManuals powered by ChatGPT xD

void printManuals(const std::vector<std::vector<int>>& manuals) {
	// Check if there are manuals to print
	if ( manuals.empty() ) {
		std::cout << "No manuals available to display.\n";
		return;
	}

	// Iterate through each manual (vector)
	for ( size_t i = 0; i < manuals.size(); ++i ) {
		std::cout << "Manual " << i + 1 << ": [";

		for ( size_t j = 0; j < manuals[i].size(); ++j ) {
			std::cout << manuals[i][j];

			// Add a comma if not the last element
			if ( j < manuals[i].size() - 1 ) {
				std::cout << ", ";
			}
		}

		std::cout << "]\n";
	}
}