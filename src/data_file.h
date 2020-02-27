#pragma once

#include "user_types.h"

#include <memory>

namespace pa {

struct ParsedResult;

class DataFile
{
public:
    explicit DataFile(std::string_view fileName);
    virtual ~DataFile();
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

StringList unlovedChildrenNames(StringUnordSet const& childrenNames,
                                StringUnordMap const& name2RelatedNames);
StringList unhappyChildrenNames(StringUnordMap const& name2RelatedNames);
StringList favoriteChildrenNames(StringUnordMap const& name2RelatedNames);

std::ostream& operator<<(std::ostream& os, StringList const& container);

} // namespace pa
