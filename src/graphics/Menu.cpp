//
// Created by conduto on 3/12/26.
//
#include <iostream>
#include <limits>
#include "graphics/Menu.h"

using std::cout;
using std::cin;
using std::numeric_limits;
using std::streamsize;
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
}

int Menu::getSafeInteger() {
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

void Menu::waitForReturnToMenu() {
    int value;

    while (true) {
        cout << "\nPress 42 to return to the menu: ";
        if (cin >> value) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            if (value == 42) {
                return;
            }
        } else {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        cout << "Invalid input. Please press 42 to return to the menu.\n";
    }
}
