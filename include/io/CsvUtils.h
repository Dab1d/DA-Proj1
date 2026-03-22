#ifndef INC_1PROJ_CSVUTILS_H
#define INC_1PROJ_CSVUTILS_H

#include <string>
#include <vector>

namespace CsvUtils {
    std::string trim(const std::string &value);

    std::string stripQuotes(const std::string &value);

    std::vector<std::string> splitCsvLine(const std::string &line);

    bool tryParseInteger(const std::string &value, int &result);

    bool parseOptionalInteger(const std::string &value, int &result);

    std::string removeInlineComment(const std::string &line);
}

#endif // INC_1PROJ_CSVUTILS_H
