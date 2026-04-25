#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

/*
 * FileManager.h
 * Data-Access Layer — the ONLY place that touches the flat files.
 * Business services call these static helpers; they never open files directly.
 */

#include "Entities.h"
#include <vector>
#include <string>

using namespace std;

class FileManager {
public:
    // --- CRUD helpers for every entity type ---
    static vector<Customer>        loadCustomers();
    static void                    saveCustomers(const vector<Customer>& list);

    static vector<Vehicle>         loadVehicles();
    static void                    saveVehicles(const vector<Vehicle>& list);

    static vector<InsurancePolicy> loadPolicies();
    static void                    savePolicies(const vector<InsurancePolicy>& list);

    static vector<Claim>           loadClaims();
    static void                    saveClaims(const vector<Claim>& list);

    static vector<Inspection>      loadInspections();
    static void                    saveInspections(const vector<Inspection>& list);

    static vector<Workshop>        loadWorkshops();
    static void                    saveWorkshops(const vector<Workshop>& list);

    static vector<Staff>           loadStaff();
    static void                    saveStaff(const vector<Staff>& list);

    // Generates a unique ID with a given prefix, e.g. "C", "V", "P"
    static string                  generateId(const string& prefix);

private:
    static const string DATA_DIR;

    static string         filePath(const string& filename);
    static string         escape(const string& s);
    static string         unescape(const string& s);
    static vector<string> tokenize(const string& line, char delim);
};

#endif // FILE_MANAGER_H
