#ifndef SOCKET_H
#define SOCKET_H

#include <string>
#include <sys/time.h>  
#include <sys/select.h>  

#ifdef _WIN32
    	#include <winsock2.h>
    	#pragma comment(lib, "ws2_32.lib")
#else
    	#include <sys/types.h>
    	#include <sys/socket.h>
    	#include <netinet/in.h>
    	#include <arpa/inet.h>
    	#include <unistd.h>
#endif

enum class Domain {
	DOMAIN_AF_INET = 2,
	DOMAIN_AF_INET6 = 10
};

enum class Type {
	TYPE_SOCK_STREAM = 1,
	TYPE_SOCK_DGRAM = 2
};

class Socket {
public:
	Socket(Domain, Type, int);
    	virtual ~Socket();
    	void create();
    	void bind(const std::string&, uint16_t);
    	void close() const;
    	int select(int, fd_set*, fd_set*, fd_set*, timeval*);
    	void setSocketFd(int);
    	void setDomain(Domain);
    	void setType(Type);
    	void setProtocol(int);
    	void setLocalIpAddress(const std::string&);
    	void setLocalPort(uint16_t);
    	void setRemotePort(uint16_t);
    	void setRemoteIpAddress(const std::string&);
    	int getSocketFd() const;
    	Domain getDomain() const;
    	Type getType() const;
    	int getProtocol() const;
    	std::string getLocalIpAddress() const;
    	uint16_t getLocalPort() const;
    	std::string getRemoteIpAddress() const;
    	uint16_t getRemotePort() const;
    	virtual void connect(const std::string&, uint16_t) = 0;
private:
	Domain domain;
	Type type;
	int protocol;
	int socketFd;
	std::string localIpAddress;
    	uint16_t localPort;
    	std::string remoteIpAddress;
   	 uint16_t remotePort;
};

class TCPSocket : public Socket {
public:
    	TCPSocket(Domain, int);
    	ssize_t send(const void*, size_t) const;
    	ssize_t recv(void*, size_t) const;
    	void listen(int);
    	TCPSocket accept();
    	void connect(const std::string&, uint16_t) override;
    	void shutdown();
};

class UDPSocket : public Socket {
public:
    	UDPSocket(Domain, int);
   	ssize_t sendto(const void*, size_t, const sockaddr_in*, socklen_t);
    	ssize_t recvfrom(void*, size_t, sockaddr_in*, socklen_t*);
    	void connect(const std::string&, uint16_t) override;
};

#endif // SOCKET_H
