#include <iostream>
#include <vector>
#include <limits>
#include "../Common/strprocess.hpp"
#include "../Common/transpose.hpp"

enum mode {
	ENCRYPT,
	DECRYPT
};

mode get_mode(const std::string &modestr) {
	if(modestr == "-e")
		return ENCRYPT;
	else if(modestr == "-d")
		return DECRYPT;
	throw std::runtime_error("Illegal mode: " + modestr);
}

void printkey(const std::string &key_trans, const std::vector<int> &human_readable) {
	for(char c : key_trans) {
		std::cout << " " << c;
	}
	std::cout << std::endl;
	for(auto &ko : human_readable) {
		printf("%2d", ko+1);
	}
	std::cout << std::endl;
	for(char _ : key_trans) {
		std::cout << "__";
	}
	std::cout << std::endl;
}

std::string transpose_int(const std::string &msg, const std::string &key_trans) {
	std::vector<std::vector<char>> grid;
	auto [column_order, human_readable] = get_column_order(key_trans);
	int msg_index = 0;

	printkey(key_trans, human_readable);

	for (size_t i = 0; msg_index < msg.size(); ++i) {
		int stop_position = column_order[i % column_order.size()] + 1;
		std::vector<char> row;

		for (int col = 0; col < stop_position && msg_index < msg.size(); ++col) {
			row.push_back(msg[msg_index++]);
			std::cout << " " << msg[msg_index-1];
		}
		for(int col = stop_position; col < key_trans.size(); col++) {
			std::cout << "--";
		}
		std::cout << std::endl;
		grid.push_back(row);
	}

	std::string ret;
	for (int col : column_order) {
		for (const auto& row : grid) {
			if (col < row.size()) {
				ret += row[col];
			}
		}
	}
	return ret;
}

std::string inverse_transpose_int(const std::string &msg, const std::string &key_trans) {
	auto [column_order, human_readable] = get_column_order(key_trans);
	std::vector<std::vector<char>> grid;
	std::vector<std::vector<size_t>> positions;

	printkey(key_trans, human_readable);
	
	size_t msg_index = 0;
	std::vector<int> row_sizes;
	size_t counter = 0;
	while (msg_index < msg.size()) {
		int stop_position = column_order[grid.size() % column_order.size()] + 1;
		row_sizes.push_back(stop_position);
		grid.push_back(std::vector<char>(stop_position, '_'));
		positions.push_back(std::vector<size_t>(stop_position, std::numeric_limits<size_t>::max()));
		for(size_t i = 0; i < stop_position; i++) {
			positions.back()[i] = counter++;
		}
		msg_index += stop_position;
	}

	msg_index = 0;
	for (int col : column_order) {
		for (size_t row = 0; row < grid.size() && msg_index < msg.size(); ++row) {
			if (col < grid[row].size()) {
				if(positions[row][col] < msg.size()) {
					grid[row][col] = msg[msg_index++];
				}
			}
		}
	}

	std::string ret;
	for (size_t i = 0; i < grid.size(); i++) {
		for (size_t j = 0; j < grid[i].size(); j++) {
			if (grid[i][j] != '_') {
				ret += grid[i][j];
			}
			std::cout << " " << grid[i][j];
		}
		for(size_t j = grid[i].size(); j < key_trans.size(); j++) {
			std::cout << "--";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
	return ret;
}

void encrypt(const std::string &msg, const std::string &key_trans) {
	std::cout << postprocess_msg(transpose_int(msg, key_trans));
}

void decrypt(const std::string &msg, const std::string &key_trans) {
	std::cout << postprocess_msg(inverse_transpose_int(msg, key_trans));
}

int main(int argc, char **argv) {
	if (argc < 4) {
		std::cout << "Usage: " << argv[0] << " [-d|-e] [msg] [key_trans]\n";
		return 1;
	}

	mode mode = get_mode(argv[1]);
	std::string msg = remove_delims(argv[2], true);
	std::string key_trans = remove_delims(argv[3]);

	switch (mode) {
		case DECRYPT:
			decrypt(msg, key_trans);
			break;
		case ENCRYPT:
			encrypt(msg, key_trans);
			break;
	}
	std::cout << std::endl;

	return 0;
}
