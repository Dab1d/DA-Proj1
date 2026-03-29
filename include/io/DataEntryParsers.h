#ifndef INC_1PROJ_DATAENTRYPARSERS_H
#define INC_1PROJ_DATAENTRYPARSERS_H

#include <string>
#include <vector>
#include "structs/Reviewer.h"
#include "structs/Submission.h"

/**
 * @brief Parsers for submission and reviewer CSV entries.
 */
namespace DataEntryParsers {
    /**
     * @brief Parses one submission row from CSV fields.
     * @param fields CSV fields belonging to a submission entry.
     * @param submission Parsed submission on success.
     * @param error Human-readable error message on failure.
     * @return `true` if the row is valid; `false` otherwise.
     * @note Time complexity: O(f), where f is the total length of all strings
     * in @p fields.
     */
    bool parseSubmission(const std::vector<std::string> &fields,
                         Submission &submission,
                         std::string &error);

    /**
     * @brief Parses one reviewer row from CSV fields.
     * @param fields CSV fields belonging to a reviewer entry.
     * @param reviewer Parsed reviewer on success.
     * @param error Human-readable error message on failure.
     * @return `true` if the row is valid; `false` otherwise.
     * @note Time complexity: O(f), where f is the total length of all strings
     * in @p fields.
     */
    bool parseReviewer(const std::vector<std::string> &fields,
                       Reviewer &reviewer,
                       std::string &error);
}

#endif // INC_1PROJ_DATAENTRYPARSERS_H
