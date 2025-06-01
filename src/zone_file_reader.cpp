#include "../include/zone_file_reader.h"
#include <fstream>
#include <sstream>
#include <iostream>
using namespace std;


ZoneFileReader::ZoneFileReader(const std::string& filename) {
    zone_file_path_ = filename;
    
    // Debug output - shows EXACT path being used
    std::cout << "DEBUG: Trying to open zone file at: " 
              << std::filesystem::absolute(zone_file_path_) << std::endl;
}


bool ZoneFileReader::load_zone_file() {
    ifstream file(zone_file_path_);
    if (!file.is_open()) {
        cerr << "Failed to open zone file: " << zone_file_path_ << endl;
        return false;
    }

    string line;
    while (getline(file, line)) {
        istringstream iss(line);
        string domain, class_, record_type, internal_ip, external_ip;
        if (iss >> domain >> class_ >> record_type >> internal_ip >> external_ip) {
            if (!domain.empty() && domain.back() == '.') {
                domain.pop_back();  
            }
            if (class_ == "IN" && record_type == "A") {
                zone_records_[domain] = make_pair(internal_ip, external_ip);
            }
        }
    }   
    cerr << "success to open zone file: " <<  endl;
    cerr<<zone_records_.size()<<endl;
    file.close();
    return true;
}


string ZoneFileReader::get_ip_for_domain(string& domain, bool is_internal) {
    std::cerr << "returning ip for host" << std::endl;
    auto it = zone_records_.find(domain);
    if (it != zone_records_.end()) {
        // will return the internal or external IP based on the `is_internal` flag
        return is_internal ? it->second.first : it->second.second;
    }
    return "";  // Return empty string if the domain is not found
}
