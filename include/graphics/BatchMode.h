#ifndef INC_1PROJ_BATCHMODE_H
#define INC_1PROJ_BATCHMODE_H

#include <string>

/**
 * @brief Handles command-line batch execution.
 */
class BatchMode {
public:
    /**
     * @brief Loads the input CSV file and prepares batch execution.
     * @param inputFile Path to the input dataset.
     * @param outputFile Output filename requested by the user.
     */
    static void run(const std::string &inputFile, const std::string &outputFile);
};

#endif // INC_1PROJ_BATCHMODE_H
