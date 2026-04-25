#ifndef SERVICES_H
#define SERVICES_H

/*
 * Services.h
 * Business-Layer declarations — one class per domain entity/concern.
 * No file I/O here; everything goes through FileManager.
 */

#include "Entities.h"
#include <vector>
#include <string>

using namespace std;

// ===========================================================================
// CustomerService
// ===========================================================================
class CustomerService {
public:
    bool              addCustomer(const Customer& c);
    bool              updateCustomer(const Customer& c);
    bool              deleteCustomer(const string& id);
    Customer          getCustomer(const string& id) const;
    vector<Customer>  getAllCustomers() const;

    // Report: new customers registered in a given month (format "YYYY-MM")
    vector<Customer>  getNewCustomersInMonth(const string& yearMonth) const;
    bool              customerExists(const string& id) const;
};

// ===========================================================================
// VehicleService
// ===========================================================================
class VehicleService {
public:
    bool             addVehicle(const Vehicle& v);
    bool             updateVehicle(const Vehicle& v);
    bool             deleteVehicle(const string& id);
    Vehicle          getVehicle(const string& id) const;
    vector<Vehicle>  getAllVehicles() const;
    vector<Vehicle>  getVehiclesByCustomer(const string& customerId) const;
    bool             vehicleExists(const string& id) const;
};

// ===========================================================================
// PolicyService
// ===========================================================================
class PolicyService {
public:
    bool                    addPolicy(const InsurancePolicy& p);
    bool                    updatePolicy(const InsurancePolicy& p);
    bool                    cancelPolicy(const string& policyId);
    InsurancePolicy         getPolicy(const string& id) const;
    vector<InsurancePolicy> getAllPolicies() const;
    vector<InsurancePolicy> getPoliciesByCustomer(const string& customerId) const;
    vector<InsurancePolicy> getActivePolicies() const;
    bool                    policyExists(const string& id) const;
    bool                    isPolicyActive(const string& policyId) const;
};

// ===========================================================================
// ClaimService
// ===========================================================================
class ClaimService {
public:
    bool           fileClaim(const Claim& c);
    bool           updateClaim(const Claim& c);
    bool           assignSurveyor(const string& claimId, const string& surveyorId);
    bool           assignWorkshop(const string& claimId, const string& workshopId);
    bool           approveClaim(const string& claimId, double approvedAmount);
    bool           rejectClaim(const string& claimId);
    Claim          getClaim(const string& id) const;
    vector<Claim>  getAllClaims() const;
    vector<Claim>  getClaimsByCustomer(const string& customerId) const;
    vector<Claim>  getPendingClaims() const;
    vector<Claim>  getClaimsByStatus(const string& status) const;
    bool           claimExists(const string& id) const;
};

// ===========================================================================
// InspectionService
// ===========================================================================
class InspectionService {
public:
    bool               submitInspection(const Inspection& ins);
    bool               updateInspection(const Inspection& ins);
    Inspection         getInspection(const string& id) const;
    Inspection         getInspectionByClaim(const string& claimId) const;
    vector<Inspection> getAllInspections() const;
    vector<Inspection> getInspectionsBySurveyor(const string& surveyorId) const;
    bool               inspectionExists(const string& id) const;
};

// ===========================================================================
// WorkshopService
// ===========================================================================
class WorkshopService {
public:
    bool             addWorkshop(const Workshop& w);
    bool             updateWorkshop(const Workshop& w);
    bool             deactivateWorkshop(const string& id);
    Workshop         getWorkshop(const string& id) const;
    vector<Workshop> getAllWorkshops() const;
    vector<Workshop> getActiveWorkshops() const;
    bool             workshopExists(const string& id) const;
};

// ===========================================================================
// StaffService
// ===========================================================================
class StaffService {
public:
    bool          addStaff(const Staff& s);
    bool          updateStaff(const Staff& s);
    bool          removeStaff(const string& id);
    Staff         getStaff(const string& id) const;
    Staff         getStaffByUsername(const string& username) const;
    vector<Staff> getAllStaff() const;
    vector<Staff> getStaffByRole(const string& role) const;
    bool          staffExists(const string& id) const;
    bool          usernameExists(const string& username) const;
};

// ===========================================================================
// AuthService — tracks the currently logged-in user for the session
// ===========================================================================
class AuthService {
public:
    Staff login(const string& username, const string& password);
    bool  isLoggedIn() const    { return loggedIn_; }
    Staff getCurrentUser() const { return currentUser_; }
    void  logout()              { loggedIn_ = false; currentUser_ = Staff{}; }

private:
    Staff currentUser_;
    bool  loggedIn_ = false;
};

// ===========================================================================
// ReportService — aggregates data from multiple services for manager reports
// ===========================================================================
class ReportService {
public:
    vector<Customer>        newCustomersInMonth(const string& yearMonth) const;
    vector<Claim>           pendingClaims() const;
    vector<Inspection>      allInspectionReports() const;
    vector<Claim>           claimHistoryForCustomer(const string& customerId) const;
    vector<InsurancePolicy> expiringPolicies(int daysAhead) const;
    string                  summaryReport() const;
};

#endif // SERVICES_H
