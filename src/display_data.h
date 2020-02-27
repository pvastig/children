#pragma once

#include <memory>

namespace pa {

struct ParsedResult;

class DisplayData
{
public:
    explicit DisplayData(ParsedResult const& result);
    void run() const;

private:
    std::unique_ptr<ParsedResult> m_result;
};

} // namespace pa
