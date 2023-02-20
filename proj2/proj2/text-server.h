/* Copyright 2023 Siegfrij */

#ifndef PROJ2_TEXT_SERVER_H_
#define PROJ2_TEXT_SERVER_H_

#include <sys/socket.h>  // Unix header for sockets, using sockets
#include <sys/un.h>  // defns for Unix domain sockets, using struct sockaddr_un
#include <sys/sysinfo.h> // using get_nprocs_conf
#include <unistd.h>  // Unix standard header, using close

#include <assert.h>  // using assert
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

class DomainSocketServer : public  UnixDomainSocket {
 public:
  using ::UnixDomainSocket::UnixDomainSocket;

  DomainSocketServer();
  void RunServer() const;
 
 private:
  const int kNum_Proc_;
};

#endif  // PROJ2_TEXT_SERVER_H_