/*
 * UIUtils.cpp
 * Small terminal-UI helpers used throughout all menus.
 */

#include "UIUtils.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <limits>
#include <ctime>

#ifdef _WIN32
  #include <conio.h>
  #include <direct.h>
#else
  #include <termios.h>
  #include <unistd.h>
  #include <sys/stat.h>
#endif

using namespace std;

namespace UI {

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void pause() {
    cout << "\n  Press Enter to continue...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void printLine(char ch, int width) {
    cout << string(width, ch) << "\n";
}

void printBanner(const string& title) {
    printLine('=');
    // Centre the title
    int padding = (60 - static_cast<int>(title.size())) / 2;
    if (padding > 0) cout << string(padding, ' ');
    cout << title << "\n";
    printLine('=');
}

void printSuccess(const string& msg) {
    cout << "\n  [OK]  " << msg << "\n";
}

void printError(const string& msg) {
    cout << "\n  [ERR] " << msg << "\n";
}

void printInfo(const string& msg) {
    cout << "  --> " << msg << "\n";
}

string prompt(const string& label) {
    cout << "  " << label << ": ";
    string val;
    getline(cin, val);
    return val;
}

double promptDouble(const string& label) {
    cout << "  " << label << ": ";
    double val = 0.0;
    string line;
    getline(cin, line);
    try { val = stod(line); } catch (...) { val = 0.0; }
    return val;
}

int promptInt(const string& label) {
    cout << "  " << label << ": ";
    int val = 0;
    string line;
    getline(cin, line);
    try { val = stoi(line); } catch (...) { val = 0; }
    return val;
}

// Masks input with '*' characters
string promptPassword(const string& label) {
    cout << "  " << label << ": ";
    string password;

#ifdef _WIN32
    char ch;
    while ((ch = (char)_getch()) != '\r') {
        if (ch == '\b' && !password.empty()) {
            cout << "\b \b";
            password.pop_back();
        } else if (ch != '\b') {
            password += ch;
            cout << '*';
        }
    }
    cout << '\n';
#else
    // POSIX: temporarily disable terminal echo
    termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    getline(cin, password);

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    cout << "\n";
#endif

    return password;
}

string today() {
    time_t now = time(nullptr);
    char buf[11];
    strftime(buf, sizeof(buf), "%Y-%m-%d", localtime(&now));
    return string(buf);
}

bool startsWith(const string& str, const string& prefix) {
    return str.size() >= prefix.size() &&
           str.compare(0, prefix.size(), prefix) == 0;
}

} // namespace UI
