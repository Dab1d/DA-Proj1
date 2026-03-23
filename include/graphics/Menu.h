//
// Created by conduto on 3/12/26.
//

#ifndef MENU_H
#define MENU_H

/**
 * @brief Class handling the User Interface and Input Validation.
 */
class Menu {
public:
    /**
     * @brief Displays the main menu options to the console.
     * @time_complexity O(1)
     */
    static void showMenu();

    /**
     * @brief Safely reads an integer from standard input with validation.
     * * This function prevents infinite loops by clearing the error state
     * and flushing the buffer if the user enters non-numeric characters.
     * @return int A valid integer provided by the user.
     */
    static int getSafeInteger();

    /**
     * @brief Waits for the user to confirm returning to the main menu.
     */
    static void waitForReturnToMenu();
};

#endif // MENU_H
