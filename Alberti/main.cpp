#include <iostream>
#include <string>
#include "../Common/strprocess.hpp"

const static std::string default_alph = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

enum mode {
	ENCRYPT,
	DECRYPT
};

size_t get_pos(char c, const std::string &alphabet) {
	for(size_t i = 0; i < alphabet.size(); i++)
		if(alphabet[i] == c) return i;
	return -1;
}

void print_alphabet(const std::string &alphabet) {
	for(char c : alphabet) {
		std::cout << c;
	}
	std::cout << std::endl;
}

void decrypt(const std::string &msg, const std::string &alfpos, const std::string &alphabet, int period, int steps, bool to_right) {
	int count = 0;
	int disp = (get_pos(alfpos[0], default_alph) - get_pos(alfpos[1], alphabet) + 26) % 26;
	std::string decmsg;

	print_alphabet(alphabet);
	print_alphabet(default_alph);

	for(size_t i = 0; i < msg.size(); i++) {
		char c = msg[i];
		if(count != 0 && count % period == 0) {
			if(to_right) 
				disp = (disp + steps) % 26;
			else
				disp = (disp - steps + 26) % 26;
		}
		int cval = get_pos(c, alphabet);

		c = default_alph[(cval + disp) % 26]; //((c - 'A') + disp) % 26 + 'A';
		decmsg += c;
		count++;
	}

	std::cout << postprocess_msg(decmsg);
}

void encrypt(const std::string &msg, const std::string &alfpos, const std::string &alphabet, int period, int steps, bool to_right) {
	int count = 0;
	int disp = (get_pos(alfpos[0], default_alph) - get_pos(alfpos[1], alphabet) + 26) % 26;
	std::string encmsg;

	std::cout << "Initial displacement: " << disp << std::endl;

	print_alphabet(default_alph);
	print_alphabet(alphabet);

	for(size_t i = 0; i < msg.size(); i++) {
		char c = msg[i];
		if(count != 0 && count % period == 0) {
			if(to_right) 
				disp = (disp + steps) % 26;
			else
				disp = (disp - steps + 26) % 26;
		}
		int cval = get_pos(c, default_alph);
		c = alphabet[(cval - disp + 26) % 26]; //((c - 'A') - disp + 26) % 26 + 'A';
		encmsg += c;
		count++;
	}
	std::cout << postprocess_msg(encmsg);
}

int main(int argc, char ** argv) {
	if(argc < 7) {
		std::cout << "Usage: " << argv[0] << " [-d|-e] [msg] [alphabet_initial_position] [period] [steps] [direction (d|i)] <custom_alphabet>\n";
		return 1;
	}
	std::string msg = remove_delims(argv[2]);
	std::string alfpos = remove_delims(argv[3]);
	int period = std::stoi(argv[4]);
	int steps = std::stoi(argv[5]);
	bool to_right = std::string(argv[6]) == "d";

	mode mode = std::string(argv[1]) == "-d" ? DECRYPT : ENCRYPT;

	std::string alphabet;
	if(argc >= 8) alphabet = remove_delims(argv[7]);
	else alphabet = default_alph;

	switch (mode) {
		case ENCRYPT:
			encrypt(msg, alfpos, alphabet, period, steps, to_right);
			break;
		case DECRYPT:
			decrypt(msg, alfpos, alphabet, period, steps, to_right);
			break;
	}
	
	std::cout << std::endl;
	return 0;
}