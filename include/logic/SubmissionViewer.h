#ifndef INC_1PROJ_SUBMISSIONVIEWER_H
#define INC_1PROJ_SUBMISSIONVIEWER_H

#include "io/DataLoader.h"

/**
 * @brief Displays the loaded submissions in interactive mode.
 */
class SubmissionViewer {
public:
    /**
     * @brief Displays all loaded submissions.
     * @param data Loaded dataset.
     * @note Time complexity: O(S), where S is the number of loaded submissions.
     */
    static void listSubmissions(const LoadedConferenceData &data);
};

#endif // INC_1PROJ_SUBMISSIONVIEWER_H
