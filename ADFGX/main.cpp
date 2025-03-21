#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <set>
#include <utility>
#include "../Common/transpose.hpp"
#include "../Common/strprocess.hpp"

const static std::string order_v = "ADFGVX", order = "ADFGX";

template <>
struct std::hash<std::pair<char, char>> {
	std::size_t operator()(const std::pair<char, char>& k) const {
		return ((std::size_t)k.first << 8) | k.second;
	}
};

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

std::unordered_map<std::pair<char, char>, char> get_polybius_table(const std::string &key_table, bool is_adfgvx) {
	std::string key = key_table;
	std::unordered_map<std::pair<char, char>, char> ret;
	std::unordered_set<char> already_added;
	std::set<char> alph = {'A', 'B', 'C', 'D', 'E','F','G','H','I','J',
			'K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z'};

	int size = is_adfgvx ? 6 : 5;
	if(!is_adfgvx) { 
		alph.erase('Y');
	}
	const std::string *org = is_adfgvx ? &order_v : &order;

	std::string poly;
	std::cout << "  ";
	for(size_t i = 0; i < org->size(); i++) {
		std::cout << org->at(i) << " ";
	}

	std::cout << std::endl;

	for(char c : key_table) {
		if(!already_added.contains(c)) {
			alph.erase(c);
			poly += c;
			if(is_adfgvx && c <= 'J' && c >= 'A') {
				poly += ((c - 'A' + 1) % 10) + '0';
			}
			already_added.emplace(c);
		}
	}
	while(alph.size() > 0){
		char c = *alph.begin();
		alph.erase(c);
		poly += c;
		if(is_adfgvx && c <= 'J' && c >= 'A') {
			poly += ((c - 'A' + 1) % 10) + '0';
		}
	}

	for(int i = 0; i < size; i++) {
		std::cout << org->at(i) << " ";
		for(int j = 0; j < size; j++) {
			char x = org->at(i), y = org->at(j);
			char c = poly[i*size + j];
			ret[std::make_pair(x, y)] = c;
			std::cout << c << " ";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
	return ret;
}

std::string get_position_in_table(const std::unordered_map<std::pair<char, char>, char> &table, char c) {
	std::string ret;
	for(auto &elem : table) {
		if(elem.second == c) {
			ret += elem.first.first;
			ret += elem.first.second;
			return ret;
		}
	}
	return ret;
}

void encrypt(const std::string &msg, const std::string &key_table, const std::string &key_trans, bool is_adfgvx) {
	auto table = get_polybius_table(key_table, is_adfgvx);
	std::string encmsg;
	for(auto &c : msg) {
		encmsg += get_position_in_table(table, c);
	}
	encmsg = transpose(encmsg, key_trans);
	std::cout << postprocess_msg(encmsg);
}

void decrypt(const std::string &msg, const std::string &key_table, const std::string &key_trans, bool is_adfgvx) {
	auto table = get_polybius_table(key_table, is_adfgvx);
	std::string tr = inverse_transpose(msg, key_trans);
	std::string decmsg;
	for(size_t i = 0; i < tr.size(); i += 2) {
		decmsg += table[std::make_pair(tr[i], tr[i+1])];
	}
	std::cout << postprocess_msg(decmsg);
}

int main(int argc, char **argv) {
	if(argc < 5) {
		std::cout << "Usage: " << argv[0] << " [-d|-e] [msg] [key-table] [key-transposition] <ADFGVX [-a] (default ADFGX)>\n";
		return 1;
	}
	bool is_adfgvx = false;
	if(argc == 6 && (std::string)argv[5] == "-a") is_adfgvx = true;

	mode mode = get_mode(argv[1]);
	std::string msg = remove_delims(argv[2], is_adfgvx);
	std::string key_table = remove_delims(argv[3]);
	std::string key_trans = remove_delims(argv[4]);
	
	switch(mode) {
		case ENCRYPT:
			encrypt(msg, key_table, key_trans, is_adfgvx);
			break;
		case DECRYPT:
			decrypt(msg, key_table, key_trans, is_adfgvx);
			break;
	}

	std::cout << std::endl;
	return 0;
}