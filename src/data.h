#pragma once

#include <forward_list>
#include <memory>
#include <ostream>
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <variant>
#include <vector>

namespace pa {

using WarningVector = std::vector<std::string>;
using StringList = std::forward_list<std::string>;
using StringUnordSet = std::unordered_set<std::string>;
using StringUnordMap = std::unordered_map<std::string, StringUnordSet>;
using ResultVariant = std::variant<StringUnordSet, StringUnordMap>;
using ResultVector = std::vector<ResultVariant>;

class DataFile
{
public:
    explicit DataFile(std::string_view fileName);
    virtual ~DataFile() = default;
    virtual bool read() = 0;
    virtual void logWarnings();
    virtual ResultVariant result() const = 0;

protected:
    size_t m_countLines = 0;
    WarningVector m_warnings;
    std::string m_fileName;
};

class ChildrenNamesFile : public DataFile
{
public:
    explicit ChildrenNamesFile(std::string_view fileName);
    bool read() override;
    ResultVariant result() const override { return m_childrenNames; }
    StringUnordSet const& childrenNames() const { return m_childrenNames; }

private:
    StringUnordSet m_childrenNames;
};

class ChildrenRelationsFile : public DataFile
{
public:
    explicit ChildrenRelationsFile(std::string_view fileName);
    bool read() override;
    ResultVariant result() const override { return m_name2RelatedNames; }
    StringUnordMap const& name2RelatedNames() const { return m_name2RelatedNames; }

private:
    StringUnordMap m_name2RelatedNames;
};

class ProcessDataFacade
{
public:
    ProcessDataFacade(int argc, char const** argv);
    void run() const;

private:
    ResultVector readData() const;

private:
    std::vector<std::unique_ptr<DataFile>> m_dataFile;
    bool m_logToFile = false;
};

struct ParsedResult
{
    StringUnordSet childrenNames;
    StringUnordMap name2RelatedNames;
    size_t size = 0;
};

class ParseResult
{
public:
    ParseResult(ResultVector const& result);
    ParsedResult parse();

private:
    ResultVector m_result;
};

class DisplayData
{
public:
    DisplayData(ParsedResult const& result);
    void run() const;

private:
    ParsedResult m_result;
};

StringList unlovedChildrenNames(StringUnordSet const& childrenNames,
                                StringUnordMap const& name2RelatedNames);
StringList unhappyChildrenNames(StringUnordMap const& name2RelatedNames);
StringList favoriteChildrenNames(StringUnordMap const& name2RelatedNames);

std::ostream& operator<<(std::ostream& os, StringList const& container);
} // namespace pa
