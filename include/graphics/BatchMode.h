#ifndef INC_1PROJ_BATCHMODE_H
#define INC_1PROJ_BATCHMODE_H

#include <string>

/**
 * @brief Executes the application in non-interactive batch mode.
 */
class BatchMode {
public:
    /**
     * @brief Loads an input dataset and reports the batch-mode summary.
     * @param inputFile Path to the input dataset.
     * @param outputFile Output filename requested by the user.
     * @time_complexity O(L + S log S + R log R), where L is the number of
     * lines in the file, S is the number of submissions, and R is the number
     * of reviewers.
     */
    static void run(const std::string &inputFile, const std::string &outputFile);
};

#endif // INC_1PROJ_BATCHMODE_H
