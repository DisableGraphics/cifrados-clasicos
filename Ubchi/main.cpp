#include <iostream>
#include <string>
#include <regex>
#include "../Common/transpose.hpp"
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

void encrypt(const std::string &msg, const std::string &key1, const std::string &key2) {
	std::cout << postprocess_msg(transpose(transpose(msg, key1), key2));
}

void decrypt(const std::string &msg, const std::string &key1, const std::string &key2) {
	std::cout << postprocess_msg(inverse_transpose(inverse_transpose(msg, key2), key1));
}

int main(int argc, char ** argv) {
	if(argc < 5) {
		std::cout << "Usage: " << argv[0] << " [-d|-e] [msg] [key1] [key2]\n";
		return 1;
	}
	mode mode = get_mode(argv[1]);
	std::string msg = remove_delims(argv[2]);
	std::string key1 = std::regex_replace(std::string(argv[3]), std::regex(" "), "");
	std::string key2 = std::regex_replace(std::string(argv[4]), std::regex(" "), "");

	switch(mode) {
		case ENCRYPT:
			encrypt(msg, key1, key2);
			break;
		case DECRYPT:
			decrypt(msg, key1, key2);
			break;
	}

	std::cout << std::endl;

	return 0;
}