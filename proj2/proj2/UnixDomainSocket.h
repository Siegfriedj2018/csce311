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
  explicit UnixDomainSocket(const char* socket_path);

 protected:
  ::sockaddr_un sock_addr_;
  std::string socket_path_;
  const char kUS_ = static_cast<char>(31);
  const char kETX_ = static_cast<char>(3);
  const char kNUL_ = static_cast<char>(0);
};

#endif  // PROJ2_UNIXDOMAINSOCKET_H_
