//#include "../include/dns_server.h"
#include "../include/split_dns.h"
#include "../include/dns_response.h"
#include <boost/asio.hpp>
#include <iostream>
#include <memory>

using namespace boost::asio;
using namespace boost::asio::ip;
using namespace std;

DNS_Server::DNS_Server(io_service& io, short udp_port, short tcp_port)
    : udp_socket_(io, udp::endpoint(udp::v4(), udp_port)),
      tcp_acceptor_(io, tcp::endpoint(boost::asio::ip::address::from_string("0.0.0.0"), tcp_port)),
      io_service_(io) {
    start_receive();
    start_accept();
}


void DNS_Server::run() {
    io_service_.run();
}

void DNS_Server::start_receive() {
    udp_socket_.async_receive_from
        (
            buffer(recv_buffer_), remote_endpoint_,
            [this](boost::system::error_code ec, std::size_t bytes_recvd
        ) {
            if (!ec && bytes_recvd > 0) {
                handle_udp_request(bytes_recvd);
            }
            start_receive();
          });
}

void DNS_Server::start_accept() {
    shared_ptr<tcp::socket> socket = make_shared<tcp::socket>(io_service_);
    tcp_acceptor_.async_accept(*socket, [this, socket](boost::system::error_code ec) {
        if (!ec) {
            handle_tcp_request(*socket);
        }
        start_accept();
    });
}

void DNS_Server::handle_udp_request(std::size_t bytes_recvd) {
    string query(recv_buffer_.begin(), recv_buffer_.begin() + bytes_recvd);
    bool is_internal = is_internal_request(remote_endpoint_.address().to_string());
    string ip_address = zone_file_reader_.get_ip_for_domain(query, is_internal);
    string response = build_response(query, ip_address);
    send_udp_response(response);
}

void DNS_Server::handle_tcp_request(tcp::socket& tcp_socket) {
    char buffer[1024];
    size_t length = tcp_socket.read_some(boost::asio::buffer(buffer, sizeof(buffer)));
    string query(buffer, length);
    bool is_internal = is_internal_request(tcp_socket.remote_endpoint().address().to_string());
    string ip_address = zone_file_reader_.get_ip_for_domain(query, is_internal);
    string response = build_response(query, ip_address);
    send_tcp_response(response, tcp_socket);
}

void DNS_Server::send_udp_response(const string& response) {
    udp_socket_.async_send_to(buffer(response), remote_endpoint_,
        [](boost::system::error_code, std::size_t) {});
}

void DNS_Server::send_tcp_response(const string& response, tcp::socket& tcp_socket) {
    boost::system::error_code ec;
    tcp_socket.send(boost::asio::buffer(response), 0, ec);
}
