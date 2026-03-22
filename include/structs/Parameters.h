#ifndef INC_1PROJ_PARAMETERS_H
#define INC_1PROJ_PARAMETERS_H
#include <string>

struct Parameters {
    int minReviewsPerSubmission;
    int maxReviewsPerReviewer;
    int primaryReviewerExpertise;
    int secondaryReviewerExpertise;
    int primarySubmissionDomain;
    int secondarySubmissionDomain;
    int generateAssignments; // 0=silent, 1=primary only, 2=primary+secondary sub, 3=all
    int riskAnalysis;        // 0=none, 1=single reviewer, K>1=K reviewers
    std::string outputFileName = "output.csv";
};

#endif //INC_1PROJ_PARAMETERS_H