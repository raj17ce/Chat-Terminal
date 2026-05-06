#include "inc/Client.h"
#include <iostream>
#include <string>

void Client::SetSocket(int newSocket) { _socket = newSocket; }

int Client::GetSocket() const { return _socket; }

void Client::ConnectToServer(const char *ipAddress, uint16_t port) {
  _createSocket();
  _createServerAddress(ipAddress, port);

  if (connect(_socket, reinterpret_cast<sockaddr *>(&_serverAddress),
              sizeof(_serverAddress)) == -1) {
    std::cout << "Server Connection failed..." << std::endl;
    _closeProgram();
  }

  std::cout << "Successfully Connected to Server on PORT : " << port
            << std::endl;

  std::string message;
  std::cout << "Enter Your Chat Name : ";
  std::getline(std::cin, message);

  int returnCode = send(_socket, message.c_str(), message.length(), 0);

  if (returnCode == -1) {
    std::cout << "Message Sending Failed...\n";
    close(_socket);
    _socket = -1;
  }
}

void Client::SendMessages() {
  std::string message;
  while (true) {
    std::getline(std::cin, message);

    if (message == "/quit") {
      break;
    }

    int returnCode = send(_socket, message.c_str(), message.length(), 0);

    if (returnCode == -1) {
      std::cout << "Message Sending Failed...\n";
      break;
    }
  }
  close(_socket);
  _socket = -1;
}

void Client::ReceiveMessages() {
  char buffer[4096];

  while (true) {
    int bytesReceived = recv(_socket, buffer, sizeof(buffer), 0);

    if (bytesReceived <= 0) {
      std::cout << "Error Receiving Message..." << std::endl;
      break;
    }

    std::string message(buffer, bytesReceived);
    std::cout << message << std::endl;
  }
  close(_socket);
  _socket = -1;
}

void Client::_createSocket() {
  _socket = socket(AF_INET, SOCK_STREAM, 0);

  if (_socket == -1) {
    std::cout << "Socket Creation failed..." << std::endl;
    _closeProgram();
  }
}

void Client::_createServerAddress(const char *ipAddress, uint16_t port) {
  _serverAddress.sin_family = AF_INET;
  _serverAddress.sin_port = htons(port);

  if (inet_pton(AF_INET, ipAddress, &_serverAddress.sin_addr) != 1) {
    std::cout << "Setting IP Address failed..." << std::endl;
    _closeProgram();
  }
}

void Client::_closeProgram() {
  std::cout << "Closing the program..." << std::endl;
  if (_socket >= 0) {
    close(_socket);
    _socket = -1;
  }
  std::exit(0);
}

bool operator==(const Client &client1, const Client &client2) {
  return (client1.GetSocket() == client2.GetSocket());
}
