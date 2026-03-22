#include <iostream>
#include <string>
#include "graphics/BatchMode.h"
#include "graphics/InteractiveMode.h"

using std::cout;
using std::string;

/**
 * @file main.cpp
 * @brief Entry point for the Conference Assignment Tool.
 */

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
        if (argc > 3) {
            BatchMode::run(argv[2], argv[3]);
        } else {
            cout << "Usage: " << argv[0] << " -b input.csv output.csv\n";
        }
        return 0;
    }

    InteractiveMode::run();
    return 0;
}
