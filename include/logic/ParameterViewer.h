#ifndef INC_1PROJ_PARAMETERVIEWER_H
#define INC_1PROJ_PARAMETERVIEWER_H

#include "io/DataLoader.h"

/**
 * @brief Displays loaded parameters.
 */
class ParameterViewer {
public:
    /**
     * @brief Displays the currently loaded parameters.
     * @param data Loaded dataset.
     */
    static void printParameters(const LoadedConferenceData &data);
};

#endif // INC_1PROJ_PARAMETERVIEWER_H
