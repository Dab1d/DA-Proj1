#ifndef INC_1PROJ_REVIEWERVIEWER_H
#define INC_1PROJ_REVIEWERVIEWER_H

#include "io/DataLoader.h"

/**
 * @brief Displays the loaded reviewers in interactive mode.
 */
class ReviewerViewer {
public:
    /**
     * @brief Displays all loaded reviewers.
     * @param data Loaded dataset.
     * @time_complexity O(R), where R is the number of loaded reviewers.
     */
    static void listReviewers(const LoadedConferenceData &data);
};

#endif // INC_1PROJ_REVIEWERVIEWER_H
