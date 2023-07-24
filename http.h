#ifndef HTTP_H
#define HTTP_H

#include <string> 

enum class Method {
	GET,
	HEAD,
	POST
};

class HTTPrequest {
public:
	HTTPrequest();
	void setHeaders(const std::string&);
	void setBody(const std::string&);
	void setMethod(Method);
	std::string getHeaders() const;
	std::string getBody() const;
	Method getMethod() const;
private:
	std::string headers;
	std::string body;
	Method method;
};

class HTTPresponse {
public:
	HTTPresponse();
	void setStatusCode(int);
	void setStatusMessage(const std::string&);
	int getStatusCode() const;
	std::string getStatusMessage() const;
	void setHeaders(const std::string&);
	void setBody(const std::string&);
	std::string getHeaders() const;
	std::string getBody() const;
	std::string toString() const;
private:
	int statusCode;
	std::string statusMessage;
	std::string headers;
	std::string body;
};

#endif // HTTP_H