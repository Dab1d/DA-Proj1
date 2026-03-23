#include "logic/SubmissionViewer.h"

#include <iostream>
#include "logic/DataActionUtils.h"

using std::cout;
void SubmissionViewer::listSubmissions(const LoadedConferenceData &data) {
    if (!DataActionUtils::ensureDataLoaded(data)) {
        return;
    }

    cout << "\nSubmissions loaded from: "
         << DataActionUtils::getDisplayFileName(data.sourceFile) << "\n";
    cout << "--------------------------------------------------------------------------\n";
    cout << "Id | Primary | Secondary | Title | Authors | Email\n";
    cout << "--------------------------------------------------------------------------\n";

    for (const Submission &submission : data.submissions) {
        cout << submission.id << " | "
             << submission.primaryTopic << " | ";
        DataActionUtils::printTopic(submission.secondaryTopic);
        cout << " | " << submission.title
             << " | " << submission.authors
             << " | " << submission.email << "\n";
    }
}
