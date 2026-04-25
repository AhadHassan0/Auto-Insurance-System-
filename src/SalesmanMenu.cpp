/*
 * SalesmanMenu.cpp
 * Screens for Salesman role:
 *   - Add / view / edit customers
 *   - Register vehicles
 *   - Issue policies
 *   - File initial claims on behalf of customers
 */

#include "Menus.h"
#include "UIUtils.h"
#include "FileManager.h"
#include <iostream>
#include <iomanip>

using namespace std;

SalesmanMenu::SalesmanMenu(AuthService& auth,
                           CustomerService& cs, VehicleService& vs,
                           PolicyService& ps, ClaimService& cls)
    : auth_(auth), customerSvc_(cs), vehicleSvc_(vs),
      policySvc_(ps), claimSvc_(cls) {}

void SalesmanMenu::show() {
    int choice = 0;
    do {
        UI::clearScreen();
        UI::printBanner("SALESMAN MENU — " + auth_.getCurrentUser().name);
        cout << "  1. Add Customer\n"
             << "  2. View Customers\n"
             << "  3. Edit Customer\n"
             << "  4. Register Vehicle\n"
             << "  5. View Vehicles\n"
             << "  6. Issue Policy\n"
             << "  7. View Policies\n"
             << "  8. File Claim\n"
             << "  0. Logout\n";
        UI::printLine();
        choice = UI::promptInt("Choice");

        switch (choice) {
            case 1: addCustomer();  break;
            case 2: viewCustomers(); break;
            case 3: editCustomer(); break;
            case 4: addVehicle();   break;
            case 5: viewVehicles(); break;
            case 6: addPolicy();    break;
            case 7: viewPolicies(); break;
            case 8: fileClaim();    break;
            case 0: UI::printInfo("Logging out..."); break;
            default: UI::printError("Invalid choice."); UI::pause();
        }
    } while (choice != 0);
}

void SalesmanMenu::addCustomer() {
    UI::clearScreen();
    UI::printBanner("ADD NEW CUSTOMER");

    Customer c;
    c.customerId     = FileManager::generateId("C");
    c.name           = UI::prompt("Full Name");
    c.phone          = UI::prompt("Phone Number");
    c.email          = UI::prompt("Email Address");
    c.address        = UI::prompt("Address");
    c.dateRegistered = UI::today();

    if (customerSvc_.addCustomer(c))
        UI::printSuccess("Customer registered. ID: " + c.customerId);
    else
        UI::printError("Failed to register customer.");
    UI::pause();
}

void SalesmanMenu::viewCustomers() {
    UI::clearScreen();
    UI::printBanner("ALL CUSTOMERS");
    auto list = customerSvc_.getAllCustomers();
    cout << left << setw(15) << "ID"
         << setw(25) << "Name"
         << setw(15) << "Phone"
         << "Registered\n";
    UI::printLine();
    for (const auto& c : list) {
        cout << setw(15) << c.customerId
             << setw(25) << c.name
             << setw(15) << c.phone
             << c.dateRegistered << "\n";
    }
    if (list.empty()) UI::printInfo("No customers found.");
    UI::pause();
}

void SalesmanMenu::editCustomer() {
    UI::clearScreen();
    UI::printBanner("EDIT CUSTOMER");
    string id = UI::prompt("Enter Customer ID to edit");
    Customer c = customerSvc_.getCustomer(id);

    if (c.customerId.empty()) {
        UI::printError("Customer not found.");
        UI::pause(); return;
    }

    cout << "  (Leave blank to keep existing value)\n\n";
    string name  = UI::prompt("Name [" + c.name + "]");
    string phone = UI::prompt("Phone [" + c.phone + "]");
    string email = UI::prompt("Email [" + c.email + "]");
    string addr  = UI::prompt("Address [" + c.address + "]");

    if (!name.empty())  c.name    = name;
    if (!phone.empty()) c.phone   = phone;
    if (!email.empty()) c.email   = email;
    if (!addr.empty())  c.address = addr;

    if (customerSvc_.updateCustomer(c))
        UI::printSuccess("Customer updated.");
    else
        UI::printError("Update failed.");
    UI::pause();
}

void SalesmanMenu::addVehicle() {
    UI::clearScreen();
    UI::printBanner("REGISTER VEHICLE");

    string customerId = UI::prompt("Customer ID");
    if (!customerSvc_.customerExists(customerId)) {
        UI::printError("Customer not found. Please add the customer first.");
        UI::pause(); return;
    }

    Vehicle v;
    v.vehicleId    = FileManager::generateId("V");
    v.customerId   = customerId;
    v.make         = UI::prompt("Make (e.g. Toyota)");
    v.model        = UI::prompt("Model (e.g. Corolla)");
    v.year         = UI::prompt("Year");
    v.licensePlate = UI::prompt("License Plate");
    v.engineNo     = UI::prompt("Engine Number");
    v.chassisNo    = UI::prompt("Chassis Number");

    if (vehicleSvc_.addVehicle(v))
        UI::printSuccess("Vehicle registered. ID: " + v.vehicleId);
    else
        UI::printError("Failed to register vehicle.");
    UI::pause();
}

void SalesmanMenu::viewVehicles() {
    UI::clearScreen();
    UI::printBanner("ALL VEHICLES");
    auto list = vehicleSvc_.getAllVehicles();
    cout << left << setw(15) << "VehicleID"
         << setw(15) << "CustomerID"
         << setw(12) << "Make"
         << setw(12) << "Model"
         << setw(6)  << "Year"
         << "Plate\n";
    UI::printLine();
    for (const auto& v : list) {
        cout << setw(15) << v.vehicleId
             << setw(15) << v.customerId
             << setw(12) << v.make
             << setw(12) << v.model
             << setw(6)  << v.year
             << v.licensePlate << "\n";
    }
    if (list.empty()) UI::printInfo("No vehicles found.");
    UI::pause();
}

void SalesmanMenu::addPolicy() {
    UI::clearScreen();
    UI::printBanner("ISSUE NEW POLICY");

    string customerId = UI::prompt("Customer ID");
    if (!customerSvc_.customerExists(customerId)) {
        UI::printError("Customer not found.");
        UI::pause(); return;
    }

    // Show customer's vehicles
    auto vehicles = vehicleSvc_.getVehiclesByCustomer(customerId);
    if (vehicles.empty()) {
        UI::printError("This customer has no registered vehicles. Register a vehicle first.");
        UI::pause(); return;
    }
    cout << "\n  Vehicles for this customer:\n";
    for (const auto& v : vehicles)
        cout << "    " << v.vehicleId << "  " << v.make << " " << v.model << "  " << v.licensePlate << "\n";

    InsurancePolicy p;
    p.policyId      = FileManager::generateId("P");
    p.customerId    = customerId;
    p.vehicleId     = UI::prompt("Vehicle ID");
    p.startDate     = UI::prompt("Start Date (YYYY-MM-DD)");
    p.endDate       = UI::prompt("End Date   (YYYY-MM-DD)");
    p.premiumAmount = UI::promptDouble("Premium Amount (PKR)");

    cout << "  Coverage Type:\n"
         << "    1. Comprehensive\n"
         << "    2. Third-Party\n";
    int ct = UI::promptInt("Choice");
    p.coverageType  = (ct == 1) ? "Comprehensive" : "Third-Party";
    p.status        = "Active";

    if (policySvc_.addPolicy(p))
        UI::printSuccess("Policy issued. ID: " + p.policyId);
    else
        UI::printError("Failed to issue policy.");
    UI::pause();
}

void SalesmanMenu::viewPolicies() {
    UI::clearScreen();
    UI::printBanner("ALL POLICIES");
    auto list = policySvc_.getAllPolicies();
    cout << left << setw(18) << "PolicyID"
         << setw(15) << "CustomerID"
         << setw(12) << "Start"
         << setw(12) << "End"
         << setw(16) << "Coverage"
         << "Status\n";
    UI::printLine();
    for (const auto& p : list) {
        cout << setw(18) << p.policyId
             << setw(15) << p.customerId
             << setw(12) << p.startDate
             << setw(12) << p.endDate
             << setw(16) << p.coverageType
             << p.status << "\n";
    }
    if (list.empty()) UI::printInfo("No policies found.");
    UI::pause();
}

void SalesmanMenu::fileClaim() {
    UI::clearScreen();
    UI::printBanner("FILE NEW CLAIM");

    string policyId = UI::prompt("Policy ID");
    if (!policySvc_.policyExists(policyId)) {
        UI::printError("Policy not found.");
        UI::pause(); return;
    }
    if (!policySvc_.isPolicyActive(policyId)) {
        UI::printError("Policy is not active. Cannot file a claim.");
        UI::pause(); return;
    }

    InsurancePolicy pol = policySvc_.getPolicy(policyId);

    Claim c;
    c.claimId         = FileManager::generateId("CL");
    c.policyId        = policyId;
    c.customerId      = pol.customerId;
    c.vehicleId       = pol.vehicleId;
    c.dateOfAccident  = UI::prompt("Date of Accident (YYYY-MM-DD)");
    c.description     = UI::prompt("Accident Description");
    c.estimatedAmount = UI::promptDouble("Estimated Damage Amount (PKR)");
    c.approvedAmount  = 0.0;
    c.status          = "Pending";
    c.surveyorId      = "";
    c.workshopId      = "";
    c.dateFiled       = UI::today();
    c.dateResolved    = "";

    if (claimSvc_.fileClaim(c))
        UI::printSuccess("Claim filed successfully. Claim ID: " + c.claimId);
    else
        UI::printError("Failed to file claim.");
    UI::pause();
}
