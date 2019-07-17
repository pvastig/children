#pragma once

#include <forward_list>
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>

namespace pa
{

class DataFile
{
public:
    virtual ~DataFile();
    virtual void read(std::string_view fileName) = 0;

protected:
    std::string m_fileName;
    size_t m_countLines = 0;
};

using StringList     = std::forward_list<std::string>;
using StringUnordSet = std::unordered_set<std::string>;
using StringUnordMap = std::unordered_map<std::string, StringUnordSet>;

class ChildrenNames : public DataFile
{
public:
    void read(std::string_view fileName) override;
    StringUnordSet const & childrenNames() const { return m_childrenNames; }

private:
    StringUnordSet m_childrenNames;
};

class ChildrenRelations : public DataFile
{
public:
    void read(std::string_view fileName) override;
    StringUnordMap const & name2Relations() const { return m_name2Relations; }

private:
    StringUnordMap m_name2Relations;
};

class ProcessDataFacade
{
public:
    ProcessDataFacade(int argc, char ** argv);
    void run() const;

    ProcessDataFacade(ProcessDataFacade&) = delete;
    ProcessDataFacade & operator=(ProcessDataFacade&) = delete;

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
};
