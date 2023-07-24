"# SimpleWebServerAndWebClient" 

This is a simple command-line web server and web client written in C++. The web server can handle HTTP GET requests and respond with a simple HTML page with image, while the web client can send HTTP GET requests to the server and display the server's response.

## Web Server
### Requirements
C++ compiler with C++17 support (e.g., g++ or clang++)
Unix-like operating system (Linux, macOS) or Windows (with some modifications for Windows compatibility)

### Build Instructions
1. Clone the repository to your local machine:
   ```bash
   git clone [https://github.com/your-username/simplewebserver.git](https://github.com/NalbandyanE/SimpleWebServerAndWebClient.git)
   cd simplewebserver
3. Build the project using g++ or any other C++ compiler:
   ```bash
   g++ socket.cpp base64.cpp http.cpp webserver.cpp
5. ```bash
   ./a.out
6. Open your web browser and type the following URL:
   http://localhost:8080
Alternatively, you can use the web client provided to send HTTP requests to the server:
   ```bash
   ./a.out http://localhost:8080

### Server Features
- Listens on port 8080 for incoming HTTP requests.
- Supports GET, HEAD, and POST methods.
- Responds to requests with simple text content and an embedded image.
- Handles 404 Not Found error when the requested image is not available.

### How It Works
1. The server listens on port 8080 for incoming connections.
2. When a client connects, the server reads the HTTP request sent by the client.
3. The server parses the request and extracts the request method and headers.
4. Based on the request method, the server generates an appropriate HTTP response.
5. If the request method is GET or HEAD, the server responds with a simple HTML page containing a message and an embedded image.
6. If the request method is POST, the server sends back the same message received in the request body.
7. If the requested image is not available, the server responds with a 404 Not Found error.

### Screenshot


### Troubleshooting
If you encounter any issues while running the web server, make sure there are no other services running on the specified port.

## Web Client
### Usage
1. ```bash
   g++ socket.cpp http.cpp webclient.cpp
2. ```bash
   ./a.out http://localhost:8080

The client will attempt to connect to the web server and send an HTTP GET request. It will then display the server's response, including any HTML content or other data returned by the server.

### Features
Sends HTTP GET requests to the web server and displays the server's response.
Handles both text-based and binary responses (e.g., HTML pages and images).

### Troubleshooting
If you encounter any issues while running the web client, make sure that the web server is running and accessible from your machine.

### Contributions
Contributions to this project are welcome. If you find any issues or have ideas for improvements, feel free to open an issue or submit a pull request.

### Contact
For any questions or inquiries, please contact eminalbandyan1@gmail.com.
