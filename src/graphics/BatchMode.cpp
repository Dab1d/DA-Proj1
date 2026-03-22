#include "graphics/BatchMode.h"

#include <iostream>
#include <string>
#include <vector>
#include "io/DataLoader.h"
#include "io/ErrorHandler.h"

using std::cout;
using std::string;
using std::vector;

/**
 * @brief Loads the batch input file and reports the configured execution data.
 * @param inputFile Path to the input dataset.
 * @param outputFile Output filename requested by the user.
 */
void BatchMode::run(const string &inputFile, const string &outputFile) {
    LoadedConferenceData data;
    vector<string> errors;

    if (!DataLoader::loadFromCsv(inputFile, data, errors)) {
        ErrorHandler::printLoadErrors(errors);
        return;
    }

    data.parameters.outputFileName = outputFile;

    cout << "Batch load successful.\n";
    cout << "Input: " << inputFile << "\n";
    cout << "Submissions: " << data.submissions.size() << "\n";
    cout << "Reviewers: " << data.reviewers.size() << "\n";
    cout << "Configured output file: " << data.parameters.outputFileName << "\n";
}
