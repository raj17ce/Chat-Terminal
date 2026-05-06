#include "inc/Server.h"
#include <algorithm>
#include <cerrno>
#include <iostream>
#include <thread>
#include <utility>
#include <vector>

void Server::Listen(const char *ipAddress, uint16_t port) {
  _createSocket();
  _createSocketAddress(ipAddress, port);
  _bindAddressToSocket();

  if (listen(_socket, SOMAXCONN) == -1) {
    std::cout << "Socket Listening failed..." << std::endl;
    _closeProgram();
  }

  std::cout << "Server Started Listening On Port : " << port << std::endl;
}

int Server::AcceptClient() {
  int clientSocket = accept(_socket, nullptr, nullptr);

  if (clientSocket == -1) {
    std::cout << "Accepting Client failed..." << std::endl;
    return -1;
  }

  char buffer[4096];
  int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);

  if (bytesReceived <= 0) {
    std::cout << "Client Connection Closed..." << std::endl;
    close(clientSocket);
    return -1;
  }

  std::string name(buffer, bytesReceived);

  {
    std::lock_guard<std::mutex> lock(_clientsMutex);
    _activeClients.insert({clientSocket, name});
  }

  std::cout << "Connection Established with " << name << "..." << std::endl;

  _pool.ExecuteTask(
      std::bind(&Server::ReceiveMessages, this, std::placeholders::_1),
      clientSocket);

  return clientSocket;
}

void Server::CloseClient(int client) {
  close(client);
  std::lock_guard<std::mutex> lock(_clientsMutex);
  auto it = _activeClients.find(client);
  if (it != _activeClients.end()) {
    _activeClients.erase(it);
  }
}

void Server::ReceiveMessages(int client) {
  char buffer[4096];

  while (true) {
    int bytesReceived = recv(client, buffer, sizeof(buffer), 0);

    if (bytesReceived <= 0) {
      {
        std::lock_guard<std::mutex> lock(_clientsMutex);
        std::cout << "Connection With " << _activeClients[client]
                  << " is Closed..." << std::endl;
      }
      CloseClient(client);
      break;
    }

    std::string tempMessage(buffer, bytesReceived);
    std::string message;

    {
      std::lock_guard<std::mutex> lock(_clientsMutex);
      message = _activeClients[client] + " : " + tempMessage;
    }

    std::cout << message << std::endl;

    std::vector<int> failedClients;

    {
      std::lock_guard<std::mutex> lock(_clientsMutex);
      for (const auto &tempClient : _activeClients) {
        if (tempClient.first != client) {
          int returnCode =
              send(tempClient.first, message.c_str(), message.length(), 0);

          if (returnCode == -1) {
            std::cout << "Sending message to " + tempClient.second +
                             " Failed...\n";
            std::cout << "Closing Connection With " + tempClient.second +
                             "...\n";
            failedClients.push_back(tempClient.first);
          }
        }
      }
    }

    for (int failed : failedClients) {
      CloseClient(failed);
    }
  }
}

void Server::_createSocket() {
  _socket = socket(AF_INET, SOCK_STREAM, 0);

  if (_socket == -1) {
    std::cout << "ListenSocket Creation failed..." << std::endl;
    _closeProgram();
  }
}

void Server::_createSocketAddress(const char *ipAddress, uint16_t port) {
  _socketAddress.sin_family = AF_INET;
  _socketAddress.sin_port = htons(port);

  if (inet_pton(AF_INET, ipAddress, &_socketAddress.sin_addr) != 1) {
    std::cout << "Setting IP Address failed..." << std::endl;
    _closeProgram();
  }
}

void Server::_bindAddressToSocket() {
  if (bind(_socket, reinterpret_cast<sockaddr *>(&_socketAddress),
           sizeof(_socketAddress)) == -1) {
    std::cout << "Binding ServerAddress to ListenSocket failed..." << std::endl;
    _closeProgram();
  }
}

void Server::_closeProgram() {
  std::cout << "Closing the program..." << std::endl;
  if (_socket >= 0) {
    close(_socket);
    _socket = -1;
  }
  std::exit(0);
}
