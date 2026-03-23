//
// Created by conduto on 3/12/26.
//

#ifndef MENU_H
#define MENU_H

/**
 * @brief Handles menu rendering and validated numeric input in the CLI.
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
     * @return A valid integer entered by the user.
     * @time_complexity O(k), where k is the number of invalid attempts before
     * a valid integer is entered.
     */
    static int getSafeInteger();

    /**
     * @brief Waits for the user to confirm returning to the main menu.
     * @time_complexity O(k), where k is the number of attempts until the user
     * enters the expected confirmation value.
     */
    static void waitForReturnToMenu();
};

#endif // MENU_H
