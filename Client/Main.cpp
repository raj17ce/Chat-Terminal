#include "include/Client.h"
#include <iostream>
#include <string>
#include <thread>

int main() {
  Client chatClient{};

  std::string addressInput;
  std::cout << "Enter IP Address of the Server: ";
  std::getline(std::cin, addressInput);

  std::string portInput;
  std::cout << "Enter Port of the Server: ";
  std::getline(std::cin, portInput);
  uint16_t port = static_cast<uint16_t>(std::stoul(portInput));

  chatClient.ConnectToServer(addressInput.c_str(), port);

  std::thread sendThread{&Client::SendMessages, &chatClient};
  std::thread receiveThread{&Client::ReceiveMessages, &chatClient};

  sendThread.join();
  receiveThread.join();
}
