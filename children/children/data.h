#pragma once
#ifndef _data_h__
#define _data_h__

#include "stdafx.h"

namespace PavelA
{
  class iData
  {
  public:
    iData() : m_countLines(0) {};
    virtual ~iData() {};
    virtual void readFromFile(const std::string& fileNameIn) = 0;


    virtual void printData() = 0;

  protected:
    unsigned int m_countLines;

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
    void readFromFile(const std::string & fileNameIn) override;
    void printData() override;
    
    const StringSet & getData() const { return m_names; }

  private:
    StringSet m_names;
    std::string m_fileNameIn;
  };

  class DataRelationsName : public iData
  {
  public:
    void readFromFile(const std::string & fileNameIn);
    void printData() override;

    const StringArrayUnordMap & getData() const { return m_namesRelations; }

  private:
    StringArrayUnordMap m_namesRelations;
    std::string m_fileNameIn;
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
    {
      for (const auto& it : container)
        std::cout << it << '\n';
    }
  }

  template<typename Any>
  void printDataMap(const Any& container)
  {
    std::cout << "\n**************** Printed data" << std::endl;
    for (const auto& it : container)
      std::cout << it.first << ' ' << it.second << '\n';
  }
};

#endif //_data_h__

