/*
 * SurveyorMenu.cpp
 * Screens for Surveyor role:
 *   - View claims assigned to this surveyor
 *   - Submit an inspection / damage report
 *   - View previously submitted inspection reports
 */

#include "Menus.h"
#include "UIUtils.h"
#include "FileManager.h"
#include <iostream>
#include <iomanip>

using namespace std;

SurveyorMenu::SurveyorMenu(AuthService& auth,
                            ClaimService& cls, InspectionService& ins,
                            VehicleService& vs, WorkshopService& ws)
    : auth_(auth), claimSvc_(cls), inspSvc_(ins),
      vehicleSvc_(vs), workshopSvc_(ws) {}

void SurveyorMenu::show() {
    int choice = 0;
    do {
        UI::clearScreen();
        UI::printBanner("SURVEYOR MENU — " + auth_.getCurrentUser().name);
        cout << "  1. View My Assigned Claims\n"
             << "  2. Submit Inspection Report\n"
             << "  3. View My Inspection Reports\n"
             << "  0. Logout\n";
        UI::printLine();
        choice = UI::promptInt("Choice");

        switch (choice) {
            case 1: viewMyAssignedClaims();    break;
            case 2: submitInspectionReport();  break;
            case 3: viewMyInspections();       break;
            case 0: UI::printInfo("Logging out..."); break;
            default: UI::printError("Invalid choice."); UI::pause();
        }
    } while (choice != 0);
}

void SurveyorMenu::viewMyAssignedClaims() {
    string myId = auth_.getCurrentUser().staffId;
    auto allClaims = claimSvc_.getAllClaims();

    UI::clearScreen();
    UI::printBanner("MY ASSIGNED CLAIMS");
    cout << left << setw(18) << "ClaimID"
         << setw(15) << "CustomerID"
         << setw(15) << "VehicleID"
         << setw(12) << "Date Filed"
         << "Status\n";
    UI::printLine();

    int count = 0;
    for (const auto& c : allClaims) {
        if (c.surveyorId == myId) {
            cout << setw(18) << c.claimId
                 << setw(15) << c.customerId
                 << setw(15) << c.vehicleId
                 << setw(12) << c.dateFiled
                 << c.status << "\n";
            ++count;
        }
    }
    if (count == 0) UI::printInfo("No claims assigned to you.");
    UI::pause();
}

void SurveyorMenu::submitInspectionReport() {
    UI::clearScreen();
    UI::printBanner("SUBMIT INSPECTION REPORT");

    string claimId = UI::prompt("Claim ID to inspect");
    Claim c = claimSvc_.getClaim(claimId);

    if (c.claimId.empty()) {
        UI::printError("Claim not found.");
        UI::pause(); return;
    }
    if (c.surveyorId != auth_.getCurrentUser().staffId) {
        UI::printError("This claim is not assigned to you.");
        UI::pause(); return;
    }
    // Prevent duplicate inspection report
    Inspection existing = inspSvc_.getInspectionByClaim(claimId);
    if (!existing.inspectionId.empty()) {
        UI::printError("An inspection report already exists for this claim.");
        UI::pause(); return;
    }

    // Show vehicle details for context
    Vehicle v = vehicleSvc_.getVehicle(c.vehicleId);
    cout << "\n  Vehicle: " << v.make << " " << v.model << " (" << v.year << ") — " << v.licensePlate << "\n";
    cout << "  Accident description: " << c.description << "\n\n";

    Inspection ins;
    ins.inspectionId        = FileManager::generateId("I");
    ins.claimId             = claimId;
    ins.surveyorId          = auth_.getCurrentUser().staffId;
    ins.inspectionDate      = UI::today();
    ins.findings            = UI::prompt("Findings / Observations");
    ins.estimatedRepairCost = UI::promptDouble("Estimated Repair Cost (PKR)");

    cout << "  Recommendation:\n"
         << "    1. Approve\n"
         << "    2. Reject\n"
         << "    3. Further Review\n";
    int rec = UI::promptInt("Choice");
    if      (rec == 1) ins.recommendation = "Approve";
    else if (rec == 2) ins.recommendation = "Reject";
    else               ins.recommendation = "Further Review";

    ins.reportSubmitted = "Yes";

    if (inspSvc_.submitInspection(ins))
        UI::printSuccess("Inspection report submitted. ID: " + ins.inspectionId);
    else
        UI::printError("Failed to submit report.");
    UI::pause();
}

void SurveyorMenu::viewMyInspections() {
    string myId = auth_.getCurrentUser().staffId;
    auto list   = inspSvc_.getInspectionsBySurveyor(myId);

    UI::clearScreen();
    UI::printBanner("MY INSPECTION REPORTS");
    cout << left << setw(18) << "InspectionID"
         << setw(18) << "ClaimID"
         << setw(12) << "Date"
         << setw(15) << "Est. Cost"
         << "Recommendation\n";
    UI::printLine();

    for (const auto& ins : list) {
        cout << setw(18) << ins.inspectionId
             << setw(18) << ins.claimId
             << setw(12) << ins.inspectionDate
             << setw(15) << fixed << setprecision(2) << ins.estimatedRepairCost
             << ins.recommendation << "\n";
    }
    if (list.empty()) UI::printInfo("No inspection reports found.");
    UI::pause();
}
