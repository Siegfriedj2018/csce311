/* Copyright 2023 Siegfrij */

#include <proj2/DomainSocketServer.h>

const char kSocket_path[] = "srv_domain_socket";

int main(int argc, char *argv[]) {
  DomainSocketServer dss(kSocket_path, true);  // pass argv[1] here
  dss.RunServer();
  return 0;
}
