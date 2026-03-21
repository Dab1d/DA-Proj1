//
// Created by conduto on 3/12/26.
//

#ifndef INC_1PROJ_DATALOADER_H
#define INC_1PROJ_DATALOADER_H
#include <string>
#include <vector>

using std::string;
using std::vector;

struct Assignment {
    int submissionId;
    int reviewerId;
    int match;
};

struct RiskEntry {
    int submissionId;
    int domain;
    int missingReviews;
};

vector<Submission> loadSubmissions(const string &path);

vector<Reviewer> loadReviewers(const string &path);

Parameters loadParameters(const string &path);

vector<Assignment> generateAssignments(
    const vector<Submission> &,
    const vector<Reviewer> &,
    const Parameters &
);

vector<RiskEntry> analyzeRisk(
    const vector<Submission> &,
    const vector<Assignment> &,
    const Parameters &
);


void writeOutput(
    const vector<Assignment> &,
    const vector<RiskEntry> &,
    const Parameters &
);
#endif //INC_1PROJ_DATALOADER_H
