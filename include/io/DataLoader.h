#ifndef INC_1PROJ_DATALOADER_H
#define INC_1PROJ_DATALOADER_H

#include <string>
#include <vector>
#include "../structs/Submission.h"
#include "../structs/Reviewer.h"
#include "../structs/Parameters.h"

/**
 * @brief Stores the parsed input data for one conference instance.
 */
struct LoadedConferenceData {
    std::vector<Submission> submissions;
    std::vector<Reviewer> reviewers;
    Parameters parameters{};
    std::string sourceFile;

    /**
     * @brief Checks whether any dataset has already been loaded.
     * @return true if a source file was successfully parsed.
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
     */
    static bool loadFromCsv(const std::string &filePath,
                            LoadedConferenceData &data,
                            std::vector<std::string> &errors);
};

#endif // INC_1PROJ_DATALOADER_H
