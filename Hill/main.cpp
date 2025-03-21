#include <iostream>
#include <array>
#include <stdexcept>
#include "../Common/strprocess.hpp"

constexpr int ALPH_SIZE = 26;

enum mode {
	ENCRYPT,
	DECRYPT
};

mode get_mode(const std::string &modestr) {
	if (modestr == "-e")
		return ENCRYPT;
	else if (modestr == "-d")
		return DECRYPT;
	throw std::runtime_error("Illegal mode: " + modestr);
}

int mod_inverse(int a, int mod) {
	a = a % mod;
	for (int x = 1; x < mod; x++) {
		if ((a * x) % mod == 1) {
			return x;
		}
	}
	throw std::runtime_error("No modular inverse exists for given key.");
}

int determinant(const std::array<int, 4>& keys) {
	return (keys[0] * keys[3] - keys[1] * keys[2]) % ALPH_SIZE;
}

std::array<int, 4> inverse_key(const std::array<int, 4>& keys) {
	int det = determinant(keys);
	int det_inv = mod_inverse(det, ALPH_SIZE);
	return {
		(keys[3] * det_inv) % ALPH_SIZE, (-keys[1] * det_inv) % ALPH_SIZE,
		(-keys[2] * det_inv) % ALPH_SIZE, (keys[0] * det_inv) % ALPH_SIZE
	};
}

void encrypt(const std::string& msg, const std::array<int, 4>& keys) {
	std::string encmsg;
	for (size_t i = 0; i < msg.size(); i += 2) {
		int x = msg[i] - 'A';
		int y = msg[i + 1] - 'A';
		int enc_x = (keys[0] * x + keys[1] * y) % ALPH_SIZE;
		int enc_y = (keys[2] * x + keys[3] * y) % ALPH_SIZE;

		char xc = static_cast<char>((enc_x + ALPH_SIZE) % ALPH_SIZE + 'A');
		char yc = static_cast<char>((enc_y + ALPH_SIZE) % ALPH_SIZE + 'A');

		printf("%4d * %c + %4d * %c = %c\n", keys[0], msg[i], keys[1], msg[i + 1], xc);
		printf("%4d * %c + %4d * %c = %c\n", keys[2], msg[i], keys[3], msg[i + 1], yc);

		encmsg += xc;
		encmsg += yc;
	}
	std::cout << postprocess_msg(encmsg);
}

void decrypt(const std::string& msg, const std::array<int, 4>& keys) {
	std::string decmsg;
	std::array<int, 4> inv_keys = inverse_key(keys);
	for (size_t i = 0; i < msg.size(); i += 2) {
		int x = msg[i] - 'A';
		int y = msg[i + 1] - 'A';
		int dec_x = (inv_keys[0] * x + inv_keys[1] * y) % ALPH_SIZE;
		int dec_y = (inv_keys[2] * x + inv_keys[3] * y) % ALPH_SIZE;
		char xc = static_cast<char>((dec_x + ALPH_SIZE) % ALPH_SIZE + 'A');
		char yc = static_cast<char>((dec_y + ALPH_SIZE) % ALPH_SIZE + 'A');

		printf("%4d * %c + %4d * %c = %c\n", inv_keys[0], msg[i], inv_keys[1], msg[i + 1], xc);
		printf("%4d * %c + %4d * %c = %c\n", inv_keys[2], msg[i], inv_keys[3], msg[i + 1], yc);

		decmsg += xc;
		decmsg += yc;
	}
	std::cout << postprocess_msg(decmsg);
}

int main(int argc, char **argv) {
	if (argc < 7) {
		std::cout << "Usage: " << argv[0] << " [-d|-e] [msg] [k11] [k12] [k21] [k22]\n";
		return 1;
	}

	mode mode = get_mode(argv[1]);
	std::string msg = remove_delims(argv[2], true);
	int k11 = std::stoi(argv[3]);
	int k12 = std::stoi(argv[4]);
	int k21 = std::stoi(argv[5]);
	int k22 = std::stoi(argv[6]);

	std::array<int, 4> keys{k11, k12, k21, k22};

	if (msg.size() % 2 != 0) {
		msg += 'X'; // Pad message to make it even-length
	}

	switch (mode) {
		case DECRYPT:
			decrypt(msg, keys);
			break;
		case ENCRYPT:
			encrypt(msg, keys);
			break;
	}
	std::cout << std::endl;

	return 0;
}
