/* Copyright 2023 Siegfrij */

#include <proj2/DomainSocketClient.h>

int main(int argc, char *argv[]) {
  if (argc < 4) {
    std::cout << "Usage:\n"
    << "\t./text-client socket_name filename search_option1\n"
    << "\t./text-client socket_name filename search_option1 + search_option2\n"
    << "\t./text-client socket_name filename search_option1 x search_option2"
    << std::endl;
    exit(EXIT_FAILURE);
  }
  DomainSocketClient dsc(*(argv+1));
  dsc.ParseCLI(argc-2, (argv+2));
  dsc.RunClient();
  return EXIT_SUCCESS;
}
