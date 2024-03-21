# Unix Client

The Unix Client is a simple project demonstrating the basic functionality of Unix sockets. 
It provides a client application that can communicate with a Unix server using socket connections.

## Supported Commands

The client application can handle the following commands:

- `date`: Requests the current date from the server.
- `time`: Requests the current time from the server.
- `exit`: Closes the connection to the server and exits the client application.
- `shutdown`: Sends a shutdown command to the server.

## Building and Running

### Prerequisites
- C/C++ compiler
- CMake
- VS Code with CMake Tools extension

### Steps to Build and Run
1. Open the project in VS Code.
2. Install the CMake Tools extension if not already installed.
3. Press `Ctrl+Shift+P` to open the command palette.
4. Select `CMake: Configure` to configure the project.
5. Build the project by selecting `CMake: Build`. Or `cmake run .  Unix_server`
6. Run the client application by executing the generated executable `./Unix_server`.
7. To run tests, use VS code test tools or Cmake tools `ctest`.

## Usage
- After running the client application, follow the prompts to enter commands.
- The client will send the entered command to the server and display the response received.
