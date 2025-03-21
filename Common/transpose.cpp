#include "transpose.hpp"
#include <vector>
#include <algorithm>
#include <iostream>

std::pair<std::vector<int>, std::vector<int>> get_column_order(const std::string &key_trans) {
	int len = key_trans.size();
	std::vector<std::pair<char, int>> chars;
	std::vector<int> order(len);
	std::vector<int> human_readable_order(len);

	for (int i = 0; i < len; ++i) {
		chars.emplace_back(key_trans[i], i);
	}

	std::sort(chars.begin(), chars.end());

	for (int i = 0; i < len; ++i) {
		order[i] = chars[i].second;
		human_readable_order[chars[i].second] = i;
	}

	return {order, human_readable_order};
}

void print_everything(const std::vector<int> &human_readable, const std::string &key_trans) {
	for(size_t i = 0; i < key_trans.size(); i++) {
		std::cout << " " << key_trans[i];
	}
	std::cout << std::endl;

	for(size_t i = 0; i < human_readable.size(); i++) {
		printf("%2d", human_readable[i]+1);
	}
	std::cout << std::endl;

	for(size_t i = 0; i < key_trans.size(); i++) {
		std::cout << "__";
	}
	std::cout << std::endl;
}

std::string transpose(const std::string &msg, const std::string &key_trans) {
	std::vector<std::vector<char>> grid;
	auto [column_order, human_readable] = get_column_order(key_trans);
	print_everything(human_readable, key_trans);
	for(size_t i = 0; i < msg.size(); i++) {
		char c = msg[i];
		if(i % key_trans.size() == 0) {
			if(i != 0)
				std::cout << std::endl;
			grid.push_back({c});
		} else {
			grid.back().push_back(c);
		}
		std::cout << " " << c;
	}
	std::string ret;
	for(auto &i : column_order) {
		for(size_t j = 0; j < grid.size(); j++) {
			try {
				ret += grid[j].at(i);
			} catch(...){}
		}
	}
	std::cout << std::endl << std::endl;
	return ret;
}

std::string inverse_transpose(const std::string &msg, const std::string &key_trans) {
	constexpr char blank = '_';
	size_t columns = key_trans.size();
	size_t rows = (msg.size() / columns) + 1;
	std::vector<std::vector<char>> grid(rows, std::vector<char>(columns, blank));
	size_t max_column_over = msg.size() % key_trans.size();

	auto [column_order, human_readable] = get_column_order(key_trans);

	print_everything(human_readable, key_trans);	

	size_t count = 0;
	for(size_t i : column_order) {
		int nmax = i >= max_column_over ? grid.size() - 1 : grid.size();
		for(int j = 0; j < nmax; j++) {
			grid[j].at(i) = msg[count];
			count++;
		}
		
	}
	std::string ret;

	for(size_t i = 0; i < grid.size(); i++) {
		for(size_t j = 0; j < grid[i].size(); j++) {
			if(grid[i][j] != blank) {
				ret += grid[i][j];
				std::cout << " " << ret.back();
			}
		}
		std::cout << std::endl;
	}

	return ret;
}