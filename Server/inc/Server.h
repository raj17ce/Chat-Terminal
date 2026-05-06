#ifndef SERVER_H
#define SERVER_H

#include "inc/ThreadPool.h"
#include <arpa/inet.h>
#include <map>
#include <memory>
#include <mutex>
#include <netinet/in.h>
#include <string>
#include <sys/socket.h>
#include <unistd.h>

class Server {
public:
  Server() : _socket{-1}, _socketAddress{}, _activeClients{}, _pool{8} {}

  ~Server() {
    if (_socket >= 0) {
      close(_socket);
    }
  }

  void Listen(const char *ipAddress, uint16_t port);
  int AcceptClient();
  void CloseClient(int client);
  void ReceiveMessages(int client);

private:
  int _socket;
  sockaddr_in _socketAddress;
  std::map<int, std::string> _activeClients;
  std::mutex _clientsMutex;
  ThreadPool _pool;

  void _createSocket();
  void _createSocketAddress(const char *ipAddress, uint16_t port);
  void _bindAddressToSocket();

  void _closeProgram();
};

#endif // SERVER_H
