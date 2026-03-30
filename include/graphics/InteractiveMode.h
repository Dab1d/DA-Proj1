#ifndef INC_1PROJ_INTERACTIVEMODE_H
#define INC_1PROJ_INTERACTIVEMODE_H

/**
 * @brief Runs the interactive command-line workflow.
 */
class InteractiveMode {
public:
    /**
     * @brief Runs the interactive menu loop.
     * @note Time complexity: O(m + delegated action costs), where m is the
     * number of menu iterations performed by the user.
     */
    static void run();
};

#endif // INC_1PROJ_INTERACTIVEMODE_H
