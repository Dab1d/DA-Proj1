#ifndef INC_1PROJ_SUBMISSIONVIEWER_H
#define INC_1PROJ_SUBMISSIONVIEWER_H

#include "io/DataLoader.h"

/**
 * @brief Displays loaded submissions.
 */
class SubmissionViewer {
public:
    /**
     * @brief Displays all loaded submissions.
     * @param data Loaded dataset.
     */
    static void listSubmissions(const LoadedConferenceData &data);
};

#endif // INC_1PROJ_SUBMISSIONVIEWER_H
