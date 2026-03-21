#include <iostream>
#include <string>
#include "Menu.h"

using std::cin;
using std::cout;
using std::string;

/**
 * @file main.cpp
 * @brief Entry point for the Conference Assignment Tool.
 */

/**
 * @brief Processes a file in batch mode.
 *
 * @param filename The name of the file to be processed.
 */
void runBatchMode(const string &filename) {
    cout << "Running in Batch Mode with file: " << filename << "\n";
    // TODO: add file processing logic
}

/**
 * @brief Loads a data file via the interactive menu.
 */
void loadDataFile() {
    cout << "Load Data File (Not implemented)\n";
}

/**
 * @brief Main function of the program.
 *
 * Supports two execution modes:
 * - Interactive mode: Provides a CLI menu for user input.
 * - Batch mode: Triggered by using the "-b" flag followed by a filename.
 *
 * @param argc Number of command-line arguments.
 * @param argv Array of command-line arguments.
 * @return int Exit code (0 for success).
 */
int main(int argc, char *argv[]) {
    // Check for Batch Mode arguments
    if (argc > 1 && string(argv[1]) == "-b") {
        if (argc > 2) {
            runBatchMode(argv[2]);
        } else {
            cout << "Error: No file provided for batch mode.\n";
        }
        return 0;
    }
    while (true) {
        Menu::showMenu();

        int choice = Menu::getSafeInteger();

        if (choice == 0) {
            cout << "Exiting...\n";
            break;
        }

        switch (choice) {
            case 1:
                loadDataFile();
                break;
            default:
                cout << "Option not available yet.\n";
                break;
        }
    }

    return 0;
}
