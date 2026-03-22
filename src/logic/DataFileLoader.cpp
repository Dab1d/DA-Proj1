#include "logic/DataFileLoader.h"

#include <iostream>
#include <string>
#include <vector>
#include "io/ErrorHandler.h"

using std::cout;
using std::getline;
using std::string;
using std::vector;

/**
 * @brief Loads a data file via the interactive menu flow.
 * @param data Dataset storage updated on success.
 */
void DataFileLoader::loadDataFile(LoadedConferenceData &data) {
    cout << "Enter the CSV file path: ";

    string filePath;
    getline(std::cin, filePath);

    if (filePath.empty()) {
        cout << "No file path provided.\n";
        return;
    }

    LoadedConferenceData loadedData;
    vector<string> errors;
    if (!DataLoader::loadFromCsv(filePath, loadedData, errors)) {
        ErrorHandler::printLoadErrors(errors);
        return;
    }

    data = loadedData;
    cout << "File loaded successfully.\n";
    cout << "Submissions: " << data.submissions.size()
         << ", Reviewers: " << data.reviewers.size() << "\n";
}
