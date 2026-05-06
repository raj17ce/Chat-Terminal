#ifndef CLIENT_H
#define CLIENT_H

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

class Client {
public:
  Client() : _socket{-1}, _serverAddress{} {}

  ~Client() {
    if (_socket >= 0) {
      close(_socket);
    }
  }

  void SetSocket(int newSocket);
  int GetSocket() const;

  void ConnectToServer(const char *ipAddress, uint16_t port);

  void SendMessages();
  void ReceiveMessages();

  friend bool operator==(const Client &client1, const Client &client2);

private:
  int _socket;
  sockaddr_in _serverAddress;

  void _createSocket();
  void _createServerAddress(const char *ipAddress, uint16_t port);

  void _closeProgram();
};

#endif // CLIENT_H
