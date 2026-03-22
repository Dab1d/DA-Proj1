#ifndef INC_1PROJ_ERRORHANDLER_H
#define INC_1PROJ_ERRORHANDLER_H

#include <string>
#include <vector>

/**
 * @brief Centralizes application error reporting.
 */
class ErrorHandler {
public:
    /**
     * @brief Prints the list of loading/parsing errors.
     * @param errors Collection of detected errors.
     */
    static void printLoadErrors(const std::vector<std::string> &errors);
};

#endif // INC_1PROJ_ERRORHANDLER_H
