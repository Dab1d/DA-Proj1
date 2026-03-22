#ifndef INC_1PROJ_REVIEWERVIEWER_H
#define INC_1PROJ_REVIEWERVIEWER_H

#include "io/DataLoader.h"

/**
 * @brief Displays loaded reviewers.
 */
class ReviewerViewer {
public:
    /**
     * @brief Displays all loaded reviewers.
     * @param data Loaded dataset.
     */
    static void listReviewers(const LoadedConferenceData &data);
};

#endif // INC_1PROJ_REVIEWERVIEWER_H
