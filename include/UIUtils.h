#ifndef UI_UTILS_H
#define UI_UTILS_H

/*
 * UIUtils.h
 * Small text-UI helper functions: prompts, banners, table printing, etc.
 * Keeps display logic out of both the menus and the business layer.
 */

#include <string>
#include <vector>
using namespace std;

namespace UI {
    void        clearScreen();
    void        pause();                         // "Press Enter to continue..."
    void        printBanner(const string& title);
    void        printLine(char ch = '-', int width = 60);
    void        printSuccess(const string& msg);
    void        printError(const string& msg);
    void        printInfo(const string& msg);

    string      prompt(const string& label);
    double      promptDouble(const string& label);
    int         promptInt(const string& label);
    string      promptPassword(const string& label); // masks input with *

    // Returns today's date as YYYY-MM-DD
    string      today();

    // Returns true if str starts with prefix (used for month filtering)
    bool        startsWith(const string& str, const string& prefix);
}

#endif // UI_UTILS_H
