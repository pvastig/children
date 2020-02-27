#include "data_file.h"

#include "parse_result.h"
#include "utils.h"

#include <algorithm>
#include <cassert>
#include <fstream>
#include <future>
#include <sstream>
#include <vector>

namespace pa {

DataFile::DataFile(std::string_view fileName) : m_fileName(fileName) {}

DataFile::~DataFile() = default;

void DataFile::logWarnings()
{
    if (m_warnings.empty())
        return;

    utils::Log logToFile(m_fileName.data());
    for (auto const& warning : m_warnings)
        logToFile << warning;
}

ChildrenNamesFile::ChildrenNamesFile(std::string_view fileName) : DataFile(fileName) {}

bool ChildrenNamesFile::read()
{
    // TODO: take into account binary mode
    std::ifstream ifs(m_fileName.data());
    assert(ifs.is_open());
    if (!ifs.is_open())
        return false;

    // TODO: add checking if line contains 2 or more words
    std::string line;
    // TODO: improve reading using chunks, take a look at boost?
    WarningVector warnings;
    while (std::getline(ifs, line)) {
        ++m_countLines;
        if (line.empty()) {
            warnings.emplace_back(std::to_string(m_countLines) + " is empty");
            continue;
        }

        if (auto [it, inserted] = m_childrenNames.insert(line); !inserted)
            warnings.emplace_back(std::to_string(m_countLines) + " has duplicated data : " + line);
    }
    m_warnings = std::move(warnings);
    return m_warnings.empty();
}

ChildrenRelationsFile::ChildrenRelationsFile(std::string_view fileName) : DataFile(fileName) {}

bool ChildrenRelationsFile::read()
{
    std::ifstream ifs(m_fileName.data());
    assert(ifs.is_open());
    if (!ifs.is_open())
        return false;

    std::string line;
    WarningVector warnings;
    while (std::getline(ifs, line)) {
        ++m_countLines;
        if (line.empty()) {
            warnings.emplace_back(std::to_string(m_countLines) + " is empty");
            continue;
        }

        // TODO: add check if line contains one word or more than 2 words
        std::istringstream iss(line);
        std::string word1, word2;
        // TODO: investigate this case
        if (!(iss >> word1 >> word2)) {
            warnings.emplace_back(std::to_string(m_countLines) + " has invalid data: " + line);
            continue;
        }
        // The case is from task
        if (word1 == word2) {
            warnings.push_back(std::to_string(m_countLines) + " has same words: " + line);
            continue;
        }
        m_name2RelatedNames[word1].insert(word2);
    }
    m_warnings = std::move(warnings);
    return m_warnings.empty();
}

StringList unlovedChildrenNames(StringUnordSet const& childrenNames,
                                StringUnordMap const& name2RelatedNames)
{
    auto foundHappyName = [&name2RelatedNames](auto const& childrenName) {
        for (auto const& [dummy, name2Relation] : name2RelatedNames)
            if (name2Relation.find(childrenName) != name2Relation.cend())
                return true;
        return false;
    };
    StringList result;
    for (auto const& childrenName : childrenNames)
        if (!foundHappyName(childrenName))
            result.push_front(childrenName);
    return result;
}

StringList unhappyChildrenNames(StringUnordMap const& name2RelatedNames)
{
    auto foundHappyName = [&name2RelatedNames](auto const& childrenName) {
        for (auto const& [dummy, name2Relation] : name2RelatedNames)
            if (name2Relation.find(childrenName) != name2Relation.cend())
                return true;
        return false;
    };
    StringList results;
    for (auto const& [childrenName, dummy] : name2RelatedNames)
        if (!foundHappyName(childrenName))
            results.push_front(childrenName);
    return results;
}

StringList favoriteChildrenNames(StringUnordMap const& name2RelatedNames)
{
    std::unordered_map<std::string, size_t> favoriteName2Count;
    for (auto const& [dummy, relatedNames] : name2RelatedNames)
        for (auto const& relatedName : relatedNames)
            ++favoriteName2Count[relatedName];

    StringList results;
    for (auto const& [name, count] : favoriteName2Count)
        if (size_t const filter = 1; count > filter)
            results.push_front(name + ": " + std::to_string(count));
    return results;
}

std::ostream& operator<<(std::ostream& os, StringList const& container)
{
    os << "(\n";
    // TODO: thinking about setting width output
    utils::print(os, container);
    return os << ")";
}

} // namespace pa
