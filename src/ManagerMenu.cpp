/*
 * ManagerMenu.cpp
 * All screens available to a logged-in Manager.
 * Covers: customers, vehicles, policies, claims (approve/reject),
 *         inspections, workshops, staff management, and reports.
 */

#include "Menus.h"
#include "UIUtils.h"
#include "FileManager.h"
#include <iostream>
#include <iomanip>

using namespace std;

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
ManagerMenu::ManagerMenu(AuthService& auth,
                         CustomerService& cs, VehicleService& vs,
                         PolicyService& ps, ClaimService& cls,
                         InspectionService& ins, WorkshopService& ws,
                         StaffService& ss, ReportService& rs)
    : auth_(auth), customerSvc_(cs), vehicleSvc_(vs),
      policySvc_(ps), claimSvc_(cls), inspSvc_(ins),
      workshopSvc_(ws), staffSvc_(ss), reportSvc_(rs) {}

// ---------------------------------------------------------------------------
// Main menu loop
// ---------------------------------------------------------------------------
void ManagerMenu::show() {
    int choice = 0;
    do {
        UI::clearScreen();
        UI::printBanner("MANAGER MENU — " + auth_.getCurrentUser().name);
        cout << "  1. Manage Customers\n"
             << "  2. Manage Vehicles\n"
             << "  3. Manage Policies\n"
             << "  4. Manage Claims\n"
             << "  5. Manage Workshops\n"
             << "  6. Manage Staff\n"
             << "  7. Reports\n"
             << "  0. Logout\n";
        UI::printLine();
        choice = UI::promptInt("Choice");

        switch (choice) {
            case 1: manageCustomers(); break;
            case 2: manageVehicles();  break;
            case 3: managePolicies();  break;
            case 4: manageClaims();    break;
            case 5: manageWorkshops(); break;
            case 6: manageStaff();     break;
            case 7: viewReports();     break;
            case 0: UI::printInfo("Logging out..."); break;
            default: UI::printError("Invalid choice."); UI::pause();
        }
    } while (choice != 0);
}

// ---------------------------------------------------------------------------
// Customer management
// ---------------------------------------------------------------------------
void ManagerMenu::manageCustomers() {
    int choice = 0;
    do {
        UI::clearScreen();
        UI::printBanner("MANAGE CUSTOMERS");
        cout << "  1. View All Customers\n"
             << "  2. Search Customer by ID\n"
             << "  0. Back\n";
        UI::printLine();
        choice = UI::promptInt("Choice");

        if (choice == 1) {
            auto list = customerSvc_.getAllCustomers();
            UI::clearScreen();
            UI::printBanner("ALL CUSTOMERS");
            cout << left << setw(15) << "ID"
                 << setw(22) << "Name"
                 << setw(14) << "Phone"
                 << setw(20) << "Email"
                 << "Registered\n";
            UI::printLine();
            for (const auto& c : list) {
                cout << setw(15) << c.customerId
                     << setw(22) << c.name
                     << setw(14) << c.phone
                     << setw(20) << c.email
                     << c.dateRegistered << "\n";
            }
            if (list.empty()) UI::printInfo("No customers found.");
            UI::pause();
        }
        else if (choice == 2) {
            string id = UI::prompt("Enter Customer ID");
            Customer c = customerSvc_.getCustomer(id);
            if (c.customerId.empty()) {
                UI::printError("Customer not found.");
            } else {
                UI::clearScreen();
                UI::printBanner("CUSTOMER DETAIL");
                cout << "  ID       : " << c.customerId     << "\n"
                     << "  Name     : " << c.name           << "\n"
                     << "  Phone    : " << c.phone          << "\n"
                     << "  Email    : " << c.email          << "\n"
                     << "  Address  : " << c.address        << "\n"
                     << "  Joined   : " << c.dateRegistered << "\n";
            }
            UI::pause();
        }
    } while (choice != 0);
}

// ---------------------------------------------------------------------------
// Vehicle management
// ---------------------------------------------------------------------------
void ManagerMenu::manageVehicles() {
    UI::clearScreen();
    UI::printBanner("ALL VEHICLES");
    auto list = vehicleSvc_.getAllVehicles();
    cout << left << setw(15) << "VehicleID"
         << setw(15) << "CustomerID"
         << setw(10) << "Make"
         << setw(12) << "Model"
         << setw(6)  << "Year"
         << "Plate\n";
    UI::printLine();
    for (const auto& v : list) {
        cout << setw(15) << v.vehicleId
             << setw(15) << v.customerId
             << setw(10) << v.make
             << setw(12) << v.model
             << setw(6)  << v.year
             << v.licensePlate << "\n";
    }
    if (list.empty()) UI::printInfo("No vehicles found.");
    UI::pause();
}

// ---------------------------------------------------------------------------
// Policy management
// ---------------------------------------------------------------------------
void ManagerMenu::managePolicies() {
    int choice = 0;
    do {
        UI::clearScreen();
        UI::printBanner("MANAGE POLICIES");
        cout << "  1. View All Policies\n"
             << "  2. Cancel a Policy\n"
             << "  0. Back\n";
        UI::printLine();
        choice = UI::promptInt("Choice");

        if (choice == 1) {
            auto list = policySvc_.getAllPolicies();
            UI::clearScreen();
            UI::printBanner("ALL POLICIES");
            cout << left << setw(18) << "PolicyID"
                 << setw(15) << "CustomerID"
                 << setw(12) << "Start"
                 << setw(12) << "End"
                 << setw(14) << "Coverage"
                 << "Status\n";
            UI::printLine();
            for (const auto& p : list) {
                cout << setw(18) << p.policyId
                     << setw(15) << p.customerId
                     << setw(12) << p.startDate
                     << setw(12) << p.endDate
                     << setw(14) << p.coverageType
                     << p.status << "\n";
            }
            if (list.empty()) UI::printInfo("No policies found.");
            UI::pause();
        }
        else if (choice == 2) {
            string id = UI::prompt("Enter Policy ID to cancel");
            if (policySvc_.cancelPolicy(id))
                UI::printSuccess("Policy cancelled.");
            else
                UI::printError("Policy not found.");
            UI::pause();
        }
    } while (choice != 0);
}

// ---------------------------------------------------------------------------
// Claim management (approve, reject, assign)
// ---------------------------------------------------------------------------
void ManagerMenu::manageClaims() {
    int choice = 0;
    do {
        UI::clearScreen();
        UI::printBanner("MANAGE CLAIMS");
        cout << "  1. View All Claims\n"
             << "  2. View Pending Claims\n"
             << "  3. Assign Surveyor to Claim\n"
             << "  4. Assign Workshop to Claim\n"
             << "  5. Approve Claim\n"
             << "  6. Reject Claim\n"
             << "  7. View Inspection Report for Claim\n"
             << "  8. View Customer Claim History\n"
             << "  0. Back\n";
        UI::printLine();
        choice = UI::promptInt("Choice");

        switch (choice) {
            case 1: {
                auto list = claimSvc_.getAllClaims();
                UI::clearScreen();
                UI::printBanner("ALL CLAIMS");
                cout << left << setw(18) << "ClaimID"
                     << setw(15) << "CustomerID"
                     << setw(12) << "Date Filed"
                     << setw(12) << "Est. Amt"
                     << "Status\n";
                UI::printLine();
                for (const auto& c : list) {
                    cout << setw(18) << c.claimId
                         << setw(15) << c.customerId
                         << setw(12) << c.dateFiled
                         << setw(12) << fixed << setprecision(2) << c.estimatedAmount
                         << c.status << "\n";
                }
                if (list.empty()) UI::printInfo("No claims found.");
                UI::pause();
                break;
            }
            case 2: viewPendingClaims();         break;
            case 3: assignSurveyorToClaim();     break;
            case 4: assignWorkshopToClaim();     break;
            case 5: approveClaim();              break;
            case 6: rejectClaim();               break;
            case 7: viewInspectionReport();      break;
            case 8: viewCustomerClaimHistory();  break;
            case 0: break;
            default: UI::printError("Invalid choice."); UI::pause();
        }
    } while (choice != 0);
}

void ManagerMenu::viewPendingClaims() {
    auto list = claimSvc_.getPendingClaims();
    UI::clearScreen();
    UI::printBanner("PENDING CLAIMS");
    cout << left << setw(18) << "ClaimID"
         << setw(15) << "CustomerID"
         << setw(12) << "Date Filed"
         << "Description\n";
    UI::printLine();
    for (const auto& c : list) {
        cout << setw(18) << c.claimId
             << setw(15) << c.customerId
             << setw(12) << c.dateFiled
             << c.description.substr(0, 30) << "\n";
    }
    if (list.empty()) UI::printInfo("No pending claims.");
    UI::pause();
}

void ManagerMenu::assignSurveyorToClaim() {
    string claimId    = UI::prompt("Claim ID");
    // Show available surveyors
    auto surveyors = staffSvc_.getStaffByRole("Surveyor");
    cout << "\n  Available Surveyors:\n";
    for (const auto& s : surveyors)
        cout << "    " << s.staffId << " - " << s.name << "\n";
    string surveyorId = UI::prompt("Surveyor ID");
    if (claimSvc_.assignSurveyor(claimId, surveyorId))
        UI::printSuccess("Surveyor assigned. Claim status set to 'Under Review'.");
    else
        UI::printError("Claim not found.");
    UI::pause();
}

void ManagerMenu::assignWorkshopToClaim() {
    string claimId = UI::prompt("Claim ID");
    // Show active workshops
    auto workshops = workshopSvc_.getActiveWorkshops();
    cout << "\n  Registered Active Workshops:\n";
    for (const auto& w : workshops)
        cout << "    " << w.workshopId << " - " << w.name << "\n";
    string workshopId = UI::prompt("Workshop ID");
    if (claimSvc_.assignWorkshop(claimId, workshopId))
        UI::printSuccess("Workshop assigned to claim.");
    else
        UI::printError("Claim not found.");
    UI::pause();
}

void ManagerMenu::approveClaim() {
    string claimId = UI::prompt("Claim ID to approve");
    Claim c = claimSvc_.getClaim(claimId);
    if (c.claimId.empty()) { UI::printError("Claim not found."); UI::pause(); return; }

    // Show inspection report first
    Inspection ins = inspSvc_.getInspectionByClaim(claimId);
    if (ins.inspectionId.empty()) {
        UI::printError("No inspection report found for this claim. Cannot approve.");
        UI::pause(); return;
    }
    UI::clearScreen();
    UI::printBanner("INSPECTION REPORT");
    cout << "  Claim ID     : " << ins.claimId             << "\n"
         << "  Surveyor     : " << ins.surveyorId          << "\n"
         << "  Date         : " << ins.inspectionDate      << "\n"
         << "  Findings     : " << ins.findings            << "\n"
         << "  Est. Cost    : " << fixed << setprecision(2) << ins.estimatedRepairCost << "\n"
         << "  Recommendation: " << ins.recommendation     << "\n";
    UI::printLine();

    double amount = UI::promptDouble("Approved amount (PKR)");
    if (claimSvc_.approveClaim(claimId, amount))
        UI::printSuccess("Claim approved.");
    else
        UI::printError("Failed to approve claim.");
    UI::pause();
}

void ManagerMenu::rejectClaim() {
    string claimId = UI::prompt("Claim ID to reject");
    if (claimSvc_.rejectClaim(claimId))
        UI::printSuccess("Claim rejected.");
    else
        UI::printError("Claim not found.");
    UI::pause();
}

void ManagerMenu::viewInspectionReport() {
    string claimId = UI::prompt("Claim ID");
    Inspection ins = inspSvc_.getInspectionByClaim(claimId);
    if (ins.inspectionId.empty()) {
        UI::printError("No inspection found for this claim.");
    } else {
        UI::clearScreen();
        UI::printBanner("INSPECTION REPORT");
        cout << "  Inspection ID : " << ins.inspectionId        << "\n"
             << "  Claim ID      : " << ins.claimId             << "\n"
             << "  Surveyor ID   : " << ins.surveyorId          << "\n"
             << "  Date          : " << ins.inspectionDate      << "\n"
             << "  Findings      : " << ins.findings            << "\n"
             << "  Est. Repair   : PKR " << fixed << setprecision(2) << ins.estimatedRepairCost << "\n"
             << "  Recommendation: " << ins.recommendation      << "\n"
             << "  Submitted     : " << ins.reportSubmitted     << "\n";
    }
    UI::pause();
}

void ManagerMenu::viewCustomerClaimHistory() {
    string customerId = UI::prompt("Customer ID");
    auto list = claimSvc_.getClaimsByCustomer(customerId);
    UI::clearScreen();
    UI::printBanner("CLAIM HISTORY — " + customerId);
    cout << left << setw(18) << "ClaimID"
         << setw(12) << "Filed"
         << setw(12) << "Resolved"
         << setw(12) << "Approved"
         << "Status\n";
    UI::printLine();
    for (const auto& c : list) {
        cout << setw(18) << c.claimId
             << setw(12) << c.dateFiled
             << setw(12) << (c.dateResolved.empty() ? "-" : c.dateResolved)
             << setw(12) << fixed << setprecision(2) << c.approvedAmount
             << c.status << "\n";
    }
    if (list.empty()) UI::printInfo("No claims found for this customer.");
    UI::pause();
}

// ---------------------------------------------------------------------------
// Workshop management
// ---------------------------------------------------------------------------
void ManagerMenu::manageWorkshops() {
    int choice = 0;
    do {
        UI::clearScreen();
        UI::printBanner("MANAGE WORKSHOPS");
        cout << "  1. View All Workshops\n"
             << "  2. Add Workshop\n"
             << "  3. Deactivate Workshop\n"
             << "  0. Back\n";
        UI::printLine();
        choice = UI::promptInt("Choice");

        if (choice == 1) {
            auto list = workshopSvc_.getAllWorkshops();
            UI::clearScreen();
            UI::printBanner("ALL WORKSHOPS");
            cout << left << setw(15) << "ID"
                 << setw(25) << "Name"
                 << setw(14) << "Phone"
                 << "Status\n";
            UI::printLine();
            for (const auto& w : list) {
                cout << setw(15) << w.workshopId
                     << setw(25) << w.name
                     << setw(14) << w.phone
                     << w.status << "\n";
            }
            if (list.empty()) UI::printInfo("No workshops registered.");
            UI::pause();
        }
        else if (choice == 2) {
            Workshop w;
            w.workshopId     = FileManager::generateId("W");
            w.name           = UI::prompt("Workshop Name");
            w.address        = UI::prompt("Address");
            w.phone          = UI::prompt("Phone");
            w.registrationNo = UI::prompt("Registration Number");
            w.status         = "Active";
            if (workshopSvc_.addWorkshop(w))
                UI::printSuccess("Workshop registered. ID: " + w.workshopId);
            else
                UI::printError("Failed to add workshop.");
            UI::pause();
        }
        else if (choice == 3) {
            string id = UI::prompt("Workshop ID to deactivate");
            if (workshopSvc_.deactivateWorkshop(id))
                UI::printSuccess("Workshop deactivated.");
            else
                UI::printError("Workshop not found.");
            UI::pause();
        }
    } while (choice != 0);
}

// ---------------------------------------------------------------------------
// Staff management
// ---------------------------------------------------------------------------
void ManagerMenu::manageStaff() {
    int choice = 0;
    do {
        UI::clearScreen();
        UI::printBanner("MANAGE STAFF");
        cout << "  1. View All Staff\n"
             << "  2. Add Staff Member\n"
             << "  3. Remove Staff Member\n"
             << "  0. Back\n";
        UI::printLine();
        choice = UI::promptInt("Choice");

        if (choice == 1) {
            auto list = staffSvc_.getAllStaff();
            UI::clearScreen();
            UI::printBanner("ALL STAFF");
            cout << left << setw(15) << "ID"
                 << setw(22) << "Name"
                 << setw(12) << "Role"
                 << "Username\n";
            UI::printLine();
            for (const auto& s : list) {
                cout << setw(15) << s.staffId
                     << setw(22) << s.name
                     << setw(12) << s.role
                     << s.username << "\n";
            }
            if (list.empty()) UI::printInfo("No staff found.");
            UI::pause();
        }
        else if (choice == 2) {
            Staff s;
            s.staffId    = FileManager::generateId("S");
            s.name       = UI::prompt("Full Name");
            s.phone      = UI::prompt("Phone");
            s.email      = UI::prompt("Email");
            cout << "  Role (1=Manager, 2=Salesman, 3=Surveyor): ";
            int r = UI::promptInt("Role");
            if      (r == 1) s.role = "Manager";
            else if (r == 2) s.role = "Salesman";
            else             s.role = "Surveyor";
            s.username   = UI::prompt("Username");
            s.password   = UI::prompt("Password");
            s.dateJoined = UI::today();
            if (staffSvc_.addStaff(s))
                UI::printSuccess("Staff added. ID: " + s.staffId);
            else
                UI::printError("Failed (username may already exist).");
            UI::pause();
        }
        else if (choice == 3) {
            string id = UI::prompt("Staff ID to remove");
            if (staffSvc_.removeStaff(id))
                UI::printSuccess("Staff member removed.");
            else
                UI::printError("Staff not found.");
            UI::pause();
        }
    } while (choice != 0);
}

// ---------------------------------------------------------------------------
// Reports
// ---------------------------------------------------------------------------
void ManagerMenu::viewReports() {
    int choice = 0;
    do {
        UI::clearScreen();
        UI::printBanner("REPORTS");
        cout << "  1. New Customers This Month\n"
             << "  2. Pending Claims\n"
             << "  3. All Inspection Reports\n"
             << "  4. Customer Claim History\n"
             << "  5. Policies Expiring Soon (30 days)\n"
             << "  6. System Summary\n"
             << "  0. Back\n";
        UI::printLine();
        choice = UI::promptInt("Choice");

        switch (choice) {
            case 1: viewNewCustomers();   break;
            case 2: viewPendingClaims();  break;
            case 3: {
                auto list = reportSvc_.allInspectionReports();
                UI::clearScreen();
                UI::printBanner("ALL INSPECTION REPORTS");
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
                if (list.empty()) UI::printInfo("No inspection reports.");
                UI::pause();
                break;
            }
            case 4: viewCustomerClaimHistory(); break;
            case 5: viewExpiringSoon();  break;
            case 6: viewSummary();       break;
            case 0: break;
            default: UI::printError("Invalid choice."); UI::pause();
        }
    } while (choice != 0);
}

void ManagerMenu::viewNewCustomers() {
    string yearMonth = UI::prompt("Enter month (YYYY-MM)");
    auto list = reportSvc_.newCustomersInMonth(yearMonth);
    UI::clearScreen();
    UI::printBanner("NEW CUSTOMERS IN " + yearMonth);
    for (const auto& c : list)
        cout << "  " << c.customerId << "  " << c.name << "  (" << c.dateRegistered << ")\n";
    if (list.empty()) UI::printInfo("No new customers in that month.");
    UI::pause();
}

void ManagerMenu::viewExpiringSoon() {
    auto list = reportSvc_.expiringPolicies(30);
    UI::clearScreen();
    UI::printBanner("POLICIES EXPIRING IN 30 DAYS");
    cout << left << setw(18) << "PolicyID"
         << setw(15) << "CustomerID"
         << setw(12) << "End Date"
         << "Coverage\n";
    UI::printLine();
    for (const auto& p : list) {
        cout << setw(18) << p.policyId
             << setw(15) << p.customerId
             << setw(12) << p.endDate
             << p.coverageType << "\n";
    }
    if (list.empty()) UI::printInfo("No policies expiring within 30 days.");
    UI::pause();
}

void ManagerMenu::viewSummary() {
    UI::clearScreen();
    cout << reportSvc_.summaryReport();
    UI::pause();
}
