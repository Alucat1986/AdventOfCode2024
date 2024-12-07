#include <algorithm>
#include <array>
#include <chrono>
#include <fstream>
#include <iostream>
#include <map>
#include <ranges>
#include <span>
#include <sstream>
#include <string>
#include <vector>

struct Calc {
	long long add(long long a, long long b);
	long long multiply(long long a, long long);
	long long concatenate(long long a, long long);
};

// Thanks to a friend of mine, github.com/HazardyKnusperkeks
static constexpr auto logTable = [](void) noexcept {
	std::array<std::int64_t, 12> ret;
	ret[0] = 1;
	std::ranges::generate(ret | std::views::drop(1), [log = std::int64_t{ 1 }](void) mutable noexcept {
		log *= 10;
		return log;
	});
	return ret;
}();

bool readFile(const std::string& filePath, std::multimap<long long, std::vector<long long>>& data);
long long sumCalibrationResults(const std::multimap<long long, std::vector<long long>>& data);
long long sumCalibrationResultsSpan(const std::multimap<long long, std::vector<long long>>& data);
bool tryForAddition(const long long& sum, const std::vector<long long>& summands);
bool tryForMultiplication(const long long& product, const std::vector<long long >& factors);
bool tryForCombinedAddAndMul(const long long sum, long long currentSum, std::vector<long long> factors);
bool tryForCombinedAddAndMulSpan(const long long sum, long long currentSum, std::span<const long long> factors);

int main() {
	char fileToLoad;
	std::string file = "input\\";
	std::multimap<long long, std::vector<long long>> equations;

	std::cout << "Loading (E)xample or (I)nput? ";
	fileToLoad = std::cin.get();

	if ( fileToLoad == 'E' || fileToLoad == 'e' )
		file += "example.txt";
	else if ( fileToLoad == 'I' || fileToLoad == 'i' )
		file += "input.txt";
	else
		std::cout << "No File found with that name\n";

	if ( !readFile(file, equations) )
		std::cout << "Couldn't open file \"" << file << "\"!\n";

	const auto start = std::chrono::system_clock::now();
	std::cout << "Sum of Calibration Results: " << sumCalibrationResults(equations) << "\n";
	const auto end = std::chrono::system_clock::now();
	
	const auto startt = std::chrono::system_clock::now();
	std::cout << "Sum of Calibration Results: " << sumCalibrationResultsSpan(equations) << "\n";
	const auto endd = std::chrono::system_clock::now();

	std::cout << "Time of execution(copy): " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start) << "\n";
	std::cout << "Time of execution(span): " << std::chrono::duration_cast<std::chrono::milliseconds>(endd - startt) << "\n";

	std::cin.get();
}

long long Calc::add(long long a, long long b) {
	return a + b;
}

long long Calc::multiply(long long a, long long b) {
	if ( !a || !b )
		return Calc::add(a, b);
	else
		return a *= b;
}

long long Calc::concatenate(long long a, long long b) {
	if ( !a || !b )
		return Calc::add(a, b);
	else {
		const auto log = std::ranges::upper_bound(logTable, b);
		return a * *log + b;
	}
}

bool readFile(const std::string& filePath, std::multimap<long long, std::vector<long long>>& data) {
	std::ifstream fileToRead(filePath);
	std::string line;

	if ( !fileToRead.is_open() ) {
		std::cout << "Failed to open " << filePath << " ...\n";
		return false;
	}

	std::cout << "Reading file " << filePath << " ...\n";
	while ( std::getline(fileToRead, line) ) {
		std::istringstream iStringStream(line);
		std::vector<long long> lineVector;
		long long lineKey = 0;
		long long value = 0;
		char colon;

#ifdef DEBUG
		std::cout << "Length: " << line.size() << "\t";
		std::cout << "Line: " << line;
		std::cout << "\t\t|\t";
#endif

		// Assuming the lines are all without errors
		iStringStream >> lineKey >> colon;

		while ( iStringStream >> value ) {
			lineVector.push_back(value);
		}

		data.insert({ lineKey, lineVector });

#ifdef DEBUG
		std::cout << "Key: " << lineKey << "\t";
		std::cout << "Values: ";
		for ( const auto& element : lineVector ) {
			std::cout << element << " ";
		}
		std::cout << "\n\n";
#endif
	} // while ( std::getline(fileToRead, line) )

	fileToRead.close();
	std::cout << "Finished reading file " << filePath << " ...\n";
	return true;
}

long long sumCalibrationResults(const std::multimap<long long, std::vector<long long>>& data) {
	long long sumOfResults = 0;
	for ( const auto& [result, values] : data ) {
		if ( tryForCombinedAddAndMul(result, 0, values) ) {
#ifdef DEBUG
			std::cout << "\nKey: " << result << " Values: ";
			for ( const auto& value : values ) {
				std::cout << value << " ";
			}
			std::cout << " solvable!\n";
#endif
			sumOfResults += result;
		}
	}
	return sumOfResults;
}

long long sumCalibrationResultsSpan(const std::multimap<long long, std::vector<long long>>& data) {
	long long sumOfResults = 0;
	for ( const auto& [result, values] : data ) {
		if ( tryForCombinedAddAndMulSpan(result, 0, std::span<const long long>(values)) ) {
#ifdef DEBUG
			std::cout << "\nKey: " << result << " Values: ";
			for ( const auto& value : values ) {
				std::cout << value << " ";
			}
			std::cout << " solvable!\n";
#endif
			sumOfResults += result;
		}
	}
	return sumOfResults;
}

// Deprecated, used for earlier stuff
bool tryForAddition(const long long& sum, const std::vector<long long>& summands) {
	return sum == std::ranges::fold_left(summands, 0, std::plus<long long>());
}

// Deprecated, used for earlier stuff
bool tryForMultiplication(const long long& product, const std::vector<long long >& factors) {
	return product == std::ranges::fold_left(factors, 1, std::multiplies<long long>());
}

bool tryForCombinedAddAndMul(const long long sum, long long currentSum, std::vector<long long> factors) {
	if ( factors.empty() )
		return sum == currentSum;
	else {
		Calc calc;
		return tryForCombinedAddAndMul(sum, calc.add(currentSum, *factors.begin()), std::vector<long long>(factors.begin() + 1, factors.end())) ||
			   tryForCombinedAddAndMul(sum, calc.multiply(currentSum, *factors.begin()), std::vector<long long>(factors.begin() + 1, factors.end())) ||
			   tryForCombinedAddAndMul(sum, calc.concatenate(currentSum, *factors.begin()), std::vector<long long>(factors.begin() + 1, factors.end()));
	}
}

bool tryForCombinedAddAndMulSpan(const long long sum, long long currentSum, std::span<const long long> factors) {
	if ( factors.empty() )
		return sum == currentSum;
	else {
		Calc calc;
		return tryForCombinedAddAndMulSpan(sum, calc.add(currentSum, *factors.begin()), factors.subspan(1)) ||
			   tryForCombinedAddAndMulSpan(sum, calc.multiply(currentSum, *factors.begin()), factors.subspan(1)) ||
			   tryForCombinedAddAndMulSpan(sum, calc.concatenate(currentSum, *factors.begin()), factors.subspan(1));
	}
}