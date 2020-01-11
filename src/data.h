#pragma once

#include <forward_list>
#include <future>
#include <ostream>
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace pa {
using Warnings = std::vector<std::string>;

class DataFile {
public:
  virtual ~DataFile();
  virtual Warnings read(std::string_view fileName) = 0;

protected:
  size_t m_countLines = 0;
};

using StringList     = std::forward_list<std::string>;
using StringUnordSet = std::unordered_set<std::string>;
using StringUnordMap = std::unordered_map<std::string, StringUnordSet>;

class ChildrenNames : public DataFile {
public:
  Warnings read(std::string_view fileName) override;
  auto const& names() const {
    return m_childrenNames;
  }

private:
  StringUnordSet m_childrenNames;
};

class ChildrenRelations : public DataFile {
public:
  Warnings read(std::string_view fileName) override;
  auto const& name2RelatedNames() const {
    return m_name2RelatedNames;
  }

private:
  StringUnordMap m_name2RelatedNames;
};

class ProcessDataFacade {
public:
  ProcessDataFacade(int argc, char const** argv);
  void run();

  // list of unloved children
  StringList unlovedChildrenNames() const;
  // list of unhappy children
  StringList unhappyChildrenNames() const;
  // list of favorite children
  StringList favouriteChildrenNames() const;

private:
  ChildrenNames m_childrenNames;
  ChildrenRelations m_childrenRelations;

private:
  std::unordered_map<std::string_view, std::future<Warnings>> m_file2Data;
  void collectAndLogData();
  bool m_logToFile = false;
};

std::ostream& operator<<(std::ostream& os, StringList const& container);
} // namespace pa
