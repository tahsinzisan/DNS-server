#include "../include/zone_file_reader.h"
#include <fstream>
#include <sstream>
#include <iostream>
using namespace std;

ZoneFileReader::ZoneFileReader(const std::string& zone_file_path) 
    : zone_file_path_(zone_file_path) {}

bool ZoneFileReader::load_zone_file() {
    ifstream file(zone_file_path_);
    if (!file.is_open()) {
        cerr << "Failed to open zone file: " << zone_file_path_ << endl;
        return false;
    }

    string line;
    while (getline(file, line)) {
        istringstream iss(line);
        string domain, record_type, internal_ip, external_ip;
        if (iss >> domain >> record_type >> internal_ip >> external_ip) {
            if (record_type == "IN" || record_type == "A") {
                // Store both internal and external IPs for each domain
                zone_records_[domain] = make_pair(internal_ip, external_ip);
            }
        }
    }

    file.close();
    return true;
}

string ZoneFileReader::get_ip_for_domain(string& domain, bool is_internal) {
    auto it = zone_records_.find(domain);
    if (it != zone_records_.end()) {
        // Return the internal or external IP based on the `is_internal` flag
        return is_internal ? it->second.first : it->second.second;
    }
    return "";  // Return empty string if the domain is not found
}
