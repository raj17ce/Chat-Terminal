#include "inc/Server.h"
#include <iostream>
#include <memory>
#include <string>

int main() {
  auto ChatServer = std::make_unique<Server>();

    std::string portInput;
    std::cout << "Enter Port to Listen On: ";
    std::getline(std::cin, portInput);
    uint16_t port = static_cast<uint16_t>(std::stoul(portInput));

    ChatServer->Listen("0.0.0.0", port);

  while (true) {
    ChatServer->AcceptClient();
  }
}
