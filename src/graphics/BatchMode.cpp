#include "graphics/BatchMode.h"

#include <iostream>
#include <string>
#include <vector>
#include "logic/DataActionUtils.h"
#include "io/DataLoader.h"
#include "io/ErrorHandler.h"

using std::cout;
using std::string;
using std::vector;
void BatchMode::run(const string &inputFile, const string &outputFile) {
    LoadedConferenceData data;
    vector<string> errors;
    const string resolvedInputFile = DataActionUtils::resolveInputFilePath(inputFile);

    if (!DataLoader::loadFromCsv(resolvedInputFile, data, errors)) {
        ErrorHandler::printLoadErrors(errors);
        return;
    }

    data.parameters.outputFileName = outputFile;

    cout << "Batch load successful.\n";
    cout << "Input: " << DataActionUtils::getDisplayFileName(data.sourceFile) << "\n";
    cout << "Submissions: " << data.submissions.size() << "\n";
    cout << "Reviewers: " << data.reviewers.size() << "\n";
    cout << "Configured output file: " << data.parameters.outputFileName << "\n";
}
