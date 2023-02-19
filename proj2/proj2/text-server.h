/* Copyright 2023 Siegfrij */

#include <sys/socket.h>  // Unix header for sockets, using sockets
#include <sys/un.h>  // defns for Unix domain sockets, using struct sockaddr_un
#include <unistd.h>  // Unix standard header, using close

#include <cerrno>    // using errno
#include <cstddef>   // using size_t
#include <cstdlib>   // using exit
#include <cstring>   // using strncpy, strerror

#include <string>
#include <iostream>

class UnixDomainSocket {
 public:
    explicit UnixDomainSocket(const char* socket_path, bool abstract = true);

 protected:
  ::sockaddr_un sock_addr_;
  std::string socket_path_;
};

class DomainSocket : public  UnixDomainSocket {
 public:
  using ::UnixDomainSocket::UnixDomainSocket;

  void RunServer() const;
};