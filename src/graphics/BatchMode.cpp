#include "graphics/BatchMode.h"

#include <iostream>
#include <string>
#include <vector>
#include "logic/Control.h"
#include "logic/DataActionUtils.h"
#include "io/DataLoader.h"
#include "io/ErrorPrinter.h"

using std::cout;
using std::string;
using std::vector;
void BatchMode::run(const string &inputFile, const string &outputFile) {
    LoadedConferenceData data;
    vector<string> errors;
    const string resolvedInputFile = DataActionUtils::resolveInputFilePath(inputFile);

    if (!DataLoader::loadFromCsv(resolvedInputFile, data, errors)) {
        ErrorPrinter::printLoadErrors(errors);
        return;
    }

    data.parameters.outputFileName = outputFile;

    const vector<Assignment> assignments =
        generateAssignments(data.submissions, data.reviewers, data.parameters);
    const vector<RiskEntry> risks =
        analyzeRisk(data.submissions, assignments, data.parameters);
    const vector<int> riskyReviewers =
        analyzeReviewerRisk(data.submissions, data.reviewers, assignments, data.parameters);
    writeOutput(assignments, risks, riskyReviewers, data.parameters);

    cout << "Batch run successful.\n";
    cout << "Input: " << DataActionUtils::getDisplayFileName(data.sourceFile) << "\n";
    cout << "Submissions: " << data.submissions.size() << "\n";
    cout << "Reviewers: " << data.reviewers.size() << "\n";
    cout << "Assignments: " << assignments.size() << "\n";
    cout << "Missing-review submissions: " << risks.size() << "\n";
    cout << "Configured output file: " << data.parameters.outputFileName << "\n";
}
