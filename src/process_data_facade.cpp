#include "process_data_facade.h"

#include "data_file.h"
#include "display_data.h"
#include "parse_result.h"
#include "utils.h"

#include <cassert>
#include <filesystem>
#include <future>
#include <vector>

namespace pa {

ProcessDataFacade::ProcessDataFacade(int argc, char const** argv)
{
    if (argc == 4) {
        // TODO: make possibility process arguments and make help for them
        if (argv[3] == std::string("--log")) {
            m_logToFile = true;
        }
    } else if (argc != 3)
        throw std::invalid_argument("You should input 2 files");

    namespace fs = std::filesystem;
    auto wrongFilePath = [](std::string_view str) {
        fs::path filePath(str);
        return !fs::exists(filePath) || !filePath.has_filename();
    };
    std::string_view childrenFilePath = argv[1];
    if (wrongFilePath(childrenFilePath))
        throw std::invalid_argument(childrenFilePath.data() + std::string(" does not exist"));

    std::string_view childrenRelationsFilePath = argv[2];
    if (wrongFilePath(childrenRelationsFilePath))
        throw std::invalid_argument(childrenRelationsFilePath.data()
                                    + std::string(" does not exist"));

    m_dataFile.emplace_back(std::make_unique<ChildrenNamesFile>(childrenFilePath));
    m_dataFile.emplace_back(std::make_unique<ChildrenRelationsFile>(childrenRelationsFilePath));
}

ProcessDataFacade::~ProcessDataFacade() = default;

void ProcessDataFacade::run() const
{
    auto const readResult = readData();
    auto const parsedResult = ParseResult(readResult).parse();
    assert(parsedResult.size == 2);
    DisplayData(parsedResult).run();
}

ResultVector ProcessDataFacade::readData() const
{
    ResultVector result;
    for (auto&& item : m_dataFile) {
        utils::runAsync([&item]() { item->read(); }).get();
        result.emplace_back(item->result());
    }
    if (m_logToFile) {
        for (auto&& item : m_dataFile)
            item->logWarnings();
    }
    return result;
}

} // namespace pa
