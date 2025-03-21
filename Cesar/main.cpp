#include <iostream>
#include <string>
#include "../Common/strprocess.hpp"

enum mode {
	BRUTE_FORCE,
	ENCRYPT,
	DECRYPT
};

mode get_mode(const std::string &modestr) {
	if(modestr == "-b")
		return BRUTE_FORCE;
	else if(modestr == "-e")
		return ENCRYPT;
	else if(modestr == "-d")
		return DECRYPT;
	throw std::runtime_error("Illegal mode: " + modestr);
}

void brute_force(const std::string &msg) {
	for(int i = 0; i < 26; i++) {
		std::string posdecmsg;
		for(size_t j = 0; j < msg.size(); j++) {
			char c = ((msg[j] - 'A') + i) % 26 + 'A';
			posdecmsg += c;
		}
		std::cout << "Key: " << i << ": " << postprocess_msg(posdecmsg) << std::endl << std::endl;
	}
}

void encrypt(const std::string &msg, int key) {
	std::string enc;
	for(size_t j = 0; j < msg.size(); j++) {
		if(msg[j] >= 'A' && msg[j] <= 'Z') {
			char c = ((msg[j] - 'A') + key) % 26 + 'A';
			enc += c;
		}
	}
	std::cout << postprocess_msg(enc);
}

void decrypt(const std::string &msg, int key) {
	std::string dec;
	for(size_t j = 0; j < msg.size(); j++) {
		if(msg[j] >= 'A' && msg[j] <= 'Z') {
			char c = ((msg[j] - 'A') - key + 26) % 26 + 'A';
			dec += c;
		} 
	}
	std::cout << postprocess_msg(dec);
}

int main(int argc, char ** argv) {
	if(argc < 4) {
		std::cout << "Usage: " << argv[0] << " [-b|-e|-d] [msg] [key]\n";
		return 1;
	}
	mode mode = get_mode(argv[1]);
	std::string msg = remove_delims(argv[2]);
	int key = std::stoi(remove_delims(argv[3], true));
	
	switch(mode) {
		case BRUTE_FORCE:
			brute_force(msg);
			break;
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