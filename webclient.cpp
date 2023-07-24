#include "webclient.h"

#include <string>
#include <cstring>
#include <vector>
#include <algorithm>
#include <iostream>
#include <netdb.h>
#include <arpa/inet.h>

std::string resolveHostnameToIP(const std::string& hostname) {
    addrinfo hints, *res;
    std::memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC; 
    hints.ai_socktype = SOCK_STREAM; 
    int status = getaddrinfo(hostname.c_str(), nullptr, &hints, &res);
    if (status != 0) {
        std::cerr << "getaddrinfo error: " << gai_strerror(status) << std::endl;
        throw std::runtime_error("Failed to resolve hostname to IP address");
    }
    void* addr;
    char ip_str[INET6_ADDRSTRLEN];
    if (res->ai_family == AF_INET) {
        sockaddr_in* ipv4_addr = reinterpret_cast<sockaddr_in*>(res->ai_addr);
        addr = &(ipv4_addr->sin_addr);
    } else {
        sockaddr_in6* ipv6_addr = reinterpret_cast<sockaddr_in6*>(res->ai_addr);
        addr = &(ipv6_addr->sin6_addr);
    }
    inet_ntop(res->ai_family, addr, ip_str, sizeof ip_str);
    std::string ip_address = ip_str;
    freeaddrinfo(res);
    return ip_address;
}

WebClient::WebClient(const std::string& u) : url{u}, socket(Domain::DOMAIN_AF_INET, 0)
{
	socket.create();
}

ParsedURL WebClient::parse_url() {
	ParsedURL parsed_url;
    size_t scheme_end = url.find("://");
    if (scheme_end != std::string::npos) {
        parsed_url.scheme = url.substr(0, scheme_end);
        scheme_end += 3; 
    } else {
        scheme_end = 0;
    }
    size_t fragment_start = url.find('#', scheme_end);
    if (fragment_start != std::string::npos) {
        parsed_url.hash = url.substr(fragment_start + 1);
    }
    size_t query_start = url.find('?', scheme_end);
    if (query_start != std::string::npos) {
        parsed_url.queryString = url.substr(query_start + 1, fragment_start - query_start - 1);
    }
    size_t path_start = scheme_end;
    if (query_start != std::string::npos) {
        parsed_url.path = url.substr(path_start, query_start - path_start);
    } else if (fragment_start != std::string::npos) {
        parsed_url.path = url.substr(path_start, fragment_start - path_start);
    } else {
        parsed_url.path = url.substr(path_start);
    }

    size_t host_start = path_start;
    if (url[path_start] == '/') {
        host_start++;
    }
    size_t host_end = url.find('/', host_start);
    if (host_end == std::string::npos) {
        host_end = query_start != std::string::npos ? query_start : fragment_start;
    }
    size_t port_start = url.find(':', host_start);
    if (port_start != std::string::npos && port_start < host_end) {
        parsed_url.hostname = url.substr(host_start, port_start - host_start);
        parsed_url.port = url.substr(port_start + 1, host_end - port_start - 1);
    } else {
        parsed_url.hostname = url.substr(host_start, host_end - host_start);
        parsed_url.port = parsed_url.protocol == "https" ? "443" : "80";
    }
    return parsed_url;
}

void WebClient::send_request(const HTTPrequest& request) {
    ParsedURL parsedURL = parse_url();
    TCPSocket connected_socket = connect_to_host();
    Method httpMethod = request.getMethod();
    std::string headers = request.getHeaders();
    std::string httpRequest;
    httpRequest += "HTTP/1.1 ";
    httpRequest += "\r\n";
    httpRequest += headers;
    httpRequest += "\r\n\r\n";
    ssize_t sentBytes = connected_socket.send(httpRequest.c_str(), httpRequest.size());
    if (sentBytes < 0) {
        std::cout << "Request isn't received." << std::endl;
    }
    char buffer[2048];
    ssize_t receivedBytes = connected_socket.recv(buffer, sizeof(buffer));
    if (receivedBytes < 0) {
        std::cout << "Error while receiving response." << std::endl;
    } else {
        std::cout << "Server Response:\n";
        std::cout.write(buffer, receivedBytes);
        // struct sockaddr_in sa;
        // sa.sin_family = AF_INET;
        // sa.sin_port = htons(std::stoi(parsedURL.port));
        // inet_pton(AF_INET, parsedURL.hostname.c_str(), &sa.sin_addr);
        // char hostname[NI_MAXHOST];
        // int result = getnameinfo(reinterpret_cast<struct sockaddr*>(&sa), sizeof(sa), hostname, NI_MAXHOST, nullptr, 0, 0);
        // if (result == 0) {
        //     std::cout << "\nHostname from IP address: " << hostname << std::endl;
        // } else {
        //     std::cerr << "\nFailed to convert IP address to hostname: " << gai_strerror(result) << std::endl;
        // }
    }
    connected_socket.close();
}

TCPSocket WebClient::connect_to_host() {
	ParsedURL parsedURL = parse_url();
    std::string ip = resolveHostnameToIP(parsedURL.hostname);
	socket.connect(ip, std::stoi(parsedURL.port));
	return socket;
}

int main(int argc, char* argv[]) {
#if defined(_WIN32)
    WSADATA d;
    if (WSAStartup(MAKEWORD(2, 2), &d)) {
        fprintf(stderr, "Failed to initialize.\n");
        return 1;
    }
#endif

    if (argc < 2) {
        fprintf(stderr, "usage: web_get url\n");
        return 1;
    }
    char* url = argv[1];
    WebClient wclient(url);
    HTTPrequest request;
    request.setHeaders("Content-Type: application/json");
    request.setBody("{\"key\": \"value\"}");
    wclient.send_request(request);

#if defined(_WIN32)
    WSACleanup();
#endif

    return 0;
}

//"http://www.example.com:80/res/page1.php?user=bob#account"