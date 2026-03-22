#ifndef INC_1PROJ_CONTROL_H
#define INC_1PROJ_CONTROL_H
#include <vector>
#include "../structs/Submission.h"
#include "../structs/Reviewer.h"
#include "../structs/Parameters.h"
#include "../structs/ConferenceData.h"

// Generates reviewer-submission assignments using max-flow.
// Mode is driven by params.generateAssignments:
//   1 = primary domains only
//   2 = primary+secondary submission domains, primary reviewer only
//   3 = all primary+secondary on both sides
std::vector<Assignment> generateAssignments(
    const std::vector<Submission> &submissions,
    const std::vector<Reviewer> &reviewers,
    const Parameters &params
);

// Returns submissions that couldn't meet minReviewsPerSubmission
std::vector<RiskEntry> analyzeRisk(
    const std::vector<Submission> &submissions,
    const std::vector<Assignment> &assignments,
    const Parameters &params
);

// Identifies "risky" reviewers whose removal breaks the assignment.
// k = params.riskAnalysis (1 = single reviewer, K>1 = K reviewers)
std::vector<int> analyzeReviewerRisk(
    const std::vector<Submission> &submissions,
    const std::vector<Reviewer> &reviewers,
    const std::vector<Assignment> &assignments,
    const Parameters &params
);

// Writes assignment + risk sections to params.outputFileName
void writeOutput(
    const std::vector<Assignment> &assignments,
    const std::vector<RiskEntry> &risks,
    const std::vector<int> &riskyReviewers,
    const Parameters &params
);

#endif //INC_1PROJ_CONTROL_H