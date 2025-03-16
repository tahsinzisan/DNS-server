#include "../include/dns_server.h"
#include "../include/zone_file_reader.h"
#include <boost/asio.hpp>
#include <iostream>
using namespace boost::asio;
using namespace std;

int main() {

    ZoneFileReader zone_reader("../zone.txt");
    if (!zone_reader.load_zone_file()) {
        std::cerr << "Failed to load zone file!" << std::endl;
        return -1; 
    }
    try {
        io_service io_service;
        DNS_Server server(io_service, 133, 133);  
        server.run();  // Start the server 
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
