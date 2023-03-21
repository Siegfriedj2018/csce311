/* Copyright 2023 Siegfrij */

#include <proj2/DomainSocketServer.h>
#include "DomainSocketServer.h"
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
  UnixDomainSocket(socket_path), path_(""), 
  op_(""), seeking_() {/* empty */}

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
  char kBuffer[kBufferSize] = "";
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

    // int ascii_value = 0;
    // std::clog << "Kbuffer: " << std::endl;
    // for (int i = 0; i < bytes_read; i++) {
    //   ascii_value = (int)kBuffer[i];
    //   std::clog << ascii_value << " ";
    // }
    // std::clog << std::endl;
  
    while (kBuffer[bytes_read] != kETX_) {
      bytes_stream = std::string(kBuffer, bytes_read);
      exploded_string = DomainSocketServer::Explode(bytes_stream, kUS_);

      path_ = exploded_string[0];
      std::clog << "\tPATH: \"" << path_ << "\"" << std::endl;

      // std::clog << "Finding op..." << std::endl;
      if (exploded_string.size() > 2) {
        if (exploded_string[2] == "+") {
          op_ = "OR";
        } else if (exploded_string[2] == "x") {
          op_ = "AND";
        }
        // std::clog << "Op found!" <<  std::endl;
        std::clog << "\tOPERATION: " << op_ << std::endl;
        
        std::clog << "\tSEEKING: ";
        for (size_t i = 1, j = 0; i <= exploded_string.size();
            i += 2, ++j) {
          seeking_.push_back(exploded_string[i]);
          std::clog << i << " " << seeking_[j];
          if (i != exploded_string.size() - 1) {
            std::clog << ", ";
          }
        }
      } else {
        op_ = "n/a";
        std::clog << "\tOPERATION: " << op_ << std::endl;
        std::clog << "\tSEEKING: ";
        seeking_.push_back(exploded_string[1]);
        std::clog << seeking_[0];
      }
      std::clog << std::endl;

      // DomainSocketServer::SearchFile(bytes_stream);

      std::clog << "BYTES RECIEVED: " << bytes_read << std::endl;

      if (bytes_read == 0) {
        std::clog << "CLIENT DISCONNECTED" << std::endl;
        close(ser_cli_sock_fd);
      } else if (bytes_read < 0) {
        std::cerr << strerror(errno) << std::endl;
        exit(-1);
      }

      CleanUp();
      close(ser_cli_sock_fd);
      break;
      // bool found_etx = false;
      // for (int i = 0; i < bytes_read; i++) {
      //   if (kBuffer[i] == kETX_) {
      //     found_etx = true;
      //     close(ser_cli_sock_fd);
      //     break;
      //   }
      // }

      // if (found_etx) {
      //   close(ser_cli_sock_fd);
      //   break;
      // }
    }
  }
}

void DomainSocketServer::CleanUp() {
  path_.erase();
  op_.erase();
  seeking_.clear();

}

/*  
 *  This function is from the following cplusplus.com article
 *  It is not mine.
 *  source: https://cplusplus.com/articles/2wA0RXSz/
 */
const std::vector<std::string> DomainSocketServer::Explode(const std::string s,
                                                           const char c) {
  std::string buff{""};
  std::vector<std::string> v;

  for (char n : s) {
    if (n != c) {
      buff += n;
    } else if (n == c && buff != "") {
      v.push_back(buff); 
      buff.erase();
    }
  }

  if (buff != "") {
    v.push_back(buff);
  }

  return v;
}

// std::string DomainSocketServer::SearchFile(std::string buffer_string) {
// }
