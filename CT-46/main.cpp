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

std::pair<std::unordered_map<std::pair<int, int>, char>, std::vector<int>> get_polybius_table(const std::string &key_table, const bool begin_zero) {
	if(key_table.size() < 10) throw std::runtime_error("Key too short (Needs to be at least 10 chars)");
	std::string key = key_table.substr(0,10);
	std::unordered_map<std::pair<int, int>, char> ret;
	std::vector<char> alph = 
			{'A','E','I','N','O','R', 
			'B','C','D','F','G','H','J','K','L','M',
			'P','Q','S','T','U','V','W','X','Y','Z',
			' ','.',',',':','?','/','(',')','"',CODE,
			'0','1','2','3','4','5','6','7','8','9'};

	auto column_order = get_column_order(key);
	std::vector<int> orphaned_prefixes;
	orphaned_prefixes.assign(column_order.begin(), column_order.end() - 4);
	std::cout << "  ";
	for(size_t i = 0; i < column_order.size(); i++) {
		std::cout << column_order[i] << " ";
	}
	std::cout << std::endl;
	std::cout << "? ";
	for(int i = 0; i < 6; i++) {
		char c = *alph.begin();
		alph.erase(alph.begin());
		int x = column_order[i];
		ret[std::make_pair(std::numeric_limits<int>::max(), x)] = c;
		std::cout << c << " ";
	}
	std::cout << std::endl;

	for(size_t i = 6; i < column_order.size(); i++) {
		std::cout << column_order[i] << " ";
		for(size_t j = 0; j < 10; j++) {
			char c = *alph.begin();
			alph.erase(alph.begin());
			int x = column_order[i], y = column_order[j];
			if(begin_zero) { y--; if(y < 0) y = 9;}
			ret[std::make_pair(x, y)] = c;
			std::cout << c << " ";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
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

void encrypt(std::string &msg, const std::string &key, bool begin_zero) {
	auto [table, orphaned] = get_polybius_table(key, begin_zero);
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

void decrypt(std::string &msg, const std::string &key, bool begin_zero) {
	auto [table, orphaned] = get_polybius_table(key, begin_zero);
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

	std::cout << decmsg;
}

int main(int argc, char ** argv) {
	if(argc < 4) {
		std::cout << "Usage: " << argv[0] << " [-d|-e] [msg] [key] <--begin-zero>\n";
		return 1;
	}

	mode mode = get_mode(argv[1]);
	std::string msg = mode == DECRYPT ? remove_delims(argv[2], true) : ct46_delims(argv[2]);
	std::string key = remove_delims(argv[3]);
	bool begin_zero = argc == 5 ? ((std::string)argv[4] == "--begin-zero") : (false);
	switch(mode) {
		case ENCRYPT:
			encrypt(msg, key, begin_zero);
			break;
		case DECRYPT:
			decrypt(msg, key, begin_zero);
			break;
	}

	std::cout << std::endl;
	return 0;
}