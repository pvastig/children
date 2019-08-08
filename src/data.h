#pragma once

#include <future>
#include <forward_list>
#include <string>
#include <string_view>
#include <vector>
#include <unordered_map>
#include <unordered_set>

namespace pa
{

class DataFile
{
public:
    virtual ~DataFile();
    virtual bool read(std::string_view fileName) = 0;

protected:
    size_t m_countLines = 0;
};

using StringList     = std::forward_list<std::string>;
using StringUnordSet = std::unordered_set<std::string>;
using StringUnordMap = std::unordered_map<std::string, StringUnordSet>;

class ChildrenNames : public DataFile
{
public:
    bool read(std::string_view fileName) override;
    auto const & names() const { return m_childrenNames; }

private:
    StringUnordSet m_childrenNames;
};

class ChildrenRelations : public DataFile
{
public:
    bool read(std::string_view fileName) override;
    auto const & name2RelatedNames() const { return m_name2RelatedNames; }

private:
    StringUnordMap m_name2RelatedNames;
};

class ProcessDataFacade
{
public:
    ProcessDataFacade(int argc, char const ** argv);
    void run();

    //list of unloved children
    StringList unlovedChildrenNames() const;
    //list of unhappy children
    StringList unhappyChildrenNames() const;
    //list of favorite children
    StringList favouriteChildrenNames() const;

private:
    ChildrenNames m_childrenNames;
    ChildrenRelations m_childrenRelations;
};
}
