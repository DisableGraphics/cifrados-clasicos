#include <iostream>
#include <unordered_map>
#include <algorithm>
#include <vector>
#include <limits>
#include "../Common/strprocess.hpp"

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

template <>
struct std::hash<std::pair<int, int>> {
	std::size_t operator()(const std::pair<int, int>& k) const {
		return ((std::size_t)k.first << 32) | k.second;
	}
};

std::vector<int> get_column_order(const std::string &key_trans) {
	int len = key_trans.size();
	std::vector<std::pair<char, int>> chars;
	std::vector<int> order(len);

	for (int i = 0; i < len; ++i) {
		chars.emplace_back(key_trans[i], i);
	}

	std::sort(chars.begin(), chars.end());

	for (int i = 0; i < len; ++i) {
		order[chars[i].second] = (i + 1) %10;
	}

	return order;
}

std::pair<std::unordered_map<std::pair<int, int>, char>, std::vector<int>> get_polybius_table(const std::string &key_table) {
	if(key_table.size() < 10) throw std::runtime_error("Key too short (Needs to be at least 10 chars)");
	std::string key = key_table.substr(0,10);
	std::unordered_map<std::pair<int, int>, char> ret;
	std::vector<char> alph = 
	{
		'A','T','O','N','E','S','I','R',
		'B','C','D','F','G','H','J','K','L','M',
		'P','Q','U','V','W','X','Y','Z','.','#'
	};

	auto column_order = get_column_order(key);
	std::vector<int> orphaned_prefixes;
	for(int i = 0; i < 10; i++) {
		if(i != 2 && i != 6) {
			char c = *alph.begin();
			alph.erase(alph.begin());
			int x = column_order[i];
			ret[std::make_pair(std::numeric_limits<int>::max(), x)] = c;
			orphaned_prefixes.emplace_back(column_order[i]);
			std::cout << c << " (" << '-' << ',' << x << ") ";
		} else {
			std::cout << "- (-,-) ";
		}
	}
	std::cout << std::endl;

	for(size_t i = 1; i <= 2; i++) {
		for(size_t j = 0; j < 10; j++) {
			char c = *alph.begin();
			alph.erase(alph.begin());
			int col;
			if(i == 1) {
				col = 2;
			} else {
				col = 6;
			}
			int x = column_order[col], y = column_order[j];
			ret[std::make_pair(x, y)] = c;
			std::cout << c << " (" << x << ',' << y << ") ";
		}
		std::cout << std::endl;
	}
	return {ret, orphaned_prefixes};
}

std::pair<int, int> get_position_in_table(const std::unordered_map<std::pair<int, int>, char> &table, char c) {
	std::string ret;
	for(auto &elem : table) {
		if(elem.second == c) {
			return elem.first;
		}
	}
	throw std::runtime_error((std::string)"character \"" + c + "\" doesn't exist in table");
}

void encrypt(std::string &msg, const std::string &key) {
	auto [table, orphaned] = get_polybius_table(key);
	std::string encmsg;

	for (size_t i = 0; i < msg.size(); i++) {
		char first = msg[i];
		auto pos = get_position_in_table(table, first);
		int x = pos.first, y = pos.second;
		encmsg += (x == std::numeric_limits<int>::max() ? "" : std::to_string(x))
			+ std::to_string(y);
		
	}

	std::cout << postprocess_msg(encmsg);
}

bool is_orphaned_digit(const int digit, const std::vector<int> &orphaneds) {
	for(auto &elem : orphaneds) {
		if(elem == digit) return true;
	}
	return false;
}

void decrypt(std::string &msg, const std::string &key) {
	auto [table, orphaned] = get_polybius_table(key);
	std::string decmsg;

	for (size_t i = 0; i < msg.size(); i++) {
		char first = msg[i], second = '0';
		int valx = first - '0', valy = 0;
		char obtained;
		if(is_orphaned_digit(valx, orphaned)) {
			obtained = table[std::make_pair(std::numeric_limits<int>::max(), valx)];
		} else {
			second = msg[i + 1];
			valy = second - '0';
			obtained = table[std::make_pair(valx, valy)];
			i++;
		}
		decmsg += obtained;
	}

	std::cout << postprocess_msg(decmsg);
}

int main(int argc, char ** argv) {
	if(argc < 4) {
		std::cout << "Usage: " << argv[0] << " [-d|-e] [msg] [key]\n";
		return 1;
	}

	mode mode = get_mode(argv[1]);
	std::string msg = mode == DECRYPT ? remove_delims(argv[2], true) : strad_delims(argv[2]);
	std::string key = remove_delims(argv[3]);
	switch(mode) {
		case ENCRYPT:
			encrypt(msg, key);
			break;
		case DECRYPT:
			decrypt(msg, key);
			break;
	}

	std::cout << std::endl;
	return 0;
}