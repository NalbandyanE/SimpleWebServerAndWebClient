"# SimpleWebServerAndWebClient" 

This is a simple command-line web server and web client written in C++. The web server can handle HTTP GET requests and respond with a simple HTML page with image, while the web client can send HTTP GET requests to the server and display the server's response.

## Web Server
### Requirements
C++ compiler with C++17 support (e.g., g++ or clang++)
Unix-like operating system (Linux, macOS) or Windows (with some modifications for Windows compatibility)

### Build Instructions
1.Clone the repository to your local machine:
git clone https://github.com/your-username/simplewebserver.git
cd simplewebserver
2.Build the project using g++ or any other C++ compiler:
g++ socket.cpp base64.cpp http.cpp webserver.cpp

### Usage
To use the web server, run the compiled executable:
./a.out
The server will start listening, and you can access it using a web browser or the web client. (Type in your browser - `http://localhost:8080`)
Result would be
![Alt Text](Capture.png)

### Features
Handles HTTP GET requests and responds with a simple HTML page or an image.
Supports serving static files like images.

### Troubleshooting
If you encounter any issues while running the web server, make sure there are no other services running on the specified port.

## Web Client
### Usage
1. g++ socket.cpp http.cpp webclient.cpp
2. ./a.out http://localhost:8080

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
