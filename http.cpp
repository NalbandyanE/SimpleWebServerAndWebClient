#include "http.h"

HTTPrequest::HTTPrequest() : method{Method::GET} 
{
}

void HTTPrequest::setHeaders(const std::string& h) {
	headers = h;
}

void HTTPrequest::setBody(const std::string& b) {
	body = b;
}

void HTTPrequest::setMethod(Method m) {
	method = m;
}

std::string HTTPrequest::getHeaders() const {
	return headers;
}

std::string HTTPrequest::getBody() const {
	return body;
}

Method HTTPrequest::getMethod() const {
	return method;
}

HTTPresponse::HTTPresponse() : statusCode{0} 
{
}

void HTTPresponse::setStatusCode(int c) {
	statusCode = c;
}

void HTTPresponse::setStatusMessage(const std::string& m) {
	statusMessage = m; 
}

int HTTPresponse::getStatusCode() const {
	return statusCode;
}

std::string HTTPresponse::getStatusMessage() const {
	return statusMessage;
}

void HTTPresponse::setHeaders(const std::string& h) {
	headers = h;
}

void HTTPresponse::setBody(const std::string& b) {
	body = b;
}

std::string HTTPresponse::getHeaders() const {
	return headers;
}

std::string HTTPresponse::getBody() const {
	return body;
}

std::string HTTPresponse::toString() const {
    std::string responseStr;
    responseStr += "HTTP/1.1 " + std::to_string(statusCode) + " " + statusMessage + "\r\n";
    responseStr += headers;
    responseStr += "\r\n";
    responseStr += body;
    return responseStr;
}