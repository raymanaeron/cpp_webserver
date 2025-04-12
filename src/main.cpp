#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <thread>
#include <filesystem>
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

std::filesystem::path g_root;
int g_port = 8080;

bool ends_with(const std::string& str, const std::string& suffix) {
    return str.size() >= suffix.size() &&
           str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
}

std::string get_mime_type(const std::string& path) {
    if (ends_with(path, ".html")) return "text/html";
    if (ends_with(path, ".js")) return "application/javascript";
    if (ends_with(path, ".css")) return "text/css";
    if (ends_with(path, ".png")) return "image/png";
    if (ends_with(path, ".jpg")) return "image/jpeg";
    return "text/plain";
}

std::string read_file(const std::string& filepath) {
    std::ifstream file(filepath, std::ios::binary);
    if (!file) return "";
    std::ostringstream ss;
    ss << file.rdbuf();
    return ss.str();
}

void handle_client(SOCKET client) {
    char buffer[2048] = {0};
    recv(client, buffer, sizeof(buffer) - 1, 0);

    std::string request(buffer);
    std::istringstream req(request);
    std::string method, path;
    req >> method >> path;

    if (path == "/") path = "/index.html";
    std::string full_path = (g_root / path.substr(1)).string();

    std::string body = read_file(full_path);
    std::ostringstream response;

    if (!body.empty()) {
        response << "HTTP/1.1 200 OK\r\n"
                 << "Content-Type: " << get_mime_type(full_path) << "\r\n"
                 << "Content-Length: " << body.size() << "\r\n"
                 << "Connection: close\r\n\r\n"
                 << body;
    } else {
        std::string msg = "<h1>404 Not Found</h1>";
        response << "HTTP/1.1 404 Not Found\r\n"
                 << "Content-Type: text/html\r\n"
                 << "Content-Length: " << msg.size() << "\r\n"
                 << "Connection: close\r\n\r\n"
                 << msg;
    }

    send(client, response.str().c_str(), response.str().size(), 0);
    closesocket(client);
}

void print_usage() {
    std::cout << "\nSimple C++ HTTP Server\n";
    std::cout << "Usage:\n";
    std::cout << "    cpp_webserver.exe [subfolder] [port]\n";
    std::cout << "    cpp_webserver.exe --usage\n\n";
    std::cout << "Where:\n";
    std::cout << "    [subfolder]   Optional folder to serve (default: current folder)\n";
    std::cout << "    [port]        Optional port number (default: 8080)\n\n";
    std::cout << "Example:\n";
    std::cout << "    cpp_webserver.exe webappfolder 9090\n";
    std::cout << "    cpp_webserver.exe\n\n";
}

int main(int argc, char* argv[]) {
    if (argc >= 2 && std::string(argv[1]) == "--usage") {
        print_usage();
        return 0;
    }

    std::filesystem::path exe_path = std::filesystem::current_path();

    if (argc == 1) {
        std::cout << "\nNo parameters provided. Defaulting to current directory and port 8080.\n";
        std::cout << "To see usage help, run with: --usage\n\n";
    }

    std::string folder = (argc >= 2) ? argv[1] : ".";
    g_port = (argc >= 3) ? std::stoi(argv[2]) : 8080;
    g_root = exe_path / folder;

    if (!std::filesystem::exists(g_root)) {
        std::cerr << "Folder not found: " << g_root << "\n";
        return 1;
    }

    std::cout << "Serving from: " << g_root << "\n";
    std::cout << "Visit: http://localhost:" << g_port << "\n";

    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);

    SOCKET server = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(g_port);
    addr.sin_addr.s_addr = INADDR_ANY;

    bind(server, (sockaddr*)&addr, sizeof(addr));
    listen(server, SOMAXCONN);

    while (true) {
        SOCKET client = accept(server, nullptr, nullptr);
        std::thread(handle_client, client).detach();
    }

    closesocket(server);
    WSACleanup();
    return 0;
}
