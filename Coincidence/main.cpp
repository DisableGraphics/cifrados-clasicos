#include <cstdio>
#include <iostream>
#include <string>
#include <map>
#include "../Common/strprocess.hpp"

constexpr const char * spaces_prev = "   ";
constexpr const char * format = "%04d ";

void display(int N) {
	std::cout << "N: " << N << "\nN - 1: " << (N - 1) << "\nN*(N-1): " << (N-1)*N;
	std::cout << std::endl << std::endl;

	for (char c = 'A'; c <= 'Z'; c++) {
		std::cout << spaces_prev << c << " ";
	}
	std::cout << std::endl;
}

double calculate_index_of_coincidence(const std::string &msg) {
	std::map<char, int> frequency;
	int N = 0;

	// Count frequency of each letter
	for (char c : msg) {
		if (isalpha(c)) {
			char uppercase_char = toupper(c);
			frequency[uppercase_char]++;
			N++;
		}
	}

	display(N);

	// Calculate Index of Coincidence
	double IC = 0.0;
	char last_one = 'A';
	for (const auto &entry : frequency) {
		int f_i = entry.second;
		char current_one = entry.first;
		for(int i = 1; i < (current_one-last_one); i++) {
			std::cout << spaces_prev << '-' << " ";
		}
		printf(format, f_i);
		IC += f_i * (f_i - 1);

		last_one = entry.first;
	}

	for(char c = last_one; c < 'Z'; c++) {
		std::cout << spaces_prev << '-' << " ";
	}
	std::cout << std::endl;

	for (const auto &entry : frequency) {
		int f_i = entry.second;
		char current_one = entry.first;
		for(int i = 1; i < (current_one-last_one); i++) {
			std::cout << spaces_prev << '-' << " ";
		}
		printf(format, f_i - 1);

		last_one = entry.first;
	}

	for(char c = last_one; c < 'Z'; c++) {
		std::cout << spaces_prev << '-' << " ";
	}
	std::cout << std::endl;

	for (const auto &entry : frequency) {
		int f_i = entry.second;
		char current_one = entry.first;
		for(int i = 1; i < (current_one-last_one); i++) {
			std::cout << spaces_prev << '-' << " ";
		}
		printf(format, f_i*(f_i - 1));

		last_one = entry.first;
	}

	for(char c = last_one; c < 'Z'; c++) {
		std::cout << spaces_prev << '-' << " ";
	}
	std::cout << std::endl;

	std::cout << "IC = " << IC << "/" << (N*(N-1)) << std::endl;

	IC /= (N * (N - 1));

	return IC;
}

int main(int argc, char **argv) {
	if (argc < 2) {
		std::cout << "Usage: " << argv[0] << " [msg]\n";
		return 1;
	}

	std::string msg = remove_delims(argv[1]);

	// Calculate and output the Index of Coincidence
	double IC = calculate_index_of_coincidence(msg);
	std::cout << "IC: " << IC << std::endl;

	return 0;
}
