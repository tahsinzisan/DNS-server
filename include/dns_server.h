#ifndef DNS_SERVER_H
#define DNS_SERVER_H

#include <boost/asio.hpp>
#include <string>
#include <vector>
#include "zone_file_reader.h"

class DNS_Server {
public:
    DNS_Server(boost::asio::io_service& io, short udp_port, short tcp_port);
    void run();

private:
    void start_receive();
    void start_accept();
    void handle_udp_request(std::size_t bytes_recvd);
    void handle_tcp_request(boost::asio::ip::tcp::socket& tcp_socket);
    void send_udp_response(const std::string& response);
    void send_tcp_response(const std::string& response, boost::asio::ip::tcp::socket& tcp_socket);

    boost::asio::ip::udp::socket udp_socket_;
    boost::asio::ip::tcp::acceptor tcp_acceptor_;
    boost::asio::ip::udp::endpoint remote_endpoint_;
    std::vector<char> recv_buffer_{1024};
    boost::asio::io_service& io_service_;
    ZoneFileReader zone_file_reader_;
};

#endif
