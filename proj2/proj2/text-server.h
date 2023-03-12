/* Copyright 2023 Siegfrij */

#ifndef PROJ2_TEXT_SERVER_H_
#define PROJ2_TEXT_SERVER_H_

#include <sys/sysinfo.h> // using get_nprocs_conf
#include <assert.h>  // might not need this
#include <cerrno>    // using errno
#include <cstddef>   // using size_t
#include <cstdlib>   // using exit

#include <proj2/UnixDomainSocket.h>


class DomainSocketServer : public UnixDomainSocket {
 public:
  DomainSocketServer(const char* socket_path = "srv_domain_socket");
  void DomainSocket(const char* socket_path, bool abstract = true);
  void RunServer();
  
 private:
  const int kNum_Proc_ = (get_nprocs_conf() - 1);
};

#endif  // PROJ2_TEXT_SERVER_H_