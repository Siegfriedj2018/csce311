/* Copyright 2023 Siegfrij */

#include <proj2/DomainSocketServer.h>

int main(int argc, char* argv[]) {
  DomainSocketServer dss(*(argv+1));
  dss.RunServer();
  return 0;
}
