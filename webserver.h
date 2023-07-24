#ifndef WEBSERVER_H
#define WEBSERVER_H

#include "http.h"
#include "socket.h"

#include <string>

class WebServer {
public:
	WebServer(int);
	void start();
	HTTPrequest parseHTTPRequest(const std::string&);
private:
	int port;
	TCPSocket socket;
	void handleClient(const TCPSocket&);
	HTTPresponse generateResponse(const HTTPrequest&);
	void sendResponse(const TCPSocket&, const HTTPresponse&);
};

#endif // WEBSERVER_H