#ifndef WEBCLIENT_H
#define WEBCLIENT_H

#include "http.h" 
#include "socket.h"

#include <string>

struct ParsedURL {
	std::string scheme;
	std::string protocol;
	std::string hostname;
	std::string port;
	std::string path;
	std::string queryString;
	std::string hash;
};

class WebClient {
public:
	WebClient(const std::string&);
	ParsedURL parse_url();
	void send_request(const HTTPrequest&);
	TCPSocket connect_to_host();
private:
	std::string url;
	TCPSocket socket;
};

#endif // WEBCLIENT_H