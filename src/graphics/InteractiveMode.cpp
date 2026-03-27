#include "graphics/InteractiveMode.h"

#include <iostream>

#include "logic/AssignmentRunner.h"
#include "logic/DataFileLoader.h"
#include "logic/ParameterViewer.h"
#include "logic/ReviewGraphVisualizer.h"
#include "logic/ReviewerViewer.h"
#include "logic/SubmissionViewer.h"
#include "graphics/Menu.h"
#include "io/DataLoader.h"

using std::cout;
void InteractiveMode::run() {
    LoadedConferenceData data;

    while (true) {
        Menu::showMenu();

        int choice = Menu::getSafeInteger();
        if (choice == 0) break;


        switch (choice) {
            case 1:
                DataFileLoader::loadDataFile(data);
                break;
            case 2:
                SubmissionViewer::listSubmissions(data);
                break;
            case 3:
                ReviewerViewer::listReviewers(data);
                break;
            case 4:
                ParameterViewer::printParameters(data);
                break;
            case 5:
                ReviewGraphVisualizer::exportGraph(data);
                break;
            case 6:
                AssignmentRunner::run(data);
                break;
            default:
                cout << "Option not available yet.\n";
                break;
        }

        Menu::waitForReturnToMenu();

    }

    cout << "Exiting...\n";
}
