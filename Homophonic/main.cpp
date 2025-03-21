#include <cstddef>
#include <cstdio>
#include <iostream>
#include <algorithm>
#include <set>
#include <map>
#include <vector>
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

std::map<char, std::vector<size_t>> get_key_map(const std::string &key) {
	std::set<char> not_added;
	std::map<char, std::vector<size_t>> ret;
	for(char i = 'A'; i <= 'Z'; i++)
		not_added.emplace(i);
	size_t i = 0;
	for(; i < key.size(); i++) {
		char c = key[i];
		not_added.erase(c);
		ret[c].push_back(i+1);
	}

	for(; not_added.size() > 0; i++) {
		char c = *not_added.begin();
		not_added.erase(c);
		ret[c].push_back(i+1);
	}

	for(auto &elem : ret) {
		std::cout << elem.first << ": ";
		for(long code : elem.second) {
			std::cout << code << " ";
		}
		std::cout << '\b' << std::endl;
	}
	std::cout << std::endl;

	return ret;
}

void encrypt(const std::string &msg, const std::string &key) {
	std::string encmsg;
	auto keym = get_key_map(key);
	std::map<char, size_t> posmap;
	for(size_t i = 0; i < msg.size(); i++) {
		char c = msg[i];
		int pos = posmap[c];
		posmap[c] = posmap[c] % keym[c].size();
		encmsg += std::to_string(keym[c][pos]) + ",";
	}

	if(encmsg.size() > 0) encmsg.pop_back();

	std::cout << encmsg;
}

char get_char(const std::map<char, std::vector<size_t>> &keym, int val) {
	for(auto &elem : keym) {
		if(std::find(elem.second.begin(), elem.second.end(), val) != elem.second.end()) {
			return elem.first;
		}
	}
	return '\0';
}

void decrypt(std::string msg, const std::string &key) {
	std::string decmsg;
	auto keym = get_key_map(key);

	while(msg.size() > 0) {
		std::string token = msg.substr(0, msg.find(' '));
		msg = msg.erase(0, token.size() + 1);
		int val = std::stoi(token);
		decmsg += get_char(keym, val);
	}

	std::cout << postprocess_msg(decmsg);
}

int main(int argc, char ** argv) {
	if(argc < 4) {
		std::cout << "Usage: " << argv[0] << " [-d|-e] [msg] [key]\n";
		return 1;
	}

	mode mode = get_mode(argv[1]);
	std::string msg = remove_delims(argv[2], true, mode == ENCRYPT ? false : true);
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