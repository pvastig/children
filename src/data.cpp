#include "data.h"

#include "utils.h"

#include <algorithm>
#include <cassert>
#include <filesystem>
#include <future>
#include <fstream>

namespace pa {
DataFile::~DataFile() = default;

bool ChildrenNames::read(std::string_view fileName)
{
    std::ifstream ifs(fileName.data(), std::ios_base::in);
    assert(ifs.is_open());
    if (!ifs.is_open())
        return false;

    LOG.setFileName(fileName);

    std::string line;
    while (std::getline(ifs, line))
    {
        ++m_countLines;
        if (line.empty())
        {
            LOG << m_countLines << " is empty" << utils::newLine;
            continue;
        }

        if (auto [it, inserted] = m_childrenNames.insert(line); !inserted)
            LOG << m_countLines << " has duplicated data : " << line << utils::newLine;
    }

    return true;
}

bool ChildrenRelations::read(std::string_view fileName)
{
    std::ifstream ifs(fileName.data(), std::ios_base::in);
    assert(ifs.is_open());
    if (!ifs.is_open())
        return false;

    LOG.setFileName(fileName);

    std::string line;
    while (std::getline(ifs, line))
    {
        ++m_countLines;
        if (line.empty())
        {
            LOG << m_countLines << " is empty" << utils::newLine;
            continue;
        }

        std::istringstream iss(line);
        std::string word1, word2;
        //TODO: investigate this case
        if (!(iss >> word1 >> word2))
        {
            LOG << m_countLines << " has invalid data: " << line << utils::newLine;
            continue;
        }
        //The case is from task
        if (word1 == word2)
        {
            LOG << m_countLines << " has same words: " << line << utils::newLine;
            continue;
        }

        m_name2RelatedNames[word1].insert(word2);
    }

    return true;
}

ProcessDataFacade::ProcessDataFacade(int argc, char const ** argv)
{
    //TODO: split on functions
    if (argc == 4)
    {
        //TODO: make possibility process arguments and make help for them
        if (argv[3] == std::string("--log"))
            LOG.enableLog(true);
    }
    else if (argc != 3)
        throw std::invalid_argument("You should input 2 files");

    namespace fs = std::filesystem;
    auto wrongFilePath = [](std::string_view str)
    {
        fs::path filePath(str);
        return !fs::exists(filePath) || !filePath.has_filename();
    };
    std::string_view childrenFilePath = argv[1];
    if (wrongFilePath(childrenFilePath))
        throw std::invalid_argument(childrenFilePath.data() + std::string(" does not exist"));

    std::string_view childrenRelationsFilePath = argv[2];
    if (wrongFilePath(childrenRelationsFilePath))
        throw std::invalid_argument(childrenRelationsFilePath.data() +  std::string(" does not exist"));

    auto future = utils::runAsync([this, childrenFilePath]()
                       {
                           m_childrenNames.read(childrenFilePath);
                       });
    future.get();
    m_childrenRelations.read(childrenRelationsFilePath);
}

StringList ProcessDataFacade::unlovedChildrenNames() const
{
    auto const & chilrenNames   = m_childrenNames.names();
    auto const & name2Relations = m_childrenRelations.name2RelatedNames();
    auto foundHappyName = [&name2Relations](auto const & childrenName)
    {
        for (auto const & [dummy, name2Relation] : name2Relations)
            if (name2Relation.find(childrenName) != name2Relation.cend())
                return true;
        return false;
    };
    StringList result;
    for (auto const & childrenName : chilrenNames)
        if (!foundHappyName(childrenName))
            result.push_front(childrenName);

    return result;
}

StringList ProcessDataFacade::unhappyChildrenNames() const
{
    auto const & name2Relations = m_childrenRelations.name2RelatedNames();
    auto foundHappyName = [&name2Relations](auto const & childrenName)
    {
        for (auto const & [dummy, name2Relation] : name2Relations)
            if (name2Relation.find(childrenName) != name2Relation.cend())
                return true;
        return false;
    };
    StringList results;
    for (auto const & [childrenName, dummy] : name2Relations)
        if (!foundHappyName(childrenName))
            results.push_front(childrenName);

    return results;
}

StringList ProcessDataFacade::favouriteChildrenNames() const
{
    auto const & name2RelatedNames = m_childrenRelations.name2RelatedNames();
    std::unordered_map<std::string, size_t> favoriteName2Count;
    for (auto const & [dummy, relatedNames] : name2RelatedNames)
        for (auto const & relatedName : relatedNames)
            ++favoriteName2Count[relatedName];

    StringList results;
    for (auto const & [name, count] : favoriteName2Count)
        if (size_t const filter = 1; count > filter)
            results.push_front(name + ": " + std::to_string(count));

    return results;
}

void ProcessDataFacade::run() {
  const std::string menu = R"(
Select action:
    1 - Unloved children
    2 - Unhappy children
    3 - Favorite children
    ------------------------
     0 - exit
===> )";

  enum class UserSelect : char {
    Exit,
    UnlovedChildrenNames,
    UnhappyChildrenNames,
    FavouriteChildrenNames
  };
  bool readAgain = true;
  do {
    std::cout << menu;
    int num = 0;
    std::cin >> num;
    if (!std::cin) {
      std::cin.clear();
      std::string s;
      std::cin >> s;
      num = -1;
    }
    switch (static_cast<UserSelect>(num)) {
      case UserSelect::UnlovedChildrenNames:
        // TODO: instead of printing to console, print to file?
        std::cout << unlovedChildrenNames();
        break;
      case UserSelect::UnhappyChildrenNames:
        std::cout << unhappyChildrenNames();
        break;
      case UserSelect::FavouriteChildrenNames:
        std::cout << favouriteChildrenNames();
        break;
      case UserSelect::Exit:
        std::cout << "Bye-bye :)" << utils::newLine;
        readAgain = false;
        break;
      default:
        std::cout << "You entered not existed action, please, try again:)"
                  << std::endl;
        break;
    }
  } while (readAgain);
}

std::ostream &operator<<(std::ostream &os, const StringList &container) {
  os << "(\n";
  // TODO: thinking about setting width output
  utils::print(os, container);
  return os << ")";
}
}  // namespace pa
