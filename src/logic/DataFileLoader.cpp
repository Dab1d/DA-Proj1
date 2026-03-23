#include "logic/DataFileLoader.h"

#include <iostream>
#include <string>
#include <vector>
#include "io/ErrorHandler.h"
#include "logic/DataActionUtils.h"

using std::cout;
using std::getline;
using std::string;
using std::vector;
void DataFileLoader::loadDataFile(LoadedConferenceData &data) {
    cout << "Enter the CSV file name or path: ";

    string filePath;
    getline(std::cin, filePath);

    if (filePath.empty()) {
        cout << "No file path provided.\n";
        return;
    }

    LoadedConferenceData loadedData;
    vector<string> errors;
    const string resolvedFilePath = DataActionUtils::resolveInputFilePath(filePath);

    if (!DataLoader::loadFromCsv(resolvedFilePath, loadedData, errors)) {
        ErrorHandler::printLoadErrors(errors);
        return;
    }

    data = loadedData;
    cout << "File loaded successfully: "
         << DataActionUtils::getDisplayFileName(data.sourceFile) << "\n";
    cout << "Submissions: " << data.submissions.size()
         << ", Reviewers: " << data.reviewers.size() << "\n";
}
