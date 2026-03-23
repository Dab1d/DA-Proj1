#ifndef INC_1PROJ_SUBMISSION_H
#define INC_1PROJ_SUBMISSION_H

#include <string>

/**
 * @brief Represents one submission loaded from the dataset.
 */
struct Submission {
    /** @brief Submission identifier. */
    int id;
    /** @brief Submission title, author list, and contact email. */
    std::string title, authors, email;
    /** @brief Submission primary topic domain. */
    int primaryTopic;
    /** @brief Submission secondary topic domain, or `-1` when absent. */
    int secondaryTopic;
};

#endif // INC_1PROJ_SUBMISSION_H
