#include <iostream>
#include <set>
#include <unordered_set>
#include <vector>
#include "../Common/strprocess.hpp"

constexpr size_t TABLE_SIZE = 5;

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

std::vector<std::vector<char>> get_polybius_table(const std::string &key_table) {
	std::string key = key_table;
	std::vector<std::vector<char>> ret(TABLE_SIZE, std::vector<char>(TABLE_SIZE));
	std::unordered_set<char> already_added;
	std::set<char> alph = {'A', 'B', 'C', 'D', 'E','F','G','H','I',
			'K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z'};

	std::string poly;

	for(char c : key_table) {
		if(!already_added.contains(c)) {
			alph.erase(c);
			poly += c;
			already_added.emplace(c);
		}
	}
	while(alph.size() > 0){
		char c = *alph.begin();
		alph.erase(c);
		poly += c;
	}

	for(int i = 0; i < TABLE_SIZE; i++) {
		for(int j = 0; j < TABLE_SIZE; j++) {
			ret[i][j] = poly[i * TABLE_SIZE + j];
			std::cout << ret[i][j] << " ";
		}
		std::cout << std::endl;
	}
	
	return ret;
}

std::pair<int, int> find_position(const std::vector<std::vector<char>>& table, char ch) {
	if (ch == 'J') ch = 'I'; // Treat 'J' as 'I'
	for (int i = 0; i < TABLE_SIZE; ++i) {
		for (int j = 0; j < TABLE_SIZE; ++j) {
			if (table[i][j] == ch) return {i, j};
		}
	}
	char chs[2]{ch, '\0'};
	std::cout << "Char code: " << (int)ch << std::endl;

	throw std::runtime_error((std::string)"Character not found in key table: \"" + chs + "\"");
}

void encrypt(std::string &msg, const std::string &key) {
	auto table = get_polybius_table(key);
	std::string encmsg;

	for (size_t i = 0; i < msg.size(); i += 2) {
		char first = msg[i];
		char second = (i + 1 < msg.size()) ? msg[i + 1] : 'X';

		auto pos1 = find_position(table, first);
		auto pos2 = find_position(table, second);

		if (pos1.first == pos2.first) { // Same row
			encmsg += table[pos1.first][(pos1.second + 1) % TABLE_SIZE];
			encmsg += table[pos2.first][(pos2.second + 1) % TABLE_SIZE];
		} else if (pos1.second == pos2.second) { // Same column
			encmsg += table[(pos1.first + 1) % TABLE_SIZE][pos1.second];
			encmsg += table[(pos2.first + 1) % TABLE_SIZE][pos2.second];
		} else { // Rectangle
			encmsg += table[pos1.first][pos2.second];
			encmsg += table[pos2.first][pos1.second];
		}
	}

	std::cout << postprocess_msg(encmsg);
}

void decrypt(std::string &msg, const std::string &key) {
	auto table = get_polybius_table(key);
	std::string decrypted;

	for (size_t i = 0; i < msg.size(); i += 2) {
		char first = msg[i];
		char second = msg[i + 1];

		auto pos1 = find_position(table, first);
		auto pos2 = find_position(table, second);

		if (pos1.first == pos2.first) { // Same row
			decrypted += table[pos1.first][(pos1.second + TABLE_SIZE - 1) % TABLE_SIZE];
			decrypted += table[pos2.first][(pos2.second + TABLE_SIZE - 1) % TABLE_SIZE];
		} else if (pos1.second == pos2.second) { // Same column
			decrypted += table[(pos1.first + TABLE_SIZE - 1) % TABLE_SIZE][pos1.second];
			decrypted += table[(pos2.first + TABLE_SIZE - 1) % TABLE_SIZE][pos2.second];
		} else { // Rectangle
			decrypted += table[pos1.first][pos2.second];
			decrypted += table[pos2.first][pos1.second];
		}
	}

	std::cout << postprocess_msg(decrypted);
}

int main(int argc, char ** argv) {
	if(argc < 4) {
		std::cout << "Usage: " << argv[0] << " [-d|-e] [msg] [key]\n";
		return 1;
	}

	mode mode = get_mode(argv[1]);
	std::string msg = remove_delims(argv[2]);
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