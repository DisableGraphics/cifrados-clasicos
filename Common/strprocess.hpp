#include <string>

std::string remove_delims(const std::string &msg, bool allow_numbers = false, bool separate = false);
std::string strad_delims(const std::string &msg);
std::string numbers_only(const std::string &msg);
std::string postprocess_msg(const std::string &msg);
constexpr char CODE = '`';
std::string ct46_delims(const std::string &msg);
std::string remove_spaces(const std::string &msg);