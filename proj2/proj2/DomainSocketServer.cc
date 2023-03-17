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

DomainSocketServer::DomainSocketServer(const char* socket_path) :
  UnixDomainSocket(socket_path) {/* empty */}

void DomainSocketServer::RunServer() {
  int sock_fd;
  int ser_cli_sock_fd;
  std::string bytes_stream = "";
  std::vector<std::string> exploded_string;

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

  // (3) listen for connections from client
  success = listen(sock_fd, kNum_Proc_);
  if (success < 0) {
    std::cerr << strerror(errno) << std::endl;
    exit(EXIT_FAILURE);
  }

  std::clog << "SERVER STARTED" << std::endl;
  std::clog << "\tMAX CLIENTS: " << kNum_Proc_ << std::endl;

  const size_t kBufferSize = 512;
  char kBuffer[kBufferSize];
  ::ssize_t bytes_read;

  while (true) {
    // (4) accept connection from client
    std::clog << "WAITING FOR CONNECTION..." << std::endl;
    ser_cli_sock_fd = accept(sock_fd, nullptr, nullptr);
    if (ser_cli_sock_fd < 0) {
      std::cerr << strerror(errno) << std::endl;
      exit(EXIT_FAILURE);
    }

    std::clog << "CLIENT CONNECTED"  << std::endl;

    bytes_read = read(ser_cli_sock_fd, kBuffer, kBufferSize);

    while (kBuffer[bytes_read] != kETX_) {
      
      std::cout << "Testing: " << kBuffer << std::endl;
      exploded_string = DomainSocketServer::Explode(kBuffer, kUS_);
      std::clog << "STREAM EXPloded" << std::endl;
      for (std::string s : exploded_string) {
        std::cout << "Testing: " << s << std::endl;
      }
      path_ = exploded_string[0];

      std::clog << "PATH: \"" << path_ << "\"" << std::endl;

      if (exploded_string[2] == "+") {
        op_ = "OR";
      } else if (exploded_string[2] == "x") {
        op_ = "AND";
      } else {
        op_ = "n/a";
      }

      std::clog << "OPERATION: " << op_ << std::endl;

      for (size_t i = 1, j = 0; i < exploded_string.size();
           i = (2 * i) + 1, ++j) {
        seeking_.push_back(exploded_string[i]);
        std::clog << "SEEKING: " << seeking_[j] << ", ";
      }
      std::clog << std::endl;

      // DomainSocketServer::SearchFile(bytes_stream);

      std::clog << "BYTES RECIEVED: " << bytes_read << std::endl;
      bytes_read = read(ser_cli_sock_fd, kBuffer, kBufferSize);

      if (bytes_read == 0) {
        std::clog << "CLIENT DISCONNECTED" << std::endl;
        close(ser_cli_sock_fd);
      } else if (bytes_read < 0) {
        std::cerr << strerror(errno) << std::endl;
        exit(-1);
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


/*  
 *  This function is from the following cplusplus.com article
 *  It is not mine.
 *  https://cplusplus.com/articles/2wA0RXSz/
 */
const std::vector<std::string> DomainSocketServer::Explode(const std::string s,
                                                           const char c) {
  std::string buff{""};
  std::vector<std::string> v;

  for (char n : s) {
    if (n != c) {
      buff += n;
    } else if (n == c && buff != "") {
      v.push_back(buff); buff = "";
    }
  }

  if (buff != "") {
    v.push_back(buff);
  }

  for (size_t i = 0; i < v.size(); ++i) {
    std::cout << v[i] << std::endl;
  }
  return v;
}

// std::string DomainSocketServer::SearchFile(std::string buffer_string) {
// }
