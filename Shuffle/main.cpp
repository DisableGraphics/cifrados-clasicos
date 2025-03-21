#include <iostream>
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

void encrypt(const std::string &msg, int rows) {
	size_t columns = msg.size() / rows;
	std::vector<std::vector<char>> grid(rows, std::vector<char>(columns+1, '_'));

	for(int i = 0; i < columns+1; i++) {
		for(int j = 0; j < rows; j++) {
			size_t pos = i*rows + j;
			if(pos >= msg.size()) {
				goto end;
			}
			grid[j][i] = msg[pos];
		}
	}
	end:
	std::string encmsg;
	for(int i = 0; i < rows; i++) {
		for(int j = 0; j < columns+1; j++) {
			char c = grid[i][j];
			if(c != '_') {
				encmsg += c;
				std::cout << c;
			}
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
	std::cout << postprocess_msg(encmsg);
}

void decrypt(const std::string &msg, int rows) {
	size_t columns = msg.size() / rows;
	size_t resto = msg.size() % rows;
	std::vector<std::vector<char>> grid(rows, std::vector<char>(columns+1, '_'));
	size_t count = 0;
	for(int i = 0; i < rows; i++) {
		size_t maxcol = i >= resto ? columns : columns + 1;
		for(int j = 0; j < maxcol; j++) {
			if(count >= msg.size()) {
				goto end;
			}
			grid[i][j] = msg[count];
			count++;
		}
	}
	end:
	std::string decmsg;

	for(int i = 0; i < columns+1; i++) {
		for(int j = 0; j < rows; j++) {
			char c = grid[j][i];
			if(c != '_') {
				decmsg += c;
				std::cout << c;
			}
		}
		std::cout << std::endl;
	}

	std::cout << postprocess_msg(decmsg);
}

int main(int argc, char ** argv) {
	if(argc < 4) {
		std::cout << "Usage: " << argv[0] << " [-d|-e] [msg] [period]\n";
		return 1;
	}

	mode mode = get_mode(argv[1]);
	std::string msg = remove_delims(argv[2], true);
	int period = std::stoi(argv[3]);

	switch(mode) {
		case DECRYPT:
			decrypt(msg, period);
			break;
		case ENCRYPT:
			encrypt(msg, period);
			break;
	}
	std::cout << std::endl;

	return 0;
}