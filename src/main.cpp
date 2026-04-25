/*
 * main.cpp
 * Entry point and App class.
 * App bootstraps all services, seeds a default admin on first run,
 * and drives the login / role-dispatch loop.
 */

#include "Menus.h"
#include "UIUtils.h"
#include "FileManager.h"
#include <iostream>
#include <sys/stat.h>

#ifdef _WIN32
  #include <direct.h>
  #define MAKE_DIR(p) _mkdir(p)
#else
  #define MAKE_DIR(p) mkdir(p, 0755)
#endif

using namespace std;

// ---------------------------------------------------------------------------
// App::seedDefaultAdmin
// Creates a default Manager account if no staff file exists yet.
// Credentials: username=admin  password=admin123
// ---------------------------------------------------------------------------
void App::seedDefaultAdmin() {
    auto staff = FileManager::loadStaff();
    if (!staff.empty()) return;   // already seeded

    Staff mgr;
    mgr.staffId    = "S0001";
    mgr.name       = "System Administrator";
    mgr.phone      = "0000-0000000";
    mgr.email      = "admin@insure.com";
    mgr.role       = "Manager";
    mgr.username   = "admin";
    mgr.password   = "admin123";
    mgr.dateJoined = UI::today();

    staffSvc_.addStaff(mgr);
    cout << "\n  [INFO] Default admin created. Username: admin  Password: admin123\n";
}

// ---------------------------------------------------------------------------
// App::loginScreen
// Tries up to 3 times before returning to prevent brute-force attempts.
// ---------------------------------------------------------------------------
void App::loginScreen() {
    int attempts = 0;
    while (attempts < 3) {
        UI::clearScreen();
        UI::printBanner("AUTO INSURANCE INFORMATION SYSTEM");
        cout << "\n  Please log in to continue.\n\n";

        string username = UI::prompt("Username");
        string password = UI::promptPassword("Password");

        Staff user = auth_.login(username, password);
        if (!user.staffId.empty()) {
            UI::printSuccess("Welcome, " + user.name + "! [" + user.role + "]");
            UI::pause();

            // Route to the correct role menu
            if (user.role == "Manager") {
                ManagerMenu m(auth_, customerSvc_, vehicleSvc_, policySvc_,
                              claimSvc_, inspSvc_, workshopSvc_, staffSvc_, reportSvc_);
                m.show();
            } else if (user.role == "Salesman") {
                SalesmanMenu m(auth_, customerSvc_, vehicleSvc_, policySvc_, claimSvc_);
                m.show();
            } else if (user.role == "Surveyor") {
                SurveyorMenu m(auth_, claimSvc_, inspSvc_, vehicleSvc_, workshopSvc_);
                m.show();
            }
            return;
        }

        UI::printError("Invalid username or password.");
        ++attempts;
        if (attempts < 3) {
            cout << "  Attempts remaining: " << (3 - attempts) << "\n";
            UI::pause();
        }
    }
    UI::printError("Too many failed attempts. Exiting.");
}

// ---------------------------------------------------------------------------
// App::run — main application loop
// ---------------------------------------------------------------------------
void App::run() {
    // Ensure data directory exists
    MAKE_DIR("data");

    seedDefaultAdmin();

    // Keep showing the login screen until the user closes the program
    char again = 'y';
    while (again == 'y' || again == 'Y') {
        loginScreen();

        UI::clearScreen();
        UI::printBanner("SESSION ENDED");
        cout << "  Login again? (y/n): ";
        string line;
        getline(cin, line);
        again = line.empty() ? 'n' : line[0];
    }

    cout << "\n  Goodbye!\n\n";
}

// ---------------------------------------------------------------------------
// main
// ---------------------------------------------------------------------------
int main() {
    App app;
    app.run();
    return 0;
}
