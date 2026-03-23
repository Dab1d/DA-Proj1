#ifndef INC_1PROJ_ERRORHANDLER_H
#define INC_1PROJ_ERRORHANDLER_H

#include <string>
#include <vector>

/**
 * @brief Centralizes console output for application errors.
 */
class ErrorHandler {
public:
    /**
     * @brief Prints the list of loading/parsing errors.
     * @param errors Collection of detected errors.
     * @time_complexity O(e), where e is the number of error messages printed.
     */
    static void printLoadErrors(const std::vector<std::string> &errors);
};

#endif // INC_1PROJ_ERRORHANDLER_H
