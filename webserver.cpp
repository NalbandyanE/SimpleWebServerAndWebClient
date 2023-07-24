#include "webserver.h"
#include "base64.h"

#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>

WebServer::WebServer(int port) : port{port}, socket(Domain::DOMAIN_AF_INET, 0) {
    	socket.create();
    	socket.bind("0.0.0.0", port);
    	socket.listen(10);
}

void WebServer::start() {
	std::cout << "WebServer listening on port " << port << std::endl;
    	while (true) {
        	TCPSocket clientSocket = socket.accept();
        	handleClient(clientSocket);
    	}
}

HTTPrequest WebServer::parseHTTPRequest(const std::string& requestString) {
    	HTTPrequest request;
    	size_t endHeaders = requestString.find("\r\n\r\n");
    	if (endHeaders == std::string::npos) {
        	throw std::runtime_error("Invalid HTTP request: No headers found.");
    	}
    	std::istringstream iss(requestString);
    	std::string requestLine;
    	std::getline(iss, requestLine); 
    	std::istringstream issRequestLine(requestLine);
    	std::string methodStr, path, httpVersion;
    	issRequestLine >> methodStr >> path >> httpVersion;
    	if (methodStr == "GET") {
       		request.setMethod(Method::GET);
    	} else if (methodStr == "HEAD") {
        	request.setMethod(Method::HEAD);
    	} else if (methodStr == "POST") {
        	request.setMethod(Method::POST);
    	} else {
        	request.setMethod(Method::GET);
    	}
    	request.setHeaders(requestString.substr(requestLine.length() + 2, endHeaders - requestLine.length() - 2));
    	if (endHeaders + 4 < requestString.size()) {
        	request.setBody(requestString.substr(endHeaders + 4));
    	}
    	return request;
}

void WebServer::handleClient(const TCPSocket& clientSocket) {
	char buffer[2048];
    	ssize_t receivedBytes = clientSocket.recv(buffer, sizeof(buffer));
    	if (receivedBytes <= 0) {
        	std::cerr << "Error while receiving request from client." << std::endl;
        	clientSocket.close();
        	return;
    	}
    	std::string requestString(buffer, receivedBytes);
    	HTTPrequest request = parseHTTPRequest(requestString);
   	HTTPresponse response = generateResponse(request);
    	sendResponse(clientSocket, response);
    	clientSocket.close();
}

HTTPresponse WebServer::generateResponse(const HTTPrequest& request) {
	HTTPresponse response;
    	response.setStatusCode(200);
    	response.setStatusMessage("OK");
    	response.setHeaders("Content-Type: text/html\r\n");
    	std::string htmlBody = "<html><body><h1><i>Hello, this is a simple web server response!<i></h1>";
    	htmlBody += "<img src=\"data:image/png;base64,";
    	std::ifstream imageFile("hello.png", std::ios::binary);
    	if (!imageFile.is_open()) {
        	response.setStatusCode(404);
        	response.setStatusMessage("Not Found");
        	response.setHeaders("Content-Type: text/plain\r\n");
        	response.setBody("404 Not Found");
        	return response;
    	}
    	std::stringstream ss;
    	ss << imageFile.rdbuf();
    	imageFile.close();
    	std::string imageData = ss.str();
    	std::string base64Image = base64_encode(reinterpret_cast<const unsigned char*>(imageData.c_str()), imageData.length());
    	htmlBody += base64Image;
    	htmlBody += "\">";
    	htmlBody += "</body></html>";
    	response.setBody(htmlBody);
    	return response;
}

void WebServer::sendResponse(const TCPSocket& clientSocket, const HTTPresponse& response) {
	std::string responseString = response.toString();
    	ssize_t sentBytes = clientSocket.send(responseString.c_str(), responseString.size());
    	if (sentBytes < 0) {
        	std::cerr << "Error while sending response to client." << std::endl;
    	}
}

int main() {
#if defined(_WIN32)
    	WSADATA d;
    	if (WSAStartup(MAKEWORD(2, 2), &d)) {
        	fprintf(stderr, "Failed to initialize.\n");
        	return 1;
    	}
#endif
    	int port = 8080; 
    	WebServer webServer(port);
    	webServer.start();

#if defined(_WIN32)
   	WSACleanup();
#endif

    	return 0;
}
