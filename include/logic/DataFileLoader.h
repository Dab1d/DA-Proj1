#ifndef INC_1PROJ_DATAFILELOADER_H
#define INC_1PROJ_DATAFILELOADER_H

#include "io/DataLoader.h"

/**
 * @brief Handles dataset loading requests in interactive mode.
 */
class DataFileLoader {
public:
    /**
     * @brief Loads a data file by asking the user for its path.
     * @param data Dataset storage updated on successful load.
     * @note Time complexity: O(L + S log S + R log R), where L is the number
     * of lines in the chosen file, S is the number of submissions, and R is
     * the number of reviewers.
     */
    static void loadDataFile(LoadedConferenceData &data);
};

#endif // INC_1PROJ_DATAFILELOADER_H
