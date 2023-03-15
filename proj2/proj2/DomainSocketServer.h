/* Copyright 2023 Siegfrij */

#ifndef PROJ2_DOMAINSOCKETSERVER_H_
#define PROJ2_DOMAINSOCKETSERVER_H_

#include <proj2/UnixDomainSocket.h>

#include <sys/sysinfo.h>  // using get_nprocs_conf
#include <assert.h>      // might not need this
#include <cerrno>        // using errno
#include <cstddef>       // using size_t
#include <cstdlib>       // using exit

class DomainSocketServer : public UnixDomainSocket {
 public:
  DomainSocketServer(const char* socket_path,
                     bool abstract = true);
  void RunServer();
  std::string searchFile(std::string filename, std::string keyword);

 private:
  const int kNum_Proc_ = (get_nprocs_conf() - 1);
};

#endif  // PROJ2_DOMAINSOCKETSERVER_H_
