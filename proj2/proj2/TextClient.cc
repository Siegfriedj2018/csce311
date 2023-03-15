/* Copyright 2023 Siegfrij */

#include <proj2/DomainSocketClient.h>

const char kSocket_path[] = "srv_domain_socket";

int main(int argc, char *argv[]) {
  DomainSocketClient dsc(kSocket_path, argc, (argv+1));
  dsc.RunClient();
  return 0;
}
