#pragma once
#ifndef _data_h__
#define _data_h__

#include "stdafx.h"

namespace PavelA
{
class iData
{
public:
  iData()
  {
  }
  virtual ~iData()
  {
  };
  virtual void read(const std::string&) = 0;
  virtual void print() = 0;

  iData(const iData &) = delete;
  iData & operator=(const iData &) = delete;
};

class DataFile : public iData
{
public:
  virtual ~DataFile()
  {
  }
protected:
  std::string m_fileName;
  size_t m_countLines;
};

using DataPtrArray = std::vector<iData*>;
using StringList = std::forward_list<std::string>;
using StringArray = std::vector<std::string>;
using StringSet = std::set<std::string>;
using StringUnordSet = std::unordered_set<std::string>;
using StringArrayMap = std::map<std::string, StringSet>;
using StringArrayUnordMap = std::unordered_map<std::string, StringUnordSet>;
using StringMap = std::map<std::string, size_t>;

class Names : public DataFile
{
public:
  void read(const std::string& fileName) override;
  void print() override;
  const StringUnordSet& getData() const { return m_names; }

private:
  StringUnordSet m_names;
};

class ChildrenRelations : public DataFile
{
public:
  void read(const std::string& fileNameIn);
  void print() override;
  const StringArrayUnordMap& getData() const { return m_namesRelations; }

private:
  StringArrayUnordMap m_namesRelations;
};

class ProcessData
{
public:
  ProcessData(const int argc, char ** const argv);
  void run();

  ProcessData(ProcessData&) = delete;
  ProcessData & operator=(ProcessData&) = delete;

  //1 list of unloved children
  StringList unlovedChildrenNames() const;
  //2 list of unhappy children
  StringList unhappyChildrenNames() const;
  //3 list of favorite children
  StringList favouriteChildrenNames() const;

private:
  const int m_argc;
  char ** const m_argv;
  Names m_names;
  ChildrenRelations m_childrenRelations;
};
};

#endif //_data_h__

