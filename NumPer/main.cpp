#include <iostream>
#include <vector>
#include <algorithm>
#include "../Common/strprocess.hpp"

std::vector<int> get_column_order(const std::string &key_trans) {
	int len = key_trans.size();
	std::vector<std::pair<char, int>> chars;
	std::vector<int> order(len);

	for (int i = 0; i < len; ++i) {
		chars.emplace_back(key_trans[i], i);
	}

	std::sort(chars.begin(), chars.end());

	for (int i = 0; i < len; ++i) {
		order[chars[i].second] = i+1;
	}

	return order;
}

void numerical_permutation(const std::string &word) {
	auto vec = get_column_order(word);

	for(auto i : vec) {
		std::cout << i << " ";
	}
	std::cout << std::endl;
}

int main(int argc, char **argv) {
	if(argc < 2) {
		std::cout << "Usage: " << argv[0] << " [word]\n";
		return 1;
	}

	std::string word = remove_delims(argv[1]);
	numerical_permutation(word);

	return 0;
}