#ifndef INC_1PROJ_DATAACTIONUTILS_H
#define INC_1PROJ_DATAACTIONUTILS_H

#include "io/DataLoader.h"

/**
 * @brief Shared helpers used by data-related interactive actions.
 */
class DataActionUtils {
public:
    /**
     * @brief Checks whether a dataset has already been loaded.
     * @param data Loaded dataset.
     * @return `true` if data is available; `false` otherwise.
     * @note Time complexity: O(1)
     */
    static bool ensureDataLoaded(const LoadedConferenceData &data);

    /**
     * @brief Resolves a user-provided dataset name to a readable file path.
     * @param filePath Path or short file name entered by the user.
     * @return Existing path when found, otherwise the original string.
     * @note Time complexity: O(1), assuming a constant number of filesystem
     * checks.
     */
    static std::string resolveInputFilePath(const std::string &filePath);

    /**
     * @brief Returns a short display name for a loaded source file.
     * @param sourceFile Original stored source file path.
     * @return File name without parent directories when possible.
     * @note Time complexity: O(p), where p is the length of @p sourceFile.
     */
    static std::string getDisplayFileName(const std::string &sourceFile);

    /**
     * @brief Prints a topic/expertise field, handling optional values.
     * @param topic Topic value or -1 when absent.
     * @note Time complexity: O(1)
     */
    static void printTopic(int topic);
};

#endif // INC_1PROJ_DATAACTIONUTILS_H
