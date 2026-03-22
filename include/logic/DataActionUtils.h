#ifndef INC_1PROJ_DATAACTIONUTILS_H
#define INC_1PROJ_DATAACTIONUTILS_H

#include "io/DataLoader.h"

/**
 * @brief Shared helpers for data-related logic actions.
 */
class DataActionUtils {
public:
    /**
     * @brief Checks whether a dataset has already been loaded.
     * @param data Loaded dataset.
     * @return true if data is available.
     */
    static bool ensureDataLoaded(const LoadedConferenceData &data);

    /**
     * @brief Prints a topic/expertise field, handling optional values.
     * @param topic Topic value or -1 when absent.
     */
    static void printTopic(int topic);
};

#endif // INC_1PROJ_DATAACTIONUTILS_H
