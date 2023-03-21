/* Copyright 2023 Siegfrij */

#include <proj2/DomainSocketServer.h>

int main(int argc, char* argv[]) {

  if (argc < 2) {
    std::cout << "Usage:  ./text-server socket_name" << std::endl;
    exit(EXIT_FAILURE);
  }

  DomainSocketServer dss(*(argv+1));
  dss.RunServer();
  return 0;
}
