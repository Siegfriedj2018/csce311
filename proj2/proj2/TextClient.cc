/* Copyright 2023 Siegfrij */

#include <proj2/DomainSocketClient.h>

int main(int argc, char *argv[]) {
  DomainSocketClient dsc(*(argv+1));
  dsc.ParseCLI(argc-2, (argv+2));
  dsc.RunClient();
  return 0;
}
