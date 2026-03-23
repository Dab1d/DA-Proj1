#include "logic/ParameterViewer.h"

#include <iostream>
#include "logic/DataActionUtils.h"

using std::cout;
void ParameterViewer::printParameters(const LoadedConferenceData &data) {
    if (!DataActionUtils::ensureDataLoaded(data)) {
        return;
    }

    const Parameters &params = data.parameters;
    cout << "\nCurrent parameters\n";
    cout << "MinReviewsPerSubmission: " << params.minReviewsPerSubmission << "\n";
    cout << "MaxReviewsPerReviewer: " << params.maxReviewsPerReviewer << "\n";
    cout << "PrimaryReviewerExpertise: " << params.primaryReviewerExpertise << "\n";
    cout << "SecondaryReviewerExpertise: " << params.secondaryReviewerExpertise << "\n";
    cout << "PrimarySubmissionDomain: " << params.primarySubmissionDomain << "\n";
    cout << "SecondarySubmissionDomain: " << params.secondarySubmissionDomain << "\n";
    cout << "GenerateAssignments: " << params.generateAssignments << "\n";
    cout << "RiskAnalysis: " << params.riskAnalysis << "\n";
    cout << "OutputFileName: " << params.outputFileName << "\n";
}
