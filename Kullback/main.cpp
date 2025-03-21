#include <iostream>
#include <unordered_map>
#include "../Common/strprocess.hpp"

constexpr const char * spaces_prev = "    ";
constexpr const char * spaces_a = "   ";
constexpr const char * format = "%04ld ";

std::unordered_map<std::string, double> kappa_p {
	{"EN", 0.0667},
	{"ES", 0.0775}
};

void print_table(std::unordered_map<char, long> &freqs) {
	for(char c = 'A'; c <= 'Z'; c++) {
		std::cout << (c == 'A' ? spaces_a : spaces_prev) << c;
	}
	std::cout << std::endl;
	for(char i = 'A'; i <= 'Z'; i++) {
		printf(format, freqs[i]);
	}
	std::cout << std::endl;
	for(char i = 'A'; i <= 'Z'; i++) {
		auto val = freqs[i]-1;
		printf(format, val >= 0 ? val : 0);
	}
	std::cout << std::endl;
	for(char i = 'A'; i <= 'Z'; i++) {
		printf(format, (freqs[i]-1)*freqs[i]);
	}
	std::cout << std::endl << std::endl;
}

double phi_obs(const std::string &msg) {
	double ret = 0;
	std::unordered_map<char, long> freqs;
	for(size_t i = 0; i < msg.size(); i++) {
		char c = msg[i];
		freqs[c]++;
	}

	for(auto &elem : freqs) {
		double freq = elem.second;
		ret += freq * (freq - 1);
	}

	print_table(freqs);

	return ret;
}

double phi_k(const std::string &msg, const std::string &lang) {
	return msg.size() * (msg.size() - 1) * kappa_p[lang];
}

double phi_r(const std::string &msg) {
	return msg.size() * (msg.size() - 1) * (1.0/26.0);
}

void kullback(const std::string &msg, const std::string &lang) {
	double phir = phi_r(msg);
	double phik = phi_k(msg, lang);
	double phiobs = phi_obs(msg);

	std::cout << "phi_r: " << msg.size() << "*" << (msg.size() - 1) << "*" << (1.0/26.0) << " = " << phir << "\n"
		"phi_k: " << msg.size() << "*" << (msg.size() - 1) << "*" << kappa_p[lang] << " = " <<  phik << "\n"
		"phi_obs: " << phiobs << std::endl;
	double difpolialf = std::abs(phiobs - phir);
	double difmonoalf = std::abs(phiobs - phik);
	if(difpolialf > difmonoalf) {
		std::cout << "Possibly monoalphabetic: ";
	} else {
		std::cout << "Possibly polialphabetic: ";
	}
	std::cout << difpolialf << " vs " << difmonoalf << std::endl;
}

int main(int argc, char ** argv) {
	if(argc < 3) {
		std::cout << "Usage: " << argv[0] << " [msg] [lang]\n";
		return 1;
	}
	std::string msg = remove_delims(argv[1]);
	std::string lang = remove_delims(argv[2]);

	kullback(msg, lang);

	return 0;
}