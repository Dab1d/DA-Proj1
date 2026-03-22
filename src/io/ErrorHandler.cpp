#include "io/ErrorHandler.h"

#include <iostream>

using std::cout;
using std::string;
using std::vector;

/**
 * @brief Prints a list of parsing or validation errors.
 * @param errors Collection of detected loading errors.
 */
void ErrorHandler::printLoadErrors(const vector<string> &errors) {
    cout << "Failed to load file:\n";
    for (const string &error : errors) {
        cout << " - " << error << "\n";
    }
}
