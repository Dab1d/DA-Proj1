//
// Created by conduto on 3/12/26.
//
#include <iostream>
#include "Menu.h"
#include <limits>

using std::cout;
using std::cin;
using std::string;
using std::numeric_limits;
using std::streamsize;

/**
 * @brief Displays the main user interface menu to the console.
 * * Provides options for data loading, listing entities, and executing the
 * assignment algorithm.
 * * @time_complexity O(1)
 */
void Menu::showMenu() {
    std::cout << "\n";
    cout << "======================================" << "\n";
    cout << "      CONFERENCE ASSIGNMENT TOOL      " << "\n";
    cout << "======================================" << "\n";
    cout << "1. Load Data File (.csv)"               << "\n";
    cout << "2. List Submissions"                    << "\n";
    cout << "3. List Reviewers"                      << "\n";
    cout << "4. Check Parameters"                    << "\n";
    cout << "5. Run Assignment (Max-Flow)"           << "\n";
    cout << "0. Exit"                                << "\n";
    cout << "Selection: ";
}

/**
 * @brief Safely reads an integer from standard input with validation.
 * * Handles non-numeric input by clearing the error state and flushing
 * the input buffer to prevent infinite loops.
 * * @param prompt The message to display to the user.
 * @return int The valid integer entered by the user.
 */
int getSafeInteger() {
    int value;
    while (true) {
        cout << "Select an option: ";
        if (cin >> value) {
            // Success: clear the trailing newline to keep the buffer clean
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        }

        // Failure: handle non-numeric input
        cout << "Invalid input. Please enter a valid number.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}
