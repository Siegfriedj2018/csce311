/* Copyright 2023 Siegfrij*/

#include <proj2/UnixDomainSocket.h>

UnixDomainSocket::UnixDomainSocket(const char* socket_path) {
  socket_path_ = std::string(socket_path);

  sock_addr_ = {};
  sock_addr_.sun_family = AF_UNIX;


  strncpy(sock_addr_.sun_path + 1,
          socket_path,
          sizeof(sock_addr_.sun_path) - 1);
}
