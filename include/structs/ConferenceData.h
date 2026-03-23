#ifndef INC_1PROJ_CONFERENCEDATA_H
#define INC_1PROJ_CONFERENCEDATA_H

/**
 * @brief Represents one reviewer assignment for a submission.
 */
struct Assignment {
    /** @brief Assigned submission identifier. */
    int submissionId;
    /** @brief Assigned reviewer identifier. */
    int reviewerId;
    /** @brief Domain used from the submission side of the match. */
    int submissionDomain;
    /** @brief Domain used from the reviewer side of the match. */
    int reviewerDomain;
};

/**
 * @brief Represents a submission that still lacks required reviews.
 */
struct RiskEntry {
    /** @brief Submission identifier. */
    int submissionId;
    /** @brief Submission domain associated with the missing reviews. */
    int domain;
    /** @brief Number of reviews still missing. */
    int missingReviews;
};

#endif // INC_1PROJ_CONFERENCEDATA_H
