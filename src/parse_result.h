#pragma once

#include "user_types.h"

namespace pa {

struct ParsedResult
{
    StringUnordSet childrenNames;
    StringUnordMap name2RelatedNames;
    size_t size = 0;
};

class ParseResult
{
public:
    explicit ParseResult(ResultVector const& result);
    ParsedResult parse();

private:
    ResultVector m_result;
};

} // namespace pa
