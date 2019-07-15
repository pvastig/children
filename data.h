#pragma once

#include <forward_list>
#include <string>
#include <string_view>
#include <set>
#include <map>
#include <vector>
#include <unordered_map>
#include <unordered_set>

namespace pa
{
class IData
{
public:
    virtual ~IData();
    virtual void read(std::string_view fileName) = 0;
    virtual void print() const = 0;
};

class DataFile : public IData
{
public:
    virtual ~DataFile();

protected:
    std::string m_fileName;
    size_t m_countLines = 0;
};

using DataArray      = std::vector<IData*>;
using StringList     = std::forward_list<std::string>;
//using StringArray    = std::vector<std::string>;
using StringSet      = std::set<std::string>;
using StringUnordSet = std::unordered_set<std::string>;
using StringMap      = std::map<std::string, StringSet>;
using StringUnordMap = std::unordered_map<std::string, StringUnordSet>;

class ChildrenNames : public DataFile
{
public:
    void read(std::string_view fileName) override;
    void print() const override;
    StringUnordSet const & childrenNames() const { return m_childrenNames; }

private:
    StringUnordSet m_childrenNames;
};

class ChildrenRelations : public DataFile
{
public:
    void read(std::string_view fileName) override;
    void print() const override;
    StringUnordMap const & name2Relations() const { return m_name2Relations; }

private:
    StringUnordMap m_name2Relations;
};

class ProcessDataFacade
{
public:
    ProcessDataFacade(int argc, char ** argv);
    void run();

    ProcessDataFacade(ProcessDataFacade&) = delete;
    ProcessDataFacade & operator=(ProcessDataFacade&) = delete;

    //1 list of unloved children
    StringList unlovedChildrenNames() const;
    //2 list of unhappy children
    StringList unhappyChildrenNames() const;
    //3 list of favorite children
    StringList favouriteChildrenNames() const;

private:
    int const m_argc;
    char ** m_argv;
    ChildrenNames m_childrenNames;
    ChildrenRelations m_childrenRelations;
};
};
