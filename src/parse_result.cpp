#include "parse_result.h"

#include <cassert>
#include <type_traits>

namespace pa {

namespace {

template<class T>
struct always_false : std::false_type
{};

} // namespace

ParseResult::ParseResult(const ResultVector& result) : m_result(result) {}

ParsedResult ParseResult::parse()
{
    // TODO: if size is not equal 2, it needs to rework the place
    assert(m_result.size() == 2);
    ParsedResult parsedResut;
    parsedResut.size = m_result.size();
    for (auto const& item : m_result) {
        std::visit(
            [&parsedResut, item](auto&& arg) {
                using T = std::decay_t<decltype(arg)>;
                if constexpr (std::is_same_v<T, StringUnordSet>)
                    parsedResut.childrenNames = arg;
                else if constexpr (std::is_same_v<T, StringUnordMap>)
                    parsedResut.name2RelatedNames = arg;
                else
                    static_assert(always_false<T>::value, "non-exhaustive visitor");
            },
            item);
    }
    return parsedResut;
}

} // namespace pa
