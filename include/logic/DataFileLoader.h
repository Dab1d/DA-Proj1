#ifndef INC_1PROJ_DATAFILELOADER_H
#define INC_1PROJ_DATAFILELOADER_H

#include "io/DataLoader.h"

/**
 * @brief Handles loading dataset files in interactive mode.
 */
class DataFileLoader {
public:
    /**
     * @brief Loads a data file by asking the user for its path.
     * @param data Dataset storage updated on successful load.
     */
    static void loadDataFile(LoadedConferenceData &data);
};

#endif // INC_1PROJ_DATAFILELOADER_H
