#include <iostream>
#include <unordered_map>
#include "../Common/strprocess.hpp"

std::unordered_map<std::string, double> kappa_p {
	{"EN", 0.0667},
	{"ES", 0.0775}
};

constexpr const char * spaces_prev = "    ";
constexpr const char * spaces_a = "   ";
constexpr const char * format = "%04d ";

void chisq(const std::string &msg1, const std::string &msg2, const std::string &lang) {
	std::unordered_map<char, int> freq1, freq2;

	for(char c = 'A'; c <= 'Z'; c++) {
		std::cout << (c == 'A' ? spaces_a : spaces_prev) << c;
	}

	std::cout << std::endl;

	for (char c : msg1) {
		if (isalpha(c)) {
			char uppercase_char = toupper(c);
			freq1[uppercase_char]++;
		}
	}

	for (char c : msg2) {
		if (isalpha(c)) {
			char uppercase_char = toupper(c);
			freq2[uppercase_char]++;
		}
	}
	size_t sum = 0;
	for(char i = 'A'; i <= 'Z'; i++) {
		sum += freq1[i] * freq2[i];
		printf(format, freq1[i]);
	}
	std::cout << std::endl;
	for(char i = 'A'; i <= 'Z'; i++) {
		printf(format, freq2[i]);
	}
	std::cout << std::endl;
	for(char i = 'A'; i <= 'Z'; i++) {
		printf(format, freq1[i] * freq2[i]);
	}
	std::cout << std::endl;

	double chi_p_calc = kappa_p[lang] * msg1.size() * msg2.size();

	std::cout << "Chi calculated: " << chi_p_calc << " (" << msg1.size() << "*" << msg2.size() << "*" << kappa_p[lang] << ")" <<  std::endl;
	std::cout << "Chi real: " << sum << std::endl;
}

int main(int argc, char ** argv) {
	if(argc < 4) {
		std::cout << "Usage: " << argv[0] << " [msg1] [msg2] [lang]\n";
		return 1;
	}

	std::string msg1 = remove_delims(argv[1]);
	std::string msg2 = remove_delims(argv[2]);
	std::string lang = remove_delims(argv[3]);

	chisq(msg1, msg2, lang);

	return 0;
}