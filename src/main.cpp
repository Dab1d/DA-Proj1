#include <iostream>
#include <string>

#include "graphics/BatchMode.h"
#include "graphics/InteractiveMode.h"

using std::cout;
using std::string;
int main(int argc, char *argv[]) {
    // Check for Batch Mode arguments
    if (argc > 1 && string(argv[1]) == "-b") {
        if (argc == 4) {
            BatchMode::run(argv[2], argv[3]);
        } else {
            cout << "Usage: " << argv[0] << " -b input.csv output.csv\n";
        }
        return 0;
    }

    InteractiveMode::run();
    return 0;
}
