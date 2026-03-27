#include "logic/AssignmentRunner.h"

#include <iostream>
#include <vector>

#include "logic/Control.h"
#include "logic/DataActionUtils.h"
#include "logic/ReviewGraphExporter.h"

using std::cout;
using std::vector;

void AssignmentRunner::run(const LoadedConferenceData &data) {
    if (!DataActionUtils::ensureDataLoaded(data)) {
        return;
    }

    const vector<Assignment> assignments =
        generateAssignments(data.submissions, data.reviewers, data.parameters);
    const vector<RiskEntry> risks =
        analyzeRisk(data.submissions, assignments, data.parameters);
    const vector<int> riskyReviewers =
        analyzeReviewerRisk(data.submissions, data.reviewers, assignments, data.parameters);

    writeOutput(assignments, risks, riskyReviewers, data.parameters);

    if (ReviewGraphExporter::exportAssignmentStepsToGif(data, assignments, "assignment_process.gif")) {
        cout << "Assignment process GIF exported to assignment_process.gif\n";
    } else {
        cout << "Could not export assignment process GIF.\n";
    }

    cout << "Assignments generated: " << assignments.size() << "\n";
    cout << "Submissions still missing reviews: " << risks.size() << "\n";
    cout << "Risky reviewers: " << riskyReviewers.size() << "\n";
    cout << "Output written to " << data.parameters.outputFileName << "\n";
}
