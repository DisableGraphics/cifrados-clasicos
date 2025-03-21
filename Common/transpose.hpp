#include <string>
#include <vector>

std::string transpose(const std::string &msg, const std::string &key_trans);
std::string inverse_transpose(const std::string &msg, const std::string &key_trans);
std::pair<std::vector<int>, std::vector<int>> get_column_order(const std::string &key_trans);