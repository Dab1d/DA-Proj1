#ifndef INC_1PROJ_DATALOADER_H
#define INC_1PROJ_DATALOADER_H

#include <string>
#include <vector>
#include "../structs/Submission.h"
#include "../structs/Reviewer.h"
#include "../structs/Parameters.h"

/**
 * @brief Stores the parsed input data for a single conference instance.
 */
struct LoadedConferenceData {
    /** @brief Parsed submission entries. */
    std::vector<Submission> submissions;
    /** @brief Parsed reviewer entries. */
    std::vector<Reviewer> reviewers;
    /** @brief Parsed configuration and control values. */
    Parameters parameters{};
    /** @brief Original source file used to populate this instance. */
    std::string sourceFile;

    /**
     * @brief Checks whether any dataset has already been loaded.
     * @return `true` if a source file was successfully parsed, `false`
     * otherwise.
     * @note Time complexity: O(1)
     */
    bool isLoaded() const;
};

/**
 * @brief Loads and validates the project CSV input format.
 */
class DataLoader {
public:
    /**
     * @brief Parses a conference CSV file into in-memory structures.
     *
     * The parser validates mandatory sections, repeated identifiers and
     * parameter consistency, collecting all detected issues in @p errors.
     *
     * @param filePath Path to the input CSV file.
     * @param data Parsed output data on success.
     * @param errors Validation or parsing errors found while loading.
     * @return true if the file was successfully parsed and validated.
     * @note Time complexity: O(L + S log S + R log R), where L is the number
     * of lines in the input file, S is the number of loaded submissions, and R
     * is the number of loaded reviewers.
     */
    static bool loadFromCsv(const std::string &filePath,
                            LoadedConferenceData &data,
                            std::vector<std::string> &errors);
};

#endif // INC_1PROJ_DATALOADER_H
