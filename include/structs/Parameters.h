#ifndef INC_1PROJ_PARAMETERS_H
#define INC_1PROJ_PARAMETERS_H

#include <string>

/**
 * @brief Stores the configuration and control values loaded from the input.
 */
struct Parameters {
    /** @brief Minimum reviews required for each submission. */
    int minReviewsPerSubmission;
    /** @brief Maximum reviews that can be assigned to each reviewer. */
    int maxReviewsPerReviewer;
    /** @brief Weight or enable flag for primary reviewer expertise. */
    int primaryReviewerExpertise;
    /** @brief Weight or enable flag for secondary reviewer expertise. */
    int secondaryReviewerExpertise;
    /** @brief Weight or enable flag for primary submission domain usage. */
    int primarySubmissionDomain;
    /** @brief Weight or enable flag for secondary submission domain usage. */
    int secondarySubmissionDomain;
    /** @brief Assignment mode: `0` silent, `1` primary only, `2` mixed, `3` all domains. */
    int generateAssignments;
    /** @brief Risk-analysis mode: `0` none, `1` single reviewer, `K > 1` grouped analysis. */
    int riskAnalysis;
    /** @brief Output file path requested by the instance configuration. */
    std::string outputFileName = "output.csv";
};

#endif // INC_1PROJ_PARAMETERS_H
