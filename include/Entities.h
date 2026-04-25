#ifndef ENTITIES_H
#define ENTITIES_H

/*
 * Entities.h
 * Plain data structures (POD-style structs) representing all core domain
 * objects in the Auto Insurance Information System.
 */

#include <string>
#include <vector>

using namespace std;

// ---------------------------------------------------------------------------
// Customer — a person who owns a policy with this company
// ---------------------------------------------------------------------------
struct Customer {
    string customerId;
    string name;
    string phone;
    string email;
    string address;
    string dateRegistered;   // format: YYYY-MM-DD
};

// ---------------------------------------------------------------------------
// Vehicle — a car/bike registered under a customer
// ---------------------------------------------------------------------------
struct Vehicle {
    string vehicleId;
    string customerId;       // owner
    string make;             // e.g. Toyota
    string model;            // e.g. Corolla
    string year;             // e.g. 2020
    string licensePlate;
    string engineNo;
    string chassisNo;
};

// ---------------------------------------------------------------------------
// InsurancePolicy — contract between company and customer for a vehicle
// ---------------------------------------------------------------------------
struct InsurancePolicy {
    string policyId;
    string customerId;
    string vehicleId;
    string startDate;
    string endDate;
    double premiumAmount;
    string coverageType;     // "Comprehensive" | "Third-Party"
    string status;           // "Active" | "Expired" | "Cancelled"
};

// ---------------------------------------------------------------------------
// Claim — filed when an insured vehicle suffers damage
// ---------------------------------------------------------------------------
struct Claim {
    string claimId;
    string policyId;
    string customerId;
    string vehicleId;
    string dateOfAccident;
    string description;
    double estimatedAmount;
    double approvedAmount;
    string status;           // "Pending" | "Under Review" | "Approved" | "Rejected"
    string surveyorId;       // assigned staff member
    string workshopId;       // where the car will be repaired
    string dateFiled;
    string dateResolved;
};

// ---------------------------------------------------------------------------
// Inspection — report submitted by a surveyor after examining the damage
// ---------------------------------------------------------------------------
struct Inspection {
    string inspectionId;
    string claimId;
    string surveyorId;
    string inspectionDate;
    string findings;
    double estimatedRepairCost;
    string recommendation;   // "Approve" | "Reject" | "Further Review"
    string reportSubmitted;  // "Yes" | "No"
};

// ---------------------------------------------------------------------------
// Workshop — a registered repair facility authorised by the company
// ---------------------------------------------------------------------------
struct Workshop {
    string workshopId;
    string name;
    string address;
    string phone;
    string registrationNo;
    string status;           // "Active" | "Inactive"
};

// ---------------------------------------------------------------------------
// Staff — employee of the company (Manager / Salesman / Surveyor)
// ---------------------------------------------------------------------------
struct Staff {
    string staffId;
    string name;
    string phone;
    string email;
    string role;             // "Manager" | "Salesman" | "Surveyor"
    string username;
    string password;         // stored as plain text (demo only)
    string dateJoined;
};

#endif // ENTITIES_H
