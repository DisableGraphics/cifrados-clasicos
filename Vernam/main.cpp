#include <iostream>
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

void encrypt(const std::string &msg, const std::string &key, bool otp) {
	std::string encmsg;
	for(size_t i = 0; i < key.size(); i++) {
		int valmsg = msg[i] - '0';
		int valkey = key[i] - '0';
		int xor_digit;
		if(otp) {
			xor_digit = ((valmsg - valkey) + 10) % 10;
		} else {
			xor_digit = ((valmsg ^ valkey));
		}
		encmsg += xor_digit + '0';
	}
	std::cout << "  " << postprocess_msg(msg) << std::endl;
	std::cout << (otp ? '-': '^') << " " << postprocess_msg(key) << std::endl;
	std::cout << "  " << postprocess_msg(encmsg) << std::endl;
}

void decrypt(const std::string &msg, const std::string &key, bool otp) {
	std::string encmsg;
	for(size_t i = 0; i < key.size(); i++) {
		int valmsg = msg[i] - '0';
		int valkey = key[i] - '0';
		int xor_digit;
		if(otp) {
			xor_digit = (valmsg + valkey) % 10;
		} else {
			xor_digit = (valmsg ^ valkey);
		}
		encmsg += xor_digit + '0';
	}
	std::cout << "  " << postprocess_msg(msg) << std::endl;
	std::cout << (otp ? '+': '^') << " " << postprocess_msg(key) << std::endl;
	std::cout << "  " << postprocess_msg(encmsg) << std::endl;
}

int main(int argc, char ** argv) {
	if(argc < 4) {
		std::cout << "Usage: " << argv[0] << " [-d|-e] [msg] [key] <--otp>\n";
		return 1;
	}
	mode mode = get_mode(argv[1]);
	std::string msg = mode == ENCRYPT ? numbers_only(argv[2]) : remove_spaces(argv[2]);
	std::string key = numbers_only(argv[3]);
	bool otp = false;
	if(argc == 5 && (std::string)argv[4] == "--otp") otp = true;

	switch(mode) {
		case ENCRYPT:
			encrypt(msg, key, otp);
			break;
		case DECRYPT:
			decrypt(msg, key, otp);
			break;
	}

	std::cout << std::endl;

	return 0;
}