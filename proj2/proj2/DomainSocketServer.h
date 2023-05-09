/* Copyright 2023 Siegfrij */

#ifndef PROJ2_DOMAINSOCKETSERVER_H_
#define PROJ2_DOMAINSOCKETSERVER_H_

#include <proj2/UnixDomainSocket.h>

#include <sys/sysinfo.h>  // using get_nprocs_conf
#include <cerrno>         // using errno
#include <cstddef>        // using size_t
#include <cstdlib>        // using exit
#include <vector>         // using vector
#include <string>         // using string
#include <fstream>        // using fstream
#include <algorithm>      // using find


class DomainSocketServer : public UnixDomainSocket {
 public:
  explicit DomainSocketServer(const char* socket_path);
  std::vector<std::string> SearchFile(std::string path,
                                      std::string operation,
                                      std::vector<std::string> seeking);
  const std::vector<std::string> Explode(const std::string s, const char c);
  void RunServer();
  void CleanUp();

 private:
  const int kNum_Proc_ = (get_nprocs_conf() - 1);
  std::string path_;
  std::string op_;
  std::vector<std::string> seeking_;
};

#endif  // PROJ2_DOMAINSOCKETSERVER_H_
