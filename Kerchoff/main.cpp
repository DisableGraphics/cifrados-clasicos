#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include "../Common/strprocess.hpp"

// Function to calculate the Index of Coincidence for a string
double calculate_index_of_coincidence(const std::string &text) {
	std::unordered_map<char, int> frequency;
	int N = 0;

	// Count frequency of each letter
	for (char c : text) {
		if (isalpha(c)) {
			char uppercase_char = toupper(c);
			frequency[uppercase_char]++;
			N++;
		}
	}

	// Calculate Index of Coincidence
	double IC = 0.0;
	for (const auto &entry : frequency) {
		int f_i = entry.second;
		IC += f_i * (f_i - 1);
	}
	IC /= (N * (N - 1));

	return IC;
}

// Function to split the ciphertext based on the guessed key length
std::vector<std::string> split_into_columns(const std::string &msg, int key_length) {
	std::vector<std::string> columns(key_length);
	for (size_t i = 0; i < msg.size(); ++i) {
		if (isalpha(msg[i])) {
			columns[i % key_length] += toupper(msg[i]);
		}
	}
	return columns;
}

// Function to apply Kerckhoffs method to find likely key lengths
void kerckhoffs_method(const std::string &msg, int max_key_length) {
	for (int key_length = 1; key_length <= max_key_length; ++key_length) {
		auto columns = split_into_columns(msg, key_length);
		
		// Calculate IC for each column and average them
		double total_IC = 0.0;
		int valid_columns = 0;
		for (const auto &column : columns) {
			if (column.size() > 1) { // Avoid divisions by zero or too small samples
				total_IC += calculate_index_of_coincidence(column);
				valid_columns++;
			}
		}
		double avg_IC = (valid_columns > 0) ? (total_IC / valid_columns) : 0;
		
		std::cout << "Key length " << key_length << " has average IC: " << avg_IC << std::endl;
	}
}

int main(int argc, char **argv) {
	if (argc < 3) {
		std::cout << "Usage: " << argv[0] << " [msg] [max_key_length]\n";
		return 1;
	}

	std::string msg = remove_delims(argv[1]);
	int max_key_length = std::stoi(argv[2]);

	// Apply Kerckhoffs method
	kerckhoffs_method(msg, max_key_length);

	return 0;
}
