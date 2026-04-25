/*
 * FileManager.cpp
 * Implements all flat-file read/write operations.
 * Format: one record per line, fields separated by '|'.
 * '|' and '\n' inside field values are escaped as \| and \n.
 */

#include "FileManager.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <ctime>

using namespace std;

const string FileManager::DATA_DIR = "data/";

// ---------------------------------------------------------------------------
// Internal helpers
// ---------------------------------------------------------------------------

string FileManager::filePath(const string& filename) {
    return DATA_DIR + filename;
}

// Escape pipe and newline chars so they don't break the delimiter scheme
string FileManager::escape(const string& s) {
    string out;
    out.reserve(s.size());
    for (char c : s) {
        if      (c == '|')  out += "\\|";
        else if (c == '\n') out += "\\n";
        else if (c == '\\') out += "\\\\";
        else                out += c;
    }
    return out;
}

string FileManager::unescape(const string& s) {
    string out;
    out.reserve(s.size());
    for (size_t i = 0; i < s.size(); ++i) {
        if (s[i] == '\\' && i + 1 < s.size()) {
            char next = s[i + 1];
            if      (next == '|')  { out += '|';  ++i; }
            else if (next == 'n')  { out += '\n'; ++i; }
            else if (next == '\\') { out += '\\'; ++i; }
            else                   { out += s[i]; }
        } else {
            out += s[i];
        }
    }
    return out;
}

// Split a line by delimiter, respecting backslash-escaping
vector<string> FileManager::tokenize(const string& line, char delim) {
    vector<string> tokens;
    string token;
    bool escaped = false;

    for (size_t i = 0; i < line.size(); ++i) {
        if (escaped) {
            token += line[i];
            escaped = false;
        } else if (line[i] == '\\') {
            token += line[i];
            escaped = true;
        } else if (line[i] == delim) {
            tokens.push_back(unescape(token));
            token.clear();
        } else {
            token += line[i];
        }
    }
    tokens.push_back(unescape(token));
    return tokens;
}

// Generates a unique ID: prefix + unix-timestamp + auto-incrementing counter
string FileManager::generateId(const string& prefix) {
    static int counter = 0;
    ++counter;
    time_t t = time(nullptr);
    return prefix + to_string(t) + to_string(counter);
}

// ---------------------------------------------------------------------------
// Customers
// ---------------------------------------------------------------------------

vector<Customer> FileManager::loadCustomers() {
    vector<Customer> list;
    ifstream fin(filePath("customers.dat"));
    string line;

    while (getline(fin, line)) {
        if (line.empty() || line[0] == '#') continue;
        auto t = tokenize(line, '|');
        if (t.size() < 6) continue;

        Customer c;
        c.customerId     = t[0];
        c.name           = t[1];
        c.phone          = t[2];
        c.email          = t[3];
        c.address        = t[4];
        c.dateRegistered = t[5];
        list.push_back(c);
    }
    return list;
}

void FileManager::saveCustomers(const vector<Customer>& list) {
    ofstream fout(filePath("customers.dat"));
    for (const auto& c : list) {
        fout << escape(c.customerId)     << '|'
             << escape(c.name)           << '|'
             << escape(c.phone)          << '|'
             << escape(c.email)          << '|'
             << escape(c.address)        << '|'
             << escape(c.dateRegistered) << '\n';
    }
}

// ---------------------------------------------------------------------------
// Vehicles
// ---------------------------------------------------------------------------

vector<Vehicle> FileManager::loadVehicles() {
    vector<Vehicle> list;
    ifstream fin(filePath("vehicles.dat"));
    string line;

    while (getline(fin, line)) {
        if (line.empty() || line[0] == '#') continue;
        auto t = tokenize(line, '|');
        if (t.size() < 8) continue;

        Vehicle v;
        v.vehicleId    = t[0];
        v.customerId   = t[1];
        v.make         = t[2];
        v.model        = t[3];
        v.year         = t[4];
        v.licensePlate = t[5];
        v.engineNo     = t[6];
        v.chassisNo    = t[7];
        list.push_back(v);
    }
    return list;
}

void FileManager::saveVehicles(const vector<Vehicle>& list) {
    ofstream fout(filePath("vehicles.dat"));
    for (const auto& v : list) {
        fout << escape(v.vehicleId)    << '|'
             << escape(v.customerId)   << '|'
             << escape(v.make)         << '|'
             << escape(v.model)        << '|'
             << escape(v.year)         << '|'
             << escape(v.licensePlate) << '|'
             << escape(v.engineNo)     << '|'
             << escape(v.chassisNo)    << '\n';
    }
}

// ---------------------------------------------------------------------------
// Policies
// ---------------------------------------------------------------------------

vector<InsurancePolicy> FileManager::loadPolicies() {
    vector<InsurancePolicy> list;
    ifstream fin(filePath("policies.dat"));
    string line;

    while (getline(fin, line)) {
        if (line.empty() || line[0] == '#') continue;
        auto t = tokenize(line, '|');
        if (t.size() < 8) continue;

        InsurancePolicy p;
        p.policyId      = t[0];
        p.customerId    = t[1];
        p.vehicleId     = t[2];
        p.startDate     = t[3];
        p.endDate       = t[4];
        p.premiumAmount = stod(t[5]);
        p.coverageType  = t[6];
        p.status        = t[7];
        list.push_back(p);
    }
    return list;
}

void FileManager::savePolicies(const vector<InsurancePolicy>& list) {
    ofstream fout(filePath("policies.dat"));
    for (const auto& p : list) {
        fout << escape(p.policyId)      << '|'
             << escape(p.customerId)    << '|'
             << escape(p.vehicleId)     << '|'
             << escape(p.startDate)     << '|'
             << escape(p.endDate)       << '|'
             << p.premiumAmount         << '|'
             << escape(p.coverageType)  << '|'
             << escape(p.status)        << '\n';
    }
}

// ---------------------------------------------------------------------------
// Claims
// ---------------------------------------------------------------------------

vector<Claim> FileManager::loadClaims() {
    vector<Claim> list;
    ifstream fin(filePath("claims.dat"));
    string line;

    while (getline(fin, line)) {
        if (line.empty() || line[0] == '#') continue;
        auto t = tokenize(line, '|');
        if (t.size() < 13) continue;

        Claim c;
        c.claimId         = t[0];
        c.policyId        = t[1];
        c.customerId      = t[2];
        c.vehicleId       = t[3];
        c.dateOfAccident  = t[4];
        c.description     = t[5];
        c.estimatedAmount = stod(t[6]);
        c.approvedAmount  = stod(t[7]);
        c.status          = t[8];
        c.surveyorId      = t[9];
        c.workshopId      = t[10];
        c.dateFiled       = t[11];
        c.dateResolved    = t[12];
        list.push_back(c);
    }
    return list;
}

void FileManager::saveClaims(const vector<Claim>& list) {
    ofstream fout(filePath("claims.dat"));
    for (const auto& c : list) {
        fout << escape(c.claimId)        << '|'
             << escape(c.policyId)       << '|'
             << escape(c.customerId)     << '|'
             << escape(c.vehicleId)      << '|'
             << escape(c.dateOfAccident) << '|'
             << escape(c.description)    << '|'
             << c.estimatedAmount        << '|'
             << c.approvedAmount         << '|'
             << escape(c.status)         << '|'
             << escape(c.surveyorId)     << '|'
             << escape(c.workshopId)     << '|'
             << escape(c.dateFiled)      << '|'
             << escape(c.dateResolved)   << '\n';
    }
}

// ---------------------------------------------------------------------------
// Inspections
// ---------------------------------------------------------------------------

vector<Inspection> FileManager::loadInspections() {
    vector<Inspection> list;
    ifstream fin(filePath("inspections.dat"));
    string line;

    while (getline(fin, line)) {
        if (line.empty() || line[0] == '#') continue;
        auto t = tokenize(line, '|');
        if (t.size() < 8) continue;

        Inspection ins;
        ins.inspectionId        = t[0];
        ins.claimId             = t[1];
        ins.surveyorId          = t[2];
        ins.inspectionDate      = t[3];
        ins.findings            = t[4];
        ins.estimatedRepairCost = stod(t[5]);
        ins.recommendation      = t[6];
        ins.reportSubmitted     = t[7];
        list.push_back(ins);
    }
    return list;
}

void FileManager::saveInspections(const vector<Inspection>& list) {
    ofstream fout(filePath("inspections.dat"));
    for (const auto& ins : list) {
        fout << escape(ins.inspectionId)        << '|'
             << escape(ins.claimId)             << '|'
             << escape(ins.surveyorId)          << '|'
             << escape(ins.inspectionDate)      << '|'
             << escape(ins.findings)            << '|'
             << ins.estimatedRepairCost         << '|'
             << escape(ins.recommendation)      << '|'
             << escape(ins.reportSubmitted)     << '\n';
    }
}

// ---------------------------------------------------------------------------
// Workshops
// ---------------------------------------------------------------------------

vector<Workshop> FileManager::loadWorkshops() {
    vector<Workshop> list;
    ifstream fin(filePath("workshops.dat"));
    string line;

    while (getline(fin, line)) {
        if (line.empty() || line[0] == '#') continue;
        auto t = tokenize(line, '|');
        if (t.size() < 6) continue;

        Workshop w;
        w.workshopId     = t[0];
        w.name           = t[1];
        w.address        = t[2];
        w.phone          = t[3];
        w.registrationNo = t[4];
        w.status         = t[5];
        list.push_back(w);
    }
    return list;
}

void FileManager::saveWorkshops(const vector<Workshop>& list) {
    ofstream fout(filePath("workshops.dat"));
    for (const auto& w : list) {
        fout << escape(w.workshopId)     << '|'
             << escape(w.name)           << '|'
             << escape(w.address)        << '|'
             << escape(w.phone)          << '|'
             << escape(w.registrationNo) << '|'
             << escape(w.status)         << '\n';
    }
}

// ---------------------------------------------------------------------------
// Staff
// ---------------------------------------------------------------------------

vector<Staff> FileManager::loadStaff() {
    vector<Staff> list;
    ifstream fin(filePath("staff.dat"));
    string line;

    while (getline(fin, line)) {
        if (line.empty() || line[0] == '#') continue;
        auto t = tokenize(line, '|');
        if (t.size() < 8) continue;

        Staff s;
        s.staffId    = t[0];
        s.name       = t[1];
        s.phone      = t[2];
        s.email      = t[3];
        s.role       = t[4];
        s.username   = t[5];
        s.password   = t[6];
        s.dateJoined = t[7];
        list.push_back(s);
    }
    return list;
}

void FileManager::saveStaff(const vector<Staff>& list) {
    ofstream fout(filePath("staff.dat"));
    for (const auto& s : list) {
        fout << escape(s.staffId)    << '|'
             << escape(s.name)       << '|'
             << escape(s.phone)      << '|'
             << escape(s.email)      << '|'
             << escape(s.role)       << '|'
             << escape(s.username)   << '|'
             << escape(s.password)   << '|'
             << escape(s.dateJoined) << '\n';
    }
}
