#include "logic/ReviewerViewer.h"

#include <iostream>
#include "logic/DataActionUtils.h"

using std::cout;

/**
 * @brief Displays all loaded reviewers.
 * @param data Loaded dataset.
 */
void ReviewerViewer::listReviewers(const LoadedConferenceData &data) {
    if (!DataActionUtils::ensureDataLoaded(data)) {
        return;
    }

    cout << "\nReviewers loaded from: " << data.sourceFile << "\n";
    cout << "-----------------------------------------------------------------\n";
    cout << "Id | Primary | Secondary | Name | Email\n";
    cout << "-----------------------------------------------------------------\n";

    for (const Reviewer &reviewer : data.reviewers) {
        cout << reviewer.id << " | "
             << reviewer.primaryExpertise << " | ";
        DataActionUtils::printTopic(reviewer.secondaryExpertise);
        cout << " | " << reviewer.name
             << " | " << reviewer.email << "\n";
    }
}
