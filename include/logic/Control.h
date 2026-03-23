#ifndef INC_1PROJ_CONTROL_H
#define INC_1PROJ_CONTROL_H

#include <vector>
#include "../structs/Submission.h"
#include "../structs/Reviewer.h"
#include "../structs/Parameters.h"
#include "../structs/ConferenceData.h"

/**
 * @brief Generates reviewer assignments for the loaded conference instance.
 *
 * The intended behavior is controlled by `params.generateAssignments`:
 * - `1`: primary domains only
 * - `2`: primary and secondary submission domains, primary reviewer expertise
 * - `3`: primary and secondary domains on both sides
 *
 * @param submissions Loaded submissions.
 * @param reviewers Loaded reviewers.
 * @param params Assignment and control parameters.
 * @return The generated reviewer-submission assignments.
 * @time_complexity O(G), where G is the complexity of the concrete assignment
 * algorithm chosen by the implementation.
 */
std::vector<Assignment> generateAssignments(
    const std::vector<Submission> &submissions,
    const std::vector<Reviewer> &reviewers,
    const Parameters &params
);

/**
 * @brief Detects submissions that did not receive the minimum number of reviews.
 * @param submissions Loaded submissions.
 * @param assignments Generated assignments.
 * @param params Assignment and control parameters.
 * @return One risk entry per submission that is still missing reviews.
 * @time_complexity O(S + A), where S is the number of submissions and A is the
 * number of assignments analyzed.
 */
std::vector<RiskEntry> analyzeRisk(
    const std::vector<Submission> &submissions,
    const std::vector<Assignment> &assignments,
    const Parameters &params
);

/**
 * @brief Identifies reviewers whose removal makes the instance infeasible.
 *
 * The analysis mode is controlled by `params.riskAnalysis`, where `1` means
 * single-reviewer analysis and values greater than `1` represent grouped
 * reviewer-removal analysis.
 *
 * @param submissions Loaded submissions.
 * @param reviewers Loaded reviewers.
 * @param assignments Existing assignments.
 * @param params Assignment and control parameters.
 * @return The identifiers of risky reviewers.
 * @time_complexity O(R * G), where R is the number of reviewers and G is the
 * complexity of one reassignment attempt in the chosen implementation.
 */
std::vector<int> analyzeReviewerRisk(
    const std::vector<Submission> &submissions,
    const std::vector<Reviewer> &reviewers,
    const std::vector<Assignment> &assignments,
    const Parameters &params
);

/**
 * @brief Writes the generated results to the configured output file.
 * @param assignments Generated assignments.
 * @param risks Submissions still missing reviews.
 * @param riskyReviewers Reviewers classified as risky.
 * @param params Assignment and output parameters.
 * @time_complexity O(A + M + K), where A is the number of assignments, M is
 * the number of missing-review entries, and K is the number of risky
 * reviewers.
 */
void writeOutput(
    const std::vector<Assignment> &assignments,
    const std::vector<RiskEntry> &risks,
    const std::vector<int> &riskyReviewers,
    const Parameters &params
);

#endif // INC_1PROJ_CONTROL_H
