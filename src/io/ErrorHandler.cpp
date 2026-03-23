#include "io/ErrorHandler.h"

#include <iostream>

using std::cout;
using std::string;
using std::vector;
void ErrorHandler::printLoadErrors(const vector<string> &errors) {
    cout << "Failed to load file:\n";
    for (const string &error : errors) {
        cout << " - " << error << "\n";
    }
}
