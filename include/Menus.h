#ifndef MENUS_H
#define MENUS_H

/*
 * Menus.h
 * One class per role (Manager / Salesman / Surveyor) plus a top-level App.
 * Each class holds service references injected at construction so menus
 * never instantiate their own services (no hidden coupling).
 */

#include "Services.h"

// ---------------------------------------------------------------------------
// ManagerMenu — full access: approvals, reports, workshops, staff
// ---------------------------------------------------------------------------
class ManagerMenu {
public:
    ManagerMenu(AuthService& auth,
                CustomerService& cs, VehicleService& vs,
                PolicyService& ps, ClaimService& cls,
                InspectionService& ins, WorkshopService& ws,
                StaffService& ss, ReportService& rs);
    void show();

private:
    AuthService&      auth_;
    CustomerService&  customerSvc_;
    VehicleService&   vehicleSvc_;
    PolicyService&    policySvc_;
    ClaimService&     claimSvc_;
    InspectionService& inspSvc_;
    WorkshopService&  workshopSvc_;
    StaffService&     staffSvc_;
    ReportService&    reportSvc_;

    // sub-menus
    void manageCustomers();
    void manageVehicles();
    void managePolicies();
    void manageClaims();
    void manageWorkshops();
    void manageStaff();
    void viewReports();

    // individual actions
    void approveClaim();
    void rejectClaim();
    void viewPendingClaims();
    void viewInspectionReport();
    void viewCustomerClaimHistory();
    void viewNewCustomers();
    void viewExpiringSoon();
    void viewSummary();
    void assignSurveyorToClaim();
    void assignWorkshopToClaim();
};

// ---------------------------------------------------------------------------
// SalesmanMenu — manages customers, vehicles, and policies
// ---------------------------------------------------------------------------
class SalesmanMenu {
public:
    SalesmanMenu(AuthService& auth,
                 CustomerService& cs, VehicleService& vs,
                 PolicyService& ps, ClaimService& cls);
    void show();

private:
    AuthService&     auth_;
    CustomerService& customerSvc_;
    VehicleService&  vehicleSvc_;
    PolicyService&   policySvc_;
    ClaimService&    claimSvc_;

    void addCustomer();
    void viewCustomers();
    void editCustomer();
    void addVehicle();
    void viewVehicles();
    void addPolicy();
    void viewPolicies();
    void fileClaim();
};

// ---------------------------------------------------------------------------
// SurveyorMenu — handles inspections assigned to this surveyor
// ---------------------------------------------------------------------------
class SurveyorMenu {
public:
    SurveyorMenu(AuthService& auth,
                 ClaimService& cls, InspectionService& ins,
                 VehicleService& vs, WorkshopService& ws);
    void show();

private:
    AuthService&      auth_;
    ClaimService&     claimSvc_;
    InspectionService& inspSvc_;
    VehicleService&   vehicleSvc_;
    WorkshopService&  workshopSvc_;

    void viewMyAssignedClaims();
    void submitInspectionReport();
    void viewMyInspections();
};

// ---------------------------------------------------------------------------
// App — bootstraps services and drives the login loop
// ---------------------------------------------------------------------------
class App {
public:
    void run();

private:
    // Service instances (owned by App for the whole session)
    AuthService       auth_;
    CustomerService   customerSvc_;
    VehicleService    vehicleSvc_;
    PolicyService     policySvc_;
    ClaimService      claimSvc_;
    InspectionService inspSvc_;
    WorkshopService   workshopSvc_;
    StaffService      staffSvc_;
    ReportService     reportSvc_;

    void loginScreen();
    void seedDefaultAdmin();   // creates a default manager account on first run
};

#endif // MENUS_H
