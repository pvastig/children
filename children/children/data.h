#pragma once
#ifndef _data_h__
#define _data_h__

#include "stdafx.h"

namespace PavelA
{
  class iData
  {
  public:
    iData() {};
    virtual ~iData() {};
    virtual void read(const std::string&) = 0;
    virtual void print() = 0;
    iData(const iData &) = delete;
    iData & operator=(const iData &) = delete;
  };

  typedef std::unique_ptr<iData>   DataPtr;
  typedef std::vector<DataPtr>     DataPtrArray;
  typedef std::forward_list<std::string> StringList;
  typedef std::vector<std::string> StringArray;
  typedef std::set<std::string>    StringSet;
  typedef std::unordered_set<std::string>  StringUnordSet;
  typedef std::map<std::string, StringSet> StringArrayMap;
  typedef std::unordered_map<std::string, StringUnordSet> StringArrayUnordMap;
  typedef std::map<std::string, size_t> StringMap;

  class DataNames : public iData
  {
  public:
    void read(const std::string & fileName) override;
    void print() override;
    const StringSet & getData() const { return m_names; }

  private:
    StringSet m_names;
    std::string m_fileName;
    size_t m_countLines;
  };

  class DataRelationsName : public iData
  {
  public:
    void read(const std::string & fileNameIn);
    void print() override;

    const StringArrayUnordMap & getData() const { return m_namesRelations; }

  private:
    StringArrayUnordMap m_namesRelations;
    std::string m_fileName;
    size_t m_countLines;
  };

  class ProcessData
  {
  public:
    ProcessData(const int argc, char ** argv);

    //1 list of unloved children
    StringList unlovedChildrenNames() const;

    //2 list of unhappy children
    StringList unhappyChildrenNames() const;

    //3 list of favorite children
    StringList favouriteChildrenNames() const;

    ProcessData(ProcessData&) = delete;
    ProcessData & operator=(ProcessData&) = delete;

  private:
    const int m_argc;
    char ** m_argv;
    DataNames m_dataNames;
    DataRelationsName m_dataRelationName;
  };

  template<typename Any>
  void printData(const Any& container)
  {
    std::cout << "\n**************** Printed data" << std::endl;
    std::copy(container.begin(), container.end(), std::ostream_iterator<Any::value_type>(std::cout, "\n"));
  }

  template<typename Any>
  void printDataMap(const Any& container)
  {
    std::cout << "\n**************** Printed data" << std::endl;
    for (const auto& item : container)
      std::cout << item.first << ' ' << item.second << '\n';
  }
};

#endif //_data_h__

