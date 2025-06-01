
#include "split_dns.h"
#include "acl_manager.h"
#include <string>

bool is_internal_request(const std::string& client_ip) {
    std::cerr << "inside internal check" << std::endl;
    return check_acl(client_ip);  // Check ACL for internal access
}
