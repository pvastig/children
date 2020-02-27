#pragma once

#include <forward_list>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <variant>
#include <vector>

namespace pa {

using StringList = std::forward_list<std::string>;
using StringUnordSet = std::unordered_set<std::string>;
using StringUnordMap = std::unordered_map<std::string, StringUnordSet>;
using ResultVariant = std::variant<StringUnordSet, StringUnordMap>;
using ResultVector = std::vector<ResultVariant>;
using WarningVector = std::vector<std::string>;

} // namespace pa
