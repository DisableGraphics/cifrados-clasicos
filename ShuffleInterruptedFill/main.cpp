#include <iostream>
#include <vector>
#include <algorithm>
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

std::vector<int> get_column_order_fill(const std::string &key_trans) {
	int len = key_trans.size();
	std::vector<std::pair<char, int>> chars;
	std::vector<int> order(len);

	for (int i = 0; i < len; ++i) {
		chars.emplace_back(key_trans[i], i);
	}

	std::sort(chars.begin(), chars.end());

	for (int i = 0; i < len; ++i) {
		order[i] = chars[i].second;
	}

	return order;
}

std::string transposemsg(const std::string &msg, const std::string &key_trans) {
	std::vector<std::vector<char>> grid;
	std::vector<int> column_order = get_column_order_fill(key_trans);
	size_t msg_index = 0;

	for (size_t i = 0; msg_index < (msg.size()/2); ++i) {
		int stop_position = column_order[i % column_order.size()] + 1;
		std::vector<char> row;

		for (int col = 0; col < stop_position && msg_index < msg.size(); ++col) {
			row.push_back(msg[msg_index++]);
			if(msg_index >= msg.size()/2) {
				break;
			}
		}
		for(int col = row.size(); col < key_trans.size(); col++) {
			row.push_back('_');
		}
		grid.push_back(row);
	}

	if(grid.back()[0] != '_') {
		auto &row = grid.back();
		size_t lastcharpos = 0;
		for(auto &c : row) if(c != '_') { lastcharpos++; c = '_'; } else break;
		msg_index -= lastcharpos;
	}

	for(auto &row : grid) {
		for(auto &c : row) {
			if(msg_index >= msg.size()) break;
			std::cout << c;
			if(c == '_')
				c = msg[msg_index++];
			
		}
		std::cout << std::endl;
		if(msg_index >= msg.size()) break;
	}


	std::string ret;
	for (int col : column_order) {
		for (const auto& row : grid) {
			if (col < row.size()) {
				if(row[col] != '_')
					ret += row[col];
			}
		}
	}
	return ret;
}

void encrypt(const std::string &msg, const std::string &key) {
	std::cout << postprocess_msg(transposemsg(msg, key));
}

void decrypt(const std::string &msg, const std::string &key) {
	std::cout << postprocess_msg(inverse_transpose(msg, key));
}

int main(int argc, char ** argv) {
	if(argc < 4) {
		std::cout << "Usage: " << argv[0] << " [-d|-e] [msg] [key]\n";
		return 1;
	}

	mode mode = get_mode(argv[1]);
	std::string msg = remove_delims(argv[2], true);
	std::string key = remove_delims(argv[3]);

	switch(mode) {
		case DECRYPT:
			decrypt(msg, key);
			break;
		case ENCRYPT:
			encrypt(msg, key);
			break;
	}
	std::cout << std::endl;

	return 0;
}