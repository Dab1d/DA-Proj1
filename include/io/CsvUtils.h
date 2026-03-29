#ifndef INC_1PROJ_CSVUTILS_H
#define INC_1PROJ_CSVUTILS_H

#include <string>
#include <vector>

/**
 * @brief Helper functions for parsing CSV-like input lines.
 */
namespace CsvUtils {
    /**
     * @brief Removes leading and trailing whitespace from a string.
     * @param value String to normalize.
     * @return A trimmed copy of @p value.
     * @note Time complexity: O(n), where n is the length of @p value.
     */
    std::string trim(const std::string &value);

    /**
     * @brief Removes matching surrounding double quotes from a field.
     * @param value Raw CSV field.
     * @return The unquoted field when surrounding quotes exist; otherwise the
     * original value.
     * @note Time complexity: O(n), where n is the length of @p value.
     */
    std::string stripQuotes(const std::string &value);

    /**
     * @brief Splits a CSV line into fields while respecting quoted segments.
     * @param line Input line to split.
     * @return The extracted CSV fields.
     * @note Time complexity: O(n), where n is the length of @p line.
     */
    std::vector<std::string> splitCsvLine(const std::string &line);

    /**
     * @brief Attempts to parse an integer from a string.
     * @param value Source text.
     * @param result Parsed integer on success.
     * @return `true` when the value is a valid integer; `false` otherwise.
     * @note Time complexity: O(n), where n is the length of @p value.
     */
    bool tryParseInteger(const std::string &value, int &result);

    /**
     * @brief Parses an optional integer field.
     * @param value Source text that may be empty.
     * @param result Parsed integer on success.
     * @return `true` when the field is empty or contains a valid integer;
     * `false` otherwise.
     * @note Time complexity: O(n), where n is the length of @p value.
     */
    bool parseOptionalInteger(const std::string &value, int &result);

    /**
     * @brief Removes an inline comment from a CSV line.
     * @param line Raw input line.
     * @return The line content without the trailing inline comment.
     * @note Time complexity: O(n), where n is the length of @p line.
     */
    std::string removeInlineComment(const std::string &line);
}

#endif // INC_1PROJ_CSVUTILS_H
