#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include "../Common/strprocess.hpp"

const static std::string first_half = "ABCDEFGHIJKLM";
const static std::string secon_half = "NOPQRSTUVWXYZ";


std::vector<std::vector<char>> gen_alphs(const std::string &key, bool to_right) {
	std::vector<std::vector<char>> ret;
	for(char c : key) {
		if(c >= 'A' && c <= 'Z') { 
			int ndisp = (c - 'A') / 2;
			ret.push_back({});

			for(int i = 0; i < 13; i++) {
				int newval;
				if(to_right) {
					newval = (i - ndisp + 13) % 13;
				} else {
					newval = (i + ndisp) % 13;
				}
				ret.back().push_back(secon_half[newval]);
			}
			for(char c = 'A'; c <= 'M'; c++) {
				std::cout << c;
			}
			std::cout << std::endl;
			for(auto c : ret.back()) {
				std::cout << c;
			}
			std::cout << std::endl << std::endl;
		}
	}
	return ret;
}

size_t get_pos(const std::vector<char> &alph, char c) {
	for(size_t i = 0; i < alph.size(); i++) {
		if(alph[i] == c) return i;
	}

	return std::numeric_limits<size_t>::max();
}

void porta(const std::string &msg, const std::string &key, bool to_right) {
	std::vector<std::vector<char>> alphabets = gen_alphs(key, to_right);
	size_t keycounter = 0;
	std::string encmsg;
	for(char c : msg) {
		if(keycounter == key.size())
			keycounter = 0;
		if(c >= 'A' && c <= 'M') {
			int cpos = c - 'A';
			encmsg += alphabets[keycounter][cpos];
			keycounter++;
		} else if(c >= 'N' && c <= 'Z') {
			int cpos = get_pos(alphabets[keycounter], c);
			encmsg += first_half[cpos];
			keycounter++;
		}
	}
	std::cout << postprocess_msg(encmsg);
}

int main(int argc, char ** argv) {
	if(argc < 3) {
		std::cout << "Usage: " << argv[0] << " [msg] [key] <alphabet displacement (default: right) [i|d]>\n";
		return 1;
	}
	std::string msg = remove_delims(argv[1]);
	std::string key = remove_delims(argv[2]);
	bool to_right = true;
	if(argc == 4 && std::string(argv[3]) == "i") to_right = false; 

	porta(msg, key, to_right);
	std::cout << std::endl;
	return 0;
}