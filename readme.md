# cpp_webserver

A lightweight, dependency-free HTTP server written in C++ using WinSock. This server serves static files (HTML, CSS, JS, images, etc.) from a specified subdirectory under the current working directory. Useful for testing web apps locally or embedding static content into larger native applications.

## Features

- Simple single-header, single-source C++ implementation
- Supports serving files like `.html`, `.js`, `.css`, `.png`, `.jpg`, and more
- Custom MIME type handling
- Multi-threaded client handling using `std::thread`
- Dynamically maps routes to a subfolder
- Returns 404 page if the file is not found
- Defaults to `index.html` when root is requested
- Usage message when launched without arguments
- Designed for easy integration as a plugin in larger C++ apps

---

## Build Instructions

This project requires **CMake** and **MinGW** (or any compiler supporting WinSock + C++17).

### Step-by-step

1. Clone or copy this project to a folder (e.g., `C:\Code\cpp_webserver`)
2. Open a command prompt and run the build script:

```cmd
b_cpp_webserver.bat
```

### USAGE

```bash
cpp_webserver.exe <subfolder> [port]
```
- subfolder: Name of the folder under the current directory to serve
- port (optional): Port number to listen on (default is 8080)

```bash
cpp_webserver.exe webappfolder 9090
```

If the current directory is C:\Code\cpp_webserver, this will serve:

```cmd
C:\Code\webapps\webappfolder
```
at

```cmd
http://localhost:9090
```

If / is requested, the server will attempt to return /index.html.

### Testing It
- Create a folder webappfolder in the same directory as the executable.
- Put an index.html inside webappfolder.
- Launch the server as:

```cmd
cpp_webserver.exe webappfolder
```

Then open http://localhost:8080 in your browser.

## Smart Defaults & Usage Flag

If you run cpp_webserver.exe without any parameters:

- It automatically serves from the current working directory.
- It uses port 8080 by default.
- It prints a quick reminder for usage:

```cmd
No parameters provided. Defaulting to current directory and port 8080.
To see usage help, run with: --usage
```

To explicitly see help:

```cmd
cpp_webserver.exe --usage
```

This prints a usage guide as follows:

```cmd
Simple C++ HTTP Server
Usage:
    cpp_webserver.exe [subfolder] [port]
    cpp_webserver.exe --usage

Where:
    [subfolder]   Optional folder to serve (default: current folder)
    [port]        Optional port number (default: 8080)

Example:
    cpp_webserver.exe webappfolder 9090
    cpp_webserver.exe
```

## Notes
- This project currently supports only Windows using WinSock.
- All output is written to stdout using std::cout.
- No emojis, icons, or external dependencies are used in console output.
- Ideal for embedding HTML onboarding flows in native installers or devices.

## License
This project is licensed under the MIT License. Use it freely, even in commercial applications.