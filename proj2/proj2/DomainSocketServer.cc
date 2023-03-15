/* Copyright 2023 Siegfrij */

#include <proj2/DomainSocketServer.h>
// notes from recording 11 on blackboard

// TODO(Siegfrij):
/*
  to parse string:
  std::string whatever = "abc;def"

  for (char c : whatever) {
    if (c == static_cast<char>(unit sep)) {
      something
    } else {
      write c
    }
  }
*/

DomainSocketServer::DomainSocketServer(const char* socket_path,
                                       bool abstract) {
  UnixDomainSocket::DomainSocket(socket_path, abstract);
}

void DomainSocketServer::RunServer() {
  int sock_fd;
  int ser_cli_sock_fd;
  std::string bytes_stream = "";

  // (1) Create a socket
  sock_fd = socket(AF_UNIX, SOCK_STREAM, 0);
  if (sock_fd < 0) {
    std::cerr << strerror(errno) << std::endl;
    exit(EXIT_FAILURE);
  }

  // (2) bind socket to address for the server
  unlink(socket_path_.c_str());

  int success = bind(sock_fd,
                     reinterpret_cast<const sockaddr*>(&sock_addr_),
                     sizeof(sock_addr_));

  if (success < 0) {
    std::cerr << strerror(errno) << std::endl;
    exit(EXIT_FAILURE);
  }

  std::clog << "SERVER STARTED" << std::endl;
  std::clog << "\tMAX CLIENTS: " << kNum_Proc_ << std::endl;

  // (3) listen for connections from client
  success = listen(sock_fd, kNum_Proc_);
  if (success < 0) {
    std::cerr << strerror(errno) << std::endl;
    exit(EXIT_FAILURE);
  }

  const size_t kbuffer_size = 34;  // read 4 byte increaments
  char buffer[kbuffer_size];
  ::ssize_t bytes_read;
  int byte_string;
  const char kKill_msg[] = "quit";
  // int bytes_wrote;

  while (true) {
    // (4) accept connection from client
    std::clog << "WAITING FOR CONNECTION..." << std::endl;
    ser_cli_sock_fd = accept(sock_fd, nullptr, nullptr);
    if (ser_cli_sock_fd < 0) {
      std::cerr << strerror(errno) << std::endl;
      exit(EXIT_FAILURE);
    }

    std::clog << "CLIENT CONNECTED ON SOCKET: " <<
                  ser_cli_sock_fd << std::endl;
    bytes_read = read(ser_cli_sock_fd, buffer, kbuffer_size);

    while (buffer[bytes_read] != kETX) {

      bytes_stream = buffer;
      std::clog << "bytes stream as int: ";
      for (char c : buffer) {
        byte_string = static_cast<int>(c);
        std::clog << byte_string << " ";
      }
      std::clog << std::endl;

      std::clog << "Testing byte stream: " << bytes_stream << std::endl;
      // writes out how many bytes received
      std::clog << "BYTES RECEIVED: " << bytes_read << std::endl;

      if (strncmp(buffer, kKill_msg, sizeof(kKill_msg)) == 0) {
        std::cout << "Server shutting down..." << std::endl;

        bytes_read = 0;
        exit(EXIT_SUCCESS);
      }


      bytes_stream = "";
      bytes_read = read(ser_cli_sock_fd, buffer, kbuffer_size);

      if (bytes_read == 0) {
        std::clog << "CLIENT DISCONNECTED" << std::endl;
        close(ser_cli_sock_fd);
        break;
      } else if (bytes_read < 0) {
        std::cerr << strerror(errno) << std::endl;
        close(ser_cli_sock_fd);
        exit(EXIT_FAILURE);
      }
    }

    // client disconnected or has an error some other way
    // if (bytes_read == 0) {
    //   std::clog << "CLIENT DISCONNECTED" << std::endl;
    //   close(ser_cli_sock_fd);
    // } else if (bytes_read < 0) {
    //   std::cerr << strerror(errno) << std::endl;
    //   exit(EXIT_FAILURE);
    // }
  }
}


      // // writes out how many bytes receive and what was received
      // std::clog << "BYTES RECEIVED: " << bytes_read << std::endl;
      // std::cout.write(buffer, bytes_read) << std::endl;

      // if (strcmp(buffer, kServer) == 0) {
      //   std::clog << "\nCLIENT SAID HELLO" << std::endl;
      //   bytes_wrote = write(ser_cli_sock_fd, test, strlen(test));
      //   std::clog << "BYTES SENT: " << bytes_wrote << std::endl;
      // } else {
      //   std::clog << "\nCLIENT DID NOT SAY HELLO" << std::endl;
      //   bytes_wrote = write(ser_cli_sock_fd, kTest2, strlen(kTest2));
      //   std::clog << "BYTES_SENT: " << bytes_wrote << std::endl;
      // }

