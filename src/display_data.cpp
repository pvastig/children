#include "display_data.h"

#include "data_file.h"
#include "parse_result.h"
#include "utils.h"

#include <iostream>

namespace pa {


DisplayData::DisplayData(ParsedResult const& result)
    : m_result(std::make_unique<ParsedResult>(result))
{}

void DisplayData::run() const
{
    std::string_view menu = R"(
    Select action:
    1 - Unloved children
    2 - Unhappy children
    3 - Favorite children
    ----------------------
    0 - exit
==> )";

    enum class UserSelect : char {
        Exit,
        UnlovedChildrenNames,
        UnhappyChildrenNames,
        FavouriteChildrenNames
    };
    bool readAgain = true;
    do {
        std::cout << menu;
        int num = -1;
        std::cin >> num;
        if (!std::cin) {
            std::cin.clear();
            std::string s;
            std::cin >> s;
        }
        switch (static_cast<UserSelect>(num)) {
        case UserSelect::UnlovedChildrenNames:
            // TODO: instead of printing to console, print to file?
            std::cout << unlovedChildrenNames(m_result->childrenNames, m_result->name2RelatedNames);
            break;
        case UserSelect::UnhappyChildrenNames:
            std::cout << unhappyChildrenNames(m_result->name2RelatedNames);
            break;
        case UserSelect::FavouriteChildrenNames:
            std::cout << favoriteChildrenNames(m_result->name2RelatedNames);
            break;
        case UserSelect::Exit:
            std::cout << "Bye-bye :)" << utils::newLine;
            readAgain = false;
            break;
        default:
            std::cout << "You entered not existed action, please, try again:)" << std::endl;
            break;
        }
    } while (readAgain);
}

} // namespace pa
