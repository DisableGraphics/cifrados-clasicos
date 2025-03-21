#include <iostream>
#include <unordered_map>
#include <vector>
#include "../Common/strprocess.hpp"

// Function to find repeated sequences and their positions
std::unordered_map<std::string, std::vector<int>> find_repeated_sequences(const std::string &msg, int seq_length) {
	std::unordered_map<std::string, std::vector<int>> sequences;
	for (size_t i = 0; i <= msg.size() - seq_length; ++i) {
		std::string seq = msg.substr(i, seq_length);
		sequences[seq].push_back(i);
	}
	return sequences;
}

// Function to calculate distances between repeated sequences
std::vector<int> calculate_distances(const std::vector<int>& positions) {
	std::vector<int> distances;
	for (size_t i = 1; i < positions.size(); ++i) {
		distances.push_back(positions[i] - positions[i - 1]);
	}
	return distances;
}

// Function to compute GCD of two numbers
int gcd(int a, int b) {
	while (b != 0) {
		int temp = b;
		b = a % b;
		a = temp;
	}
	return a;
}

// Function to calculate the GCD of a list of numbers
int gcd_of_list(const std::vector<int>& numbers) {
	int result = numbers[0];
	for (size_t i = 1; i < numbers.size(); ++i) {
		result = gcd(result, numbers[i]);
		if (result == 1) return 1; // early exit if GCD is 1
	}
	return result;
}

void kasiski(const std::string &msg, int seq_length) {
	auto sequences = find_repeated_sequences(msg, seq_length);

	// Step 2: Calculate distances and possible key lengths
	std::unordered_map<int, int> possible_key_lengths;
	for (const auto &entry : sequences) {
		if (entry.second.size() > 1) { // If sequence occurs multiple times
			auto distances = calculate_distances(entry.second);
			int gcd_value = gcd_of_list(distances);
			if (gcd_value > 1) {
				possible_key_lengths[gcd_value]++;
			}
		}
	}

	// Output possible key lengths
	std::cout << "Possible key lengths (factor frequency):\n";
	for (const auto &entry : possible_key_lengths) {
		std::cout << "Key length " << entry.first << ": " << entry.second << " occurrences\n";
	}

}

int main(int argc, char ** argv) {
	if(argc < 3) {
		std::cout << "Usage: " << argv[0] << " [msg] [seq_length]\n";
		return 1;
	}
	std::string msg = remove_delims(argv[1]);
	int seq_length = std::stoi(argv[2]);

	kasiski(msg, seq_length);

	return 0;
}