/* Copyright 2023 Siegfrij */

#ifndef PROJ2_DOMAINSOCKETCLIENT_H_
#define PROJ2_DOMAINSOCKETCLIENT_H_

#include <proj2/UnixDomainSocket.h>

#include <cassert>  // might not need
#include <cerrno>   // using errno
#include <cstddef>  // using size_t
#include <cstdlib>  // using exit

class DomainSocketClient : public UnixDomainSocket {
 public:
  DomainSocketClient(const char* socket_path,
                     int count,
                     char* command_line[]);
  void ParseCLI(int count, char* commandline[]);
  void RunClient();

 private:
  char* commands;
  
};

#endif  // PROJ2_DOMAINSOCKETCLIENT_H_
