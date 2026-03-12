#include <iostream>
#include <string>
#include <vector>
#include "Menu.h"

using std::cin;
using std::cout;
using std::string;

/**
 * @file main.cpp
 * @brief Entry point for the Conference Assignment Tool.
 */

int main(int argc, char *argv[]) {
    // T1.1: Batch Mode interface
    if (argc > 1 && string(argv[1]) == "-b") {
        cout << "Running in Batch Mode...\n";
        return 0;
    }

    int choice = 1;
    while (choice) {
        if (choice == 0) {
            cout << "Exiting...\n";
            break;
        }

        Menu::showMenu();

        if (!(cin >> choice)) {
            cout << "Invalid input. Please enter a number.\n";
            cin.clear();
            cin.ignore(10000, '\n');
            continue;
        }

        switch (choice) {
            case 1: {
                cout << "Load Data File (Not implemented)\n";
                break;
            }
            default:
                cout << "Option not available yet.\n";
                break;
        }
    }

    return 0;
}
