/* Copyright 2023 Siegfrij */

#include <proj2/DomainSocketServer.h>


DomainSocketServer::DomainSocketServer(const char* socket_path) :
  UnixDomainSocket(socket_path), path_(""), 
  op_(""), seeking_() {/* empty */}

void DomainSocketServer::RunServer() {
  int sock_fd;
  int ser_cli_sock_fd;
  std::string bytes_stream = "";
  std::vector<std::string> exploded_string;
  std::vector<std::string> results;

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
  ssize_t bytes_wrote;

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
          std::clog << seeking_[j];
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

      results = DomainSocketServer::SearchFile(path_, op_, seeking_);

      char bytes_To_Send[kBufferSize];
      size_t write_count = 0;
      for (size_t i = 0; i < results.size(); ++i) {
        for (size_t j = 0; j < results[i].size() &&
                           write_count < kBufferSize; ++j, ++write_count) {
          bytes_To_Send[write_count] = results[i][j];
        }
      }


      bytes_wrote = write(ser_cli_sock_fd, bytes_To_Send, write_count);

      std::clog << "BYTES RECIEVED: " << bytes_read << std::endl;
      std::clog << "BYTES SENT: " << bytes_wrote << std::endl; 

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
    v.push_back(buff.substr(0, buff.size() - 1));
  }

  return v;
}


std::vector<std::string> DomainSocketServer::SearchFile(std::string path,
                                           std::string operation,
                                           std::vector<std::string> seeking) {
  std::string line = "";
  std::ifstream fs;
  std::vector<std::string> found_row;
  fs.open(path);

  if (!fs.is_open()) {
    found_row.push_back("INVALID FILE");
    return found_row;
  }

  while (getline(fs, line)) {
    size_t count = 0;
    for (size_t i = 0; i < seeking.size(); ++i) {
      size_t pos = line.find(seeking.at(i));
      // std::clog << "Searching for " << seeking.at(i) << " in..." << std::endl;
      // std::clog << line << std::endl;
      // std::clog << std::endl;
      if (pos != std::string::npos) {
        ++count;
        // std::clog << seeking.at(i) << " found" << std::endl;
        if (operation.compare("OR") == 0) {
          // std::clog << "Row added" << std::endl;
          found_row.push_back(line + "\n");
        }
      }
    }
    if (operation.compare("AND") == 0 && count == seeking.size()) {
      // std::clog << "Row added" << std::endl;
      found_row.push_back(line + "\n");
    }
  }
  fs.close();
  return found_row;
}
