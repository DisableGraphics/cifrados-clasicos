#include <iostream>
#include <vector>
#include <limits>
#include "../Common/strprocess.hpp"

enum mode {
	ENCRYPT,
	DECRYPT
};

enum cipher_mode {
	TEXT_AUTOKEY,
	KEY_AUTOKEY
};

mode get_mode(const std::string &modestr) {
	if(modestr == "-e")
		return ENCRYPT;
	else if(modestr == "-d")
		return DECRYPT;
	throw std::runtime_error("Illegal mode: " + modestr);
}

cipher_mode get_cipher_mode(const std::string &modestr) {
	if(modestr == "t")
		return TEXT_AUTOKEY;
	else if(modestr == "k")
		return KEY_AUTOKEY;
	throw std::runtime_error("Illegal mode: " + modestr);
}

std::vector<std::vector<char>> gen_alphs(const std::string &key, bool to_right) {
	std::vector<std::vector<char>> ret;
	for(char c : key) {
		if(c >= 'A' && c <= 'Z') { 
			int ndisp = c - 'A';
			ret.push_back({});

			for(int i = 0; i < 26; i++) {
				int newval;
				if(!to_right) {
					newval = (i - ndisp + 26) % 26;
				} else {
					newval = (i + ndisp) % 26;
				}
				ret.back().push_back(newval + 'A');
			}
		}
	}
	return ret;
}

std::vector<char> gen_alph(char c, bool to_right) {
	std::vector<char> ret;
	int ndisp = c - 'A';
	for(int i = 0; i < 26; i++) {
		int newval;
		if(!to_right) {
			newval = (i - ndisp + 26) % 26;
		} else {
			newval = (i + ndisp) % 26;
		}
		ret.push_back(newval + 'A');
	}
	return ret;
}

void printalphabets(bool to_right, const std::vector<std::vector<char>> &alph) {
	for(auto &c : gen_alph('A', to_right)) {
		std::cout << c << " ";
	}
	std::cout << std::endl;
	for(size_t i = 0; i < alph.back().size(); i++) {
		std::cout << "--";
	}
	std::cout << std::endl;
	for(auto &alphs : alph) {
		for(char c : alphs) std::cout << c << " ";
		std::cout << std::endl;
	}
}

void encrypt(const std::string &msg, const std::string &key, bool to_right, cipher_mode cm) {
	auto alph = gen_alphs(key, to_right);
	size_t count = 0;
	std::string encmsg;
	for(size_t i = 0; i < msg.size(); i++) {
		char c = msg[i];

		if(count >= key.size()) {
			if(cm == KEY_AUTOKEY) {
				count = 0;
			} else {
				alph.push_back({});
				char k = msg[i - key.size()];
				if(k > 'Z' || k < 'A') k = i - key.size() - 1;
				alph.emplace_back(gen_alph(k, to_right));
			}
		}
		int val = c - 'A';
		
		encmsg += alph[count][val];
		count++;
	}
	printalphabets(to_right, alph);
	std::cout << std::endl;

	std::cout << postprocess_msg(encmsg);
}

size_t get_pos(const std::vector<char> &alph, char c) {
	for(size_t i = 0; i < alph.size(); i++) {
		if(alph[i] == c) return i;
	}

	return std::numeric_limits<size_t>::max();
}

void decrypt(const std::string &msg, const std::string &key, bool to_right, cipher_mode cm) {
	auto alph = gen_alphs(key, to_right);
	size_t count = 0;
	std::string clearmsg;
	std::string finmsg;
	for(size_t i = 0; i < msg.size(); i++) {
		char c = msg[i];
		if(c >= 'A' && c <= 'Z') {
			if(count >= key.size()) {
				if(cm == KEY_AUTOKEY) {
					count = 0;
				} else {
					alph.push_back({});
					char k = clearmsg[i - key.size()];
					if(k > 'Z' || k < 'A') k = i - key.size() - 1;
					alph.emplace_back(gen_alph(k, to_right));
				}
			}
			size_t val = get_pos(alph[count], c);
			char newc = (char)(val + 'A');
			finmsg += newc;
			clearmsg += newc;
			count++;
		}
	}
	printalphabets(to_right, alph);
	std::cout << std::endl;

	std::cout << postprocess_msg(finmsg);
}

int main(int argc, char ** argv) {
	if(argc < 4) {
		std::cout << "Usage: " << argv[0] << " [-d|-e] [msg] [key] <Direction: default right [d|i]> <Cipher mode: default: key-autokey [t|k]>\n";
		return 1;
	}
	mode mode = get_mode(argv[1]);
	std::string msg = remove_delims(argv[2]);
	std::string key = remove_delims(argv[3]);
	bool to_right = true;
	cipher_mode cm = KEY_AUTOKEY;
	if(argc >= 5 && std::string(argv[4]) == "i") to_right = false;
	if(argc >= 6) cm = get_cipher_mode(argv[5]);

	switch(mode) {
		case ENCRYPT:
			encrypt(msg, key, to_right, cm);
			break;
		case DECRYPT:
			decrypt(msg, key, to_right, cm);
			break;
	}
	std::cout << std::endl;

	return 0;
}