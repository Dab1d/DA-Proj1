#ifndef INC_1PROJ_ASSIGNMENTRUNNER_H
#define INC_1PROJ_ASSIGNMENTRUNNER_H

#include "io/DataLoader.h"

/**
 * @brief Runs the assignment algorithm for the currently loaded dataset.
 */
class AssignmentRunner {
public:
    /**
     * @brief Generates assignments, risk information, and output files.
     * @param data Loaded conference instance.
     * @time_complexity O(R * (R * S + F)), where R is the number of reviewers,
     * S is the number of submissions, and F is the max-flow cost on one run.
     */
    static void run(const LoadedConferenceData &data);
};

#endif // INC_1PROJ_ASSIGNMENTRUNNER_H
