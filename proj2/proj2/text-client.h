/* Copyright 2023 Siegfrij */

#ifndef PROJ2_TEXT_CLIENT_H_
#define PROJ2_TEXT_CLIENT_H_

#include <assert.h> // might not need
#include <cerrno>   // using errno
#include <cstddef>  // using size_t
#include <cstdlib>  // using exit

#include <proj2/UnixDomainSocket.h>

class DomainSocketClient : public UnixDomainSocket {
  public:
   DomainSocketClient();
   void RunClient();
  
  protected:
    ::sockaddr_un sock_addr_;
    std::string socket_path_;
}

#endif  // PROJ2_TEXT_CLIENT_H_