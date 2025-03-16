#include "dns_response.h"
#include "dns_header.h"
#include "../include/zone_file_reader.h"
#include <sstream>
using namespace std;

string build_response(const string &query, string ip_address) {
  ostringstream response;

  // Add DNS Header
  // append_dns_header(response, query);
  response.write(query.data(), 2); // Transaction ID
  response << "\x81\x80";          // Flags (Standard response, no errors)
  response << "\x00\x01";          // QDCOUNT (1 question)
  response << "\x00\x01";          // ANCOUNT (1 answer)
  response << "\x00\x00";          // NSCOUNT (0)
  response << "\x00\x00";          // ARCOUNT (0)

  // Question Section
  response << "\xc0\x0c"; // Name (pointer to domain)
  response << "\x00\x01"; // Type (A record)
  response << "\x00\x01"; // Class (IN)

  // Answer Section
  response << "\xc0\x0c";         // Name (pointer to domain)
  response << "\x00\x01";         // Type (A record)
  response << "\x00\x01";         // Class (IN)
  response << "\x00\x00\x00\x3c"; // TTL (60 seconds)
  response << "\x00\x04";         // Data length (IPv4 address)

  

  if (ip_address.empty()) {
    ip_address = "0.0.0.0"; // Default IP (e.g., Google DNS)
  }

  // Add IP address to the response (4-byte IP address)
  istringstream ip_stream(ip_address);
  unsigned char ip_bytes[4];
  char dot;
  ip_stream >> (int &)ip_bytes[0] >> dot >> (int &)ip_bytes[1] >> dot >>
      (int &)ip_bytes[2] >> dot >> (int &)ip_bytes[3];
  response.write(reinterpret_cast<char *>(ip_bytes), 4);

  return response.str();
}
