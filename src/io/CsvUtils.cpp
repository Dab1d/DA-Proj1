#include "io/CsvUtils.h"

#include <cctype>
#include <sstream>

namespace CsvUtils {
    std::string trim(const std::string &value) {
        size_t start = 0;
        while (start < value.size() &&
               std::isspace(static_cast<unsigned char>(value[start]))) {
            ++start;
        }

        size_t end = value.size();
        while (end > start &&
               std::isspace(static_cast<unsigned char>(value[end - 1]))) {
            --end;
        }

        return value.substr(start, end - start);
    }

    std::string stripQuotes(const std::string &value) {
        std::string trimmed = trim(value);
        if (trimmed.size() >= 2 && trimmed.front() == '"' && trimmed.back() == '"') {
            return trimmed.substr(1, trimmed.size() - 2);
        }
        return trimmed;
    }

    std::vector<std::string> splitCsvLine(const std::string &line) {
        std::vector<std::string> fields;
        std::string current;
        bool inQuotes = false;

        for (char ch : line) {
            if (ch == '"') {
                inQuotes = !inQuotes;
                current += ch;
                continue;
            }

            if (ch == ',' && !inQuotes) {
                fields.push_back(trim(current));
                current.clear();
                continue;
            }

            current += ch;
        }

        fields.push_back(trim(current));
        return fields;
    }

    bool tryParseInteger(const std::string &value, int &result) {
        std::string trimmed = trim(value);
        if (trimmed.empty()) {
            return false;
        }

        std::stringstream input(trimmed);
        input >> result;
        return input && input.eof();
    }

    bool parseOptionalInteger(const std::string &value, int &result) {
        std::string trimmed = trim(value);
        if (trimmed.empty()) {
            result = -1;
            return true;
        }

        return tryParseInteger(trimmed, result);
    }

    std::string removeInlineComment(const std::string &line) {
        bool inQuotes = false;
        for (size_t i = 0; i < line.size(); ++i) {
            const char ch = line[i];
            if (ch == '"') {
                inQuotes = !inQuotes;
            } else if (ch == '#' && !inQuotes) {
                return trim(line.substr(0, i));
            }
        }

        return trim(line);
    }
}
