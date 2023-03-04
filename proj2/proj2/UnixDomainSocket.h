/* Copyright 2023 Siegfrij */

#ifndef PROJ2_UNIXDOMAINSOCKET_H_
#define PROJ2_UNIXDOMAINSOCKET_H_

#include <sys/socket.h>  // Unix header for sockets, using sockets
#include <sys/un.h>  // defns for Unix domain sockets, using struct sockaddr_un
#include <unistd.h>  // Unix standard header, using close
#include <cstring>   // using strncpy, strerror


#include <string>
#include <iostream>


class UnixDomainSocket {
 public:
  inline virtual UnixDomainSocket(const char* socket_path = "srv_domain_socket", bool abstract = true) = 0;

 protected:
  const char kUS = static_cast<char>(31);
  const char ETX = static_cast<char>(3);
};

inline UnixDomainSocket::UnixDomainSocket(const char* socket_path, bool abstract) {
  socket_path_ = std::string(socket_path);

  sock_addr_ = {};
  sock_addr_.sun_family = AF_UNIX;

  if (abstract)
    strncpy(sock_addr_.sun_path + 1,
            socket_path,
            sizeof(sock_addr_.sun_path) - 1);
  else
    strncpy(sock_addr_.sun_path,
            socket_path,
            sizeof(sock_addr_.sun_path));
}

#endif // PROJ2_UNIXDOMAINSOCKET_H_