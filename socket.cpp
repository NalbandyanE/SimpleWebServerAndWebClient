#include "socket.h"

#include <stdexcept>
#include <string>
#include <arpa/inet.h>
#include <cerrno>
#include <iostream>
#include <cstring>

void handleSocketError(const std::string& errorMessage) {
    std::cerr << errorMessage << ": ";
    perror("");
}

Socket::Socket(Domain domain, Type type, int protocol) 
	: domain{domain}, type{type}, protocol{protocol}, socketFd{-1} 
{
}

Socket::~Socket() {
	close();
}

void Socket::create() {
	socketFd = ::socket(static_cast<int>(domain), static_cast<int>(type), protocol);
	if (socketFd == -1) {
	    handleSocketError("Failed to create socket");
	    throw std::runtime_error("Failed to create socket");
	}
	int reuse = 1;
    if (setsockopt(socketFd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) == -1) {
        throw std::runtime_error("Failed to set socket options");
    }
}

void Socket::bind(const std::string& ipAddress, uint16_t port) {
    sockaddr_in address{};
    address.sin_family = static_cast<int>(domain);
    address.sin_port = htons(port);
    if (inet_pton(static_cast<int>(domain), ipAddress.c_str(), &(address.sin_addr)) <= 0) {
        handleSocketError("Invalid IP address");
        throw std::runtime_error("Invalid IP address");
    }
    if (::bind(socketFd, reinterpret_cast<sockaddr*>(&address), sizeof(address)) == -1) {
        handleSocketError("Failed to bind socket");
        throw std::runtime_error("Failed to bind socket");
    }
}

void Socket::close() const {
    ::close(socketFd);
}

void Socket::setSocketFd(int sf) {
    socketFd = sf;
}

void Socket::setDomain(Domain d) {
    domain = d;
}

void Socket::setType(Type t) {
    type = t;
}

void Socket::setProtocol(int p) {
    protocol = p;
}

void Socket::setLocalIpAddress(const std::string& l) {
    localIpAddress = l;
}

void Socket::setLocalPort(uint16_t lp) {
    localPort = lp;
}

void Socket::setRemotePort(uint16_t rp) {
    remotePort = rp;
}

void Socket::setRemoteIpAddress(const std::string& r) {
    remoteIpAddress = r;
}

int Socket::getSocketFd() const {
    return socketFd;
}

Domain Socket::getDomain() const {
    return domain;
}

Type Socket::getType() const {
    return type;
}

int Socket::getProtocol() const {
    return protocol;
}

std::string Socket::getLocalIpAddress() const {
    return localIpAddress;
}

uint16_t Socket::getLocalPort() const {
    return localPort;
}

std::string Socket::getRemoteIpAddress() const {
    return remoteIpAddress;
}

uint16_t Socket::getRemotePort() const {
    return remotePort;
}

TCPSocket::TCPSocket(Domain domain, int protocol) : Socket(domain, Type::TYPE_SOCK_STREAM, protocol) 
{
}

void TCPSocket::connect(const std::string& ipAddress, uint16_t port) {
    sockaddr_in serverAddress{};
    serverAddress.sin_family = static_cast<int>(getDomain());
    serverAddress.sin_port = htons(port);
    if (::inet_pton(static_cast<int>(getDomain()), ipAddress.c_str(), &(serverAddress.sin_addr)) <= 0) {
        handleSocketError("Failed to convert IP address");
        throw std::runtime_error("Failed to convert IP address");
    }

    std::string host = ipAddress + ":" + std::to_string(port);
    std::cout << "Attempting to connect to " << host << std::endl;

    if (::connect(getSocketFd(), reinterpret_cast<sockaddr*>(&serverAddress), sizeof(serverAddress)) == -1) {
        handleSocketError("Failed to connect to server");
        throw std::runtime_error("Failed to connect to server");
    }
    std::cout << "Connected to " << host << std::endl;
}

int Socket::select(int nfds, fd_set* readfds, fd_set* writefds, fd_set* exceptfds, timeval* timeout) {
    int result;
#ifdef _WIN32
    result = ::select(0, readfds, writefds, exceptfds, timeout);
#else
    result = ::select(nfds, readfds, writefds, exceptfds, timeout);
#endif
    if (result == -1) {
        throw std::runtime_error("Failed to use select");
    }
    return result;
}

void TCPSocket::listen(int backlog) {
	if (::listen(getSocketFd(), backlog) == -1) {
        throw std::runtime_error("Failed to listen on socket");
    }
}

TCPSocket TCPSocket::accept() {
	sockaddr_in clientAddress{};
    socklen_t clientAddressLength = sizeof(clientAddress);
    int newSocketFd = ::accept(getSocketFd(), reinterpret_cast<sockaddr*>(&clientAddress), &clientAddressLength);
    if (newSocketFd == -1) {
        throw std::runtime_error("Failed to accept connection");
    }
    TCPSocket newSocket(getDomain(), getProtocol());
    newSocket.setSocketFd(newSocketFd);
    newSocket.setLocalIpAddress(inet_ntoa(clientAddress.sin_addr));
    newSocket.setLocalPort(ntohs(clientAddress.sin_port));
    newSocket.setRemoteIpAddress(inet_ntoa(clientAddress.sin_addr));
    newSocket.setRemotePort(ntohs(clientAddress.sin_port));
    return newSocket;
}

ssize_t TCPSocket::send(const void* data, size_t size) const {
    ssize_t sentBytes = ::send(getSocketFd(), data, size, 0);
    if (sentBytes == -1) {
        throw std::runtime_error("Failed to send data: " + std::string(strerror(errno)));
    } else if (static_cast<size_t>(sentBytes) != size) {
        throw std::runtime_error("Failed to send complete data");
    }
    return sentBytes;
}

ssize_t TCPSocket::recv(void* buffer, size_t size) const {
	ssize_t receivedBytes = ::recv(getSocketFd(), buffer, size, 0);
    if (receivedBytes == -1) {
        throw std::runtime_error("Failed to receive data");
    }
    return receivedBytes;
}

void TCPSocket::shutdown() {
#ifdef _WIN32
    ::shutdown(getSocketFd(), SD_BOTH);
#else
    ::shutdown(getSocketFd(), SHUT_RDWR);
#endif    
}

UDPSocket::UDPSocket(Domain domain, int protocol) : Socket(domain, Type::TYPE_SOCK_DGRAM, protocol) 
{
}

void UDPSocket::connect(const std::string& ipAddress, uint16_t port) {
    sockaddr_in remoteAddress{};
    remoteAddress.sin_family = static_cast<int>(getDomain());
    remoteAddress.sin_port = htons(port);
    if (inet_pton(static_cast<int>(getDomain()), ipAddress.c_str(), &(remoteAddress.sin_addr)) <= 0) {
        throw std::runtime_error("Invalid IP address");
    }
    if (::connect(getSocketFd(), reinterpret_cast<sockaddr*>(&remoteAddress), sizeof(remoteAddress)) == -1) {
        throw std::runtime_error("Failed to associate UDP socket with remote address");
    }
}

ssize_t UDPSocket::recvfrom(void* buffer, size_t size, sockaddr_in* srcAddress, socklen_t* srcAddressLength) {
    ssize_t receivedBytes = ::recvfrom(getSocketFd(), buffer, size, 0, reinterpret_cast<sockaddr*>(srcAddress), srcAddressLength);
    if (receivedBytes == -1) {
        handleSocketError("Failed to receive data");
        throw std::runtime_error("Failed to receive data");
    }
    return receivedBytes;
}

ssize_t UDPSocket::sendto(const void* data, size_t size, const sockaddr_in* destAddress, socklen_t destAddressLength) {
    ssize_t sentBytes = ::sendto(getSocketFd(), data, size, 0, reinterpret_cast<const sockaddr*>(destAddress), destAddressLength);
    if (sentBytes == -1) {
        handleSocketError("Failed to send data");
        throw std::runtime_error("Failed to send data");
    }
    return sentBytes;
}