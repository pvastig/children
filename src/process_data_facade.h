#pragma once

#include "user_types.h"

#include <memory>

namespace pa {

class DataFile;

class ProcessDataFacade
{
public:
    ProcessDataFacade(int argc, char const** argv);
    ~ProcessDataFacade();
    void run() const;

private:
    ResultVector readData() const;

private:
    std::vector<std::unique_ptr<DataFile>> m_dataFile;
    bool m_logToFile = false;
};

} // namespace pa
