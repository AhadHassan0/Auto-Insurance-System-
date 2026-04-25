/*
 * Services.cpp
 * Business-layer implementations.
 * Each service loads data, applies logic, and saves back through FileManager.
 * No cout/cin here — pure logic only.
 */

#include "Services.h"
#include "FileManager.h"
#include <algorithm>
#include <sstream>
#include <ctime>
#include <iomanip>

using namespace std;

// ============================================================================
// CustomerService
// ============================================================================

bool CustomerService::addCustomer(const Customer& c) {
    auto list = FileManager::loadCustomers();
    // Prevent duplicate IDs
    for (const auto& e : list)
        if (e.customerId == c.customerId) return false;
    list.push_back(c);
    FileManager::saveCustomers(list);
    return true;
}

bool CustomerService::updateCustomer(const Customer& c) {
    auto list = FileManager::loadCustomers();
    for (auto& e : list) {
        if (e.customerId == c.customerId) {
            e = c;
            FileManager::saveCustomers(list);
            return true;
        }
    }
    return false;
}

bool CustomerService::deleteCustomer(const string& id) {
    auto list = FileManager::loadCustomers();
    auto it = remove_if(list.begin(), list.end(),
        [&id](const Customer& c){ return c.customerId == id; });
    if (it == list.end()) return false;
    list.erase(it, list.end());
    FileManager::saveCustomers(list);
    return true;
}

Customer CustomerService::getCustomer(const string& id) const {
    for (const auto& c : FileManager::loadCustomers())
        if (c.customerId == id) return c;
    return Customer{};
}

vector<Customer> CustomerService::getAllCustomers() const {
    return FileManager::loadCustomers();
}

// Returns customers whose registration date starts with "YYYY-MM"
vector<Customer> CustomerService::getNewCustomersInMonth(const string& yearMonth) const {
    vector<Customer> result;
    for (const auto& c : FileManager::loadCustomers())
        if (c.dateRegistered.substr(0, 7) == yearMonth)
            result.push_back(c);
    return result;
}

bool CustomerService::customerExists(const string& id) const {
    for (const auto& c : FileManager::loadCustomers())
        if (c.customerId == id) return true;
    return false;
}

// ============================================================================
// VehicleService
// ============================================================================

bool VehicleService::addVehicle(const Vehicle& v) {
    auto list = FileManager::loadVehicles();
    for (const auto& e : list)
        if (e.vehicleId == v.vehicleId) return false;
    list.push_back(v);
    FileManager::saveVehicles(list);
    return true;
}

bool VehicleService::updateVehicle(const Vehicle& v) {
    auto list = FileManager::loadVehicles();
    for (auto& e : list) {
        if (e.vehicleId == v.vehicleId) {
            e = v;
            FileManager::saveVehicles(list);
            return true;
        }
    }
    return false;
}

bool VehicleService::deleteVehicle(const string& id) {
    auto list = FileManager::loadVehicles();
    auto it = remove_if(list.begin(), list.end(),
        [&id](const Vehicle& v){ return v.vehicleId == id; });
    if (it == list.end()) return false;
    list.erase(it, list.end());
    FileManager::saveVehicles(list);
    return true;
}

Vehicle VehicleService::getVehicle(const string& id) const {
    for (const auto& v : FileManager::loadVehicles())
        if (v.vehicleId == id) return v;
    return Vehicle{};
}

vector<Vehicle> VehicleService::getAllVehicles() const {
    return FileManager::loadVehicles();
}

vector<Vehicle> VehicleService::getVehiclesByCustomer(const string& customerId) const {
    vector<Vehicle> result;
    for (const auto& v : FileManager::loadVehicles())
        if (v.customerId == customerId) result.push_back(v);
    return result;
}

bool VehicleService::vehicleExists(const string& id) const {
    for (const auto& v : FileManager::loadVehicles())
        if (v.vehicleId == id) return true;
    return false;
}

// ============================================================================
// PolicyService
// ============================================================================

bool PolicyService::addPolicy(const InsurancePolicy& p) {
    auto list = FileManager::loadPolicies();
    for (const auto& e : list)
        if (e.policyId == p.policyId) return false;
    list.push_back(p);
    FileManager::savePolicies(list);
    return true;
}

bool PolicyService::updatePolicy(const InsurancePolicy& p) {
    auto list = FileManager::loadPolicies();
    for (auto& e : list) {
        if (e.policyId == p.policyId) {
            e = p;
            FileManager::savePolicies(list);
            return true;
        }
    }
    return false;
}

bool PolicyService::cancelPolicy(const string& policyId) {
    auto list = FileManager::loadPolicies();
    for (auto& e : list) {
        if (e.policyId == policyId) {
            e.status = "Cancelled";
            FileManager::savePolicies(list);
            return true;
        }
    }
    return false;
}

InsurancePolicy PolicyService::getPolicy(const string& id) const {
    for (const auto& p : FileManager::loadPolicies())
        if (p.policyId == id) return p;
    return InsurancePolicy{};
}

vector<InsurancePolicy> PolicyService::getAllPolicies() const {
    return FileManager::loadPolicies();
}

vector<InsurancePolicy> PolicyService::getPoliciesByCustomer(const string& customerId) const {
    vector<InsurancePolicy> result;
    for (const auto& p : FileManager::loadPolicies())
        if (p.customerId == customerId) result.push_back(p);
    return result;
}

vector<InsurancePolicy> PolicyService::getActivePolicies() const {
    vector<InsurancePolicy> result;
    for (const auto& p : FileManager::loadPolicies())
        if (p.status == "Active") result.push_back(p);
    return result;
}

bool PolicyService::policyExists(const string& id) const {
    for (const auto& p : FileManager::loadPolicies())
        if (p.policyId == id) return true;
    return false;
}

bool PolicyService::isPolicyActive(const string& policyId) const {
    for (const auto& p : FileManager::loadPolicies())
        if (p.policyId == policyId) return p.status == "Active";
    return false;
}

// ============================================================================
// ClaimService
// ============================================================================

bool ClaimService::fileClaim(const Claim& c) {
    auto list = FileManager::loadClaims();
    for (const auto& e : list)
        if (e.claimId == c.claimId) return false;
    list.push_back(c);
    FileManager::saveClaims(list);
    return true;
}

bool ClaimService::updateClaim(const Claim& c) {
    auto list = FileManager::loadClaims();
    for (auto& e : list) {
        if (e.claimId == c.claimId) {
            e = c;
            FileManager::saveClaims(list);
            return true;
        }
    }
    return false;
}

bool ClaimService::assignSurveyor(const string& claimId, const string& surveyorId) {
    auto list = FileManager::loadClaims();
    for (auto& e : list) {
        if (e.claimId == claimId) {
            e.surveyorId = surveyorId;
            e.status     = "Under Review";
            FileManager::saveClaims(list);
            return true;
        }
    }
    return false;
}

bool ClaimService::assignWorkshop(const string& claimId, const string& workshopId) {
    auto list = FileManager::loadClaims();
    for (auto& e : list) {
        if (e.claimId == claimId) {
            e.workshopId = workshopId;
            FileManager::saveClaims(list);
            return true;
        }
    }
    return false;
}

bool ClaimService::approveClaim(const string& claimId, double approvedAmount) {
    auto list = FileManager::loadClaims();
    for (auto& e : list) {
        if (e.claimId == claimId) {
            e.status         = "Approved";
            e.approvedAmount = approvedAmount;
            // Stamp today's date as resolved date
            time_t now = time(nullptr);
            char buf[11];
            strftime(buf, sizeof(buf), "%Y-%m-%d", localtime(&now));
            e.dateResolved = buf;
            FileManager::saveClaims(list);
            return true;
        }
    }
    return false;
}

bool ClaimService::rejectClaim(const string& claimId) {
    auto list = FileManager::loadClaims();
    for (auto& e : list) {
        if (e.claimId == claimId) {
            e.status = "Rejected";
            time_t now = time(nullptr);
            char buf[11];
            strftime(buf, sizeof(buf), "%Y-%m-%d", localtime(&now));
            e.dateResolved = buf;
            FileManager::saveClaims(list);
            return true;
        }
    }
    return false;
}

Claim ClaimService::getClaim(const string& id) const {
    for (const auto& c : FileManager::loadClaims())
        if (c.claimId == id) return c;
    return Claim{};
}

vector<Claim> ClaimService::getAllClaims() const {
    return FileManager::loadClaims();
}

vector<Claim> ClaimService::getClaimsByCustomer(const string& customerId) const {
    vector<Claim> result;
    for (const auto& c : FileManager::loadClaims())
        if (c.customerId == customerId) result.push_back(c);
    return result;
}

vector<Claim> ClaimService::getPendingClaims() const {
    return getClaimsByStatus("Pending");
}

vector<Claim> ClaimService::getClaimsByStatus(const string& status) const {
    vector<Claim> result;
    for (const auto& c : FileManager::loadClaims())
        if (c.status == status) result.push_back(c);
    return result;
}

bool ClaimService::claimExists(const string& id) const {
    for (const auto& c : FileManager::loadClaims())
        if (c.claimId == id) return true;
    return false;
}

// ============================================================================
// InspectionService
// ============================================================================

bool InspectionService::submitInspection(const Inspection& ins) {
    auto list = FileManager::loadInspections();
    for (const auto& e : list)
        if (e.inspectionId == ins.inspectionId) return false;
    list.push_back(ins);
    FileManager::saveInspections(list);
    return true;
}

bool InspectionService::updateInspection(const Inspection& ins) {
    auto list = FileManager::loadInspections();
    for (auto& e : list) {
        if (e.inspectionId == ins.inspectionId) {
            e = ins;
            FileManager::saveInspections(list);
            return true;
        }
    }
    return false;
}

Inspection InspectionService::getInspection(const string& id) const {
    for (const auto& ins : FileManager::loadInspections())
        if (ins.inspectionId == id) return ins;
    return Inspection{};
}

Inspection InspectionService::getInspectionByClaim(const string& claimId) const {
    for (const auto& ins : FileManager::loadInspections())
        if (ins.claimId == claimId) return ins;
    return Inspection{};
}

vector<Inspection> InspectionService::getAllInspections() const {
    return FileManager::loadInspections();
}

vector<Inspection> InspectionService::getInspectionsBySurveyor(const string& surveyorId) const {
    vector<Inspection> result;
    for (const auto& ins : FileManager::loadInspections())
        if (ins.surveyorId == surveyorId) result.push_back(ins);
    return result;
}

bool InspectionService::inspectionExists(const string& id) const {
    for (const auto& ins : FileManager::loadInspections())
        if (ins.inspectionId == id) return true;
    return false;
}

// ============================================================================
// WorkshopService
// ============================================================================

bool WorkshopService::addWorkshop(const Workshop& w) {
    auto list = FileManager::loadWorkshops();
    for (const auto& e : list)
        if (e.workshopId == w.workshopId) return false;
    list.push_back(w);
    FileManager::saveWorkshops(list);
    return true;
}

bool WorkshopService::updateWorkshop(const Workshop& w) {
    auto list = FileManager::loadWorkshops();
    for (auto& e : list) {
        if (e.workshopId == w.workshopId) {
            e = w;
            FileManager::saveWorkshops(list);
            return true;
        }
    }
    return false;
}

bool WorkshopService::deactivateWorkshop(const string& id) {
    auto list = FileManager::loadWorkshops();
    for (auto& e : list) {
        if (e.workshopId == id) {
            e.status = "Inactive";
            FileManager::saveWorkshops(list);
            return true;
        }
    }
    return false;
}

Workshop WorkshopService::getWorkshop(const string& id) const {
    for (const auto& w : FileManager::loadWorkshops())
        if (w.workshopId == id) return w;
    return Workshop{};
}

vector<Workshop> WorkshopService::getAllWorkshops() const {
    return FileManager::loadWorkshops();
}

vector<Workshop> WorkshopService::getActiveWorkshops() const {
    vector<Workshop> result;
    for (const auto& w : FileManager::loadWorkshops())
        if (w.status == "Active") result.push_back(w);
    return result;
}

bool WorkshopService::workshopExists(const string& id) const {
    for (const auto& w : FileManager::loadWorkshops())
        if (w.workshopId == id) return true;
    return false;
}

// ============================================================================
// StaffService
// ============================================================================

bool StaffService::addStaff(const Staff& s) {
    auto list = FileManager::loadStaff();
    for (const auto& e : list)
        if (e.staffId == s.staffId || e.username == s.username) return false;
    list.push_back(s);
    FileManager::saveStaff(list);
    return true;
}

bool StaffService::updateStaff(const Staff& s) {
    auto list = FileManager::loadStaff();
    for (auto& e : list) {
        if (e.staffId == s.staffId) {
            e = s;
            FileManager::saveStaff(list);
            return true;
        }
    }
    return false;
}

bool StaffService::removeStaff(const string& id) {
    auto list = FileManager::loadStaff();
    auto it = remove_if(list.begin(), list.end(),
        [&id](const Staff& s){ return s.staffId == id; });
    if (it == list.end()) return false;
    list.erase(it, list.end());
    FileManager::saveStaff(list);
    return true;
}

Staff StaffService::getStaff(const string& id) const {
    for (const auto& s : FileManager::loadStaff())
        if (s.staffId == id) return s;
    return Staff{};
}

Staff StaffService::getStaffByUsername(const string& username) const {
    for (const auto& s : FileManager::loadStaff())
        if (s.username == username) return s;
    return Staff{};
}

vector<Staff> StaffService::getAllStaff() const {
    return FileManager::loadStaff();
}

vector<Staff> StaffService::getStaffByRole(const string& role) const {
    vector<Staff> result;
    for (const auto& s : FileManager::loadStaff())
        if (s.role == role) result.push_back(s);
    return result;
}

bool StaffService::staffExists(const string& id) const {
    for (const auto& s : FileManager::loadStaff())
        if (s.staffId == id) return true;
    return false;
}

bool StaffService::usernameExists(const string& username) const {
    for (const auto& s : FileManager::loadStaff())
        if (s.username == username) return true;
    return false;
}

// ============================================================================
// AuthService
// ============================================================================

Staff AuthService::login(const string& username, const string& password) {
    StaffService ss;
    Staff s = ss.getStaffByUsername(username);
    if (!s.staffId.empty() && s.password == password) {
        currentUser_ = s;
        loggedIn_    = true;
        return s;
    }
    loggedIn_ = false;
    return Staff{};
}

// ============================================================================
// ReportService
// ============================================================================

vector<Customer> ReportService::newCustomersInMonth(const string& yearMonth) const {
    CustomerService cs;
    return cs.getNewCustomersInMonth(yearMonth);
}

vector<Claim> ReportService::pendingClaims() const {
    ClaimService cls;
    return cls.getPendingClaims();
}

vector<Inspection> ReportService::allInspectionReports() const {
    return FileManager::loadInspections();
}

vector<Claim> ReportService::claimHistoryForCustomer(const string& customerId) const {
    ClaimService cls;
    return cls.getClaimsByCustomer(customerId);
}

// Returns policies whose end date falls within the next `daysAhead` days
vector<InsurancePolicy> ReportService::expiringPolicies(int daysAhead) const {
    PolicyService ps;
    auto all = ps.getActivePolicies();
    vector<InsurancePolicy> result;

    time_t now = time(nullptr);
    time_t limit = now + static_cast<time_t>(daysAhead) * 86400;

    for (const auto& p : all) {
        // Parse end date YYYY-MM-DD
        tm t = {};
        istringstream ss(p.endDate);
        ss >> get_time(&t, "%Y-%m-%d");
        time_t endTime = mktime(&t);
        if (endTime >= now && endTime <= limit)
            result.push_back(p);
    }
    return result;
}

string ReportService::summaryReport() const {
    PolicyService  ps;
    ClaimService   cls;
    CustomerService cs;
    WorkshopService ws;

    ostringstream oss;
    oss << "\n========== SYSTEM SUMMARY REPORT ==========\n";
    oss << "Total Customers  : " << cs.getAllCustomers().size()   << "\n";
    oss << "Active Policies  : " << ps.getActivePolicies().size() << "\n";
    oss << "Pending Claims   : " << cls.getPendingClaims().size() << "\n";
    oss << "Total Claims     : " << cls.getAllClaims().size()      << "\n";
    oss << "Active Workshops : " << ws.getActiveWorkshops().size()<< "\n";
    oss << "============================================\n";
    return oss.str();
}
