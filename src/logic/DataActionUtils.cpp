#include "logic/DataActionUtils.h"

#include <iostream>

using std::cout;

/**
 * @brief Checks whether data has already been loaded.
 * @param data Loaded dataset.
 * @return true if the dataset is available.
 */
bool DataActionUtils::ensureDataLoaded(const LoadedConferenceData &data) {
    if (data.isLoaded()) {
        return true;
    }

    cout << "No dataset loaded yet. Use option 1 first.\n";
    return false;
}

/**
 * @brief Prints one topic/expertise value.
 * @param topic Topic value, or -1 if absent.
 */
void DataActionUtils::printTopic(int topic) {
    if (topic == -1) {
        cout << "-";
    } else {
        cout << topic;
    }
}
