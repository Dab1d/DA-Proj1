//
// Created by conduto on 3/12/26.
//
#include <iostream>
#include "Menu.h"

using std::cout;

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