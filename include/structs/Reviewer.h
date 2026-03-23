#ifndef INC_1PROJ_REVIEWER_H
#define INC_1PROJ_REVIEWER_H

#include <string>

/**
 * @brief Represents one reviewer loaded from the dataset.
 */
struct Reviewer {
    /** @brief Reviewer identifier. */
    int id;
    /** @brief Reviewer name. */
    std::string name, email;
    /** @brief Reviewer primary expertise domain. */
    int primaryExpertise;
    /** @brief Reviewer secondary expertise domain, or `-1` when absent. */
    int secondaryExpertise;
};

#endif // INC_1PROJ_REVIEWER_H
