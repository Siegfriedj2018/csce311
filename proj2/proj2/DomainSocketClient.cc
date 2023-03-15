/* Copyright 2023 Siegfrij */

#include <proj2/DomainSocketClient.h>

DomainSocketClient::DomainSocketClient(const char* socket_path,
                                       int count,
                                       char* commandline[]) {
  UnixDomainSocket::DomainSocket(socket_path, true);
  ParseCLI(count, commandline);
}

void DomainSocketClient::ParseCLI(int count, char* commadline[]) {
  for (int i = 1, )
}

void DomainSocketClient::RunClient() {
  // (1) open nameless Unix socket
  int socket_fd = socket(AF_UNIX, SOCK_STREAM, 0);
  if (socket_fd < 0) {
    std::cerr << strerror(errno) << std::endl;
    exit(-1);
  }

  // (2) connect to an existing socket
  int success = connect(socket_fd,
                        // sockaddr_un is a Unix sockaddr
                        reinterpret_cast<const sockaddr*>(&sock_addr_),
                        sizeof(sock_addr_));
  if (success < 0) {
    std::cerr << strerror(errno) << std::endl;
    exit(-1);
  }

  // (3) write to socket
  const size_t kbuffer_size = 32;
  char buffer[kbuffer_size];
  // ssize_t bytes_read;
  ssize_t bytes_wrote;
  // const char kLF = static_cast<char>(10);
  const char kNUL = static_cast<char>(0);

  // while (true) {
    std::cin.getline(buffer, kbuffer_size);

    while (std::cin.gcount() > 0) {
      if (std::cin.gcount() == kbuffer_size - 1 && std::cin.fail()) {
        std::cin.clear();
      }

      // write() is equivalent to send() with no flags in send's 3rd param
      bytes_wrote = write(socket_fd, buffer, std::cin.gcount());
      // bytes_wrote += write(socket_fd, &kLF, sizeof(kLF));
      bytes_wrote += write(socket_fd, &kETX, sizeof(kETX));
      bytes_wrote += write(socket_fd, &kNUL, sizeof(kNUL));
      std::cout << "sent " << bytes_wrote + 2 << " bytes" << std::endl;

      if (bytes_wrote < 0) {
        std::cerr << strerror(errno) << std::endl;
        exit(-1);
      }

      if (bytes_wrote == 0) {
        std::clog << "Server dropped connection!" << std::endl;
        exit(-2);
      }
      // if ((bytes_read = read(socket_fd,
      //                        buffer,
      //                        kbuffer_size)) > 0) {
      //   std::clog << "SERVER: ";
      //   std::cout.write(buffer, bytes_read) << std::endl;
      //   std::clog << "BYTES READ: " << bytes_read << std::endl;
      //   std::clog << "READ COMPLETE\n" << std::endl;
      // }

      std::cin.getline(buffer, kbuffer_size);
    }
  // }
}
