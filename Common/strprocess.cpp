#include "strprocess.hpp"
#include <cctype>
#include <string>

std::string remove_delims(const std::string &msg, bool allow_numbers, bool separate) {
	std::string ret;
	bool has_already_separate = false;
	for(char c : msg) {
		if(c >= 'a' && c <= 'z') {
			has_already_separate = false;
			ret += std::toupper(c);
		} else if(c >= 'A' && c <= 'Z') {
			has_already_separate = false;
			ret += c;
		} else if(allow_numbers && c >= '0' && c <= '9') {
			has_already_separate = false;
			ret += c;
		} else if(separate) {
			if(!has_already_separate) {
				has_already_separate = true;
				ret += " ";
			}
		}
	}
	return ret;
}

//SPC (.) (,) (:) ? / ( ) " CODE

std::string ct46_delims(const std::string &msg) {
	std::string ret;
	for(char c : msg) {
		if(c >= 'a' && c <= 'z') {
			ret += std::toupper(c);
		} else if((c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || 
			c == '.' || c == ',' || c == ':' || c == '?' || c == '/' || 
			c == '(' || c == ')' || c == '"' || c == CODE) {
			ret += c;
		} else {
			ret += " ";
		}
	}
	return ret;
}

std::string strad_delims(const std::string &msg) {
	std::string ret;
	for(char c : msg) {
		if(c >= 'a' && c <= 'z') {
			ret += std::toupper(c);
		} else if((c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || 
			c == '.' || c == '#') {
			ret += c;
		}
	}
	return ret;
}

std::string numbers_only(const std::string &msg) {
	std::string ret;
	for(char c : msg) {
		if(c >= '0' && c <= '9') {
			ret += c;
		}
	}
	return ret;
}

std::string postprocess_msg(const std::string &msg) {
	std::string ret;
	for(size_t i = 0; i < msg.size(); i++) {
		ret += msg[i];
		if(i != 0 && (i+1) % 5 == 0)
			ret += " ";
	}
	return ret;
}

std::string remove_spaces(const std::string &msg) {
	std::string ret;
	for(auto &c : msg) {
		if(c != ' ') ret.push_back(c);
	}
	return ret;
}