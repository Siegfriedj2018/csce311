/* Copyright 2023 Siegfrij */

#include <proj2/DomainSocketClient.h>

DomainSocketClient::DomainSocketClient(const char* socket_path) :
  UnixDomainSocket(socket_path) {/* empty */}

// Parses cli and builds string to be sent to the server
void DomainSocketClient::ParseCLI(int count, char* commandline[]) {
  commands_ += commandline[0];  // path
  commands_ += kUS_;
  for (int i = 1; i < count; ++i) {
    commands_ += commandline[i];  // search string and op
    if (i == count-1) {
      commands_ += kETX_;
      commands_ += kNUL_;
    } else {
      commands_ += kUS_;
    }
  }

  char op = *(*(commandline+2));
  // std::cout << "Current op: " << *(*(commandline+2)) << std::endl;
  if (count > 5) {
    for (int i = 2; i < count; i += 2) {
      std::clog << *(*(commandline+i)) << std::endl;
      if (*(*(commandline+i)) != op) {
        std::cerr << "Mixed bool_ops not presently supported" << std::endl;
        exit(EXIT_FAILURE);
      }
    }
  }

  for (size_t i = 0; i < commands_.size(); ++i) {
    std::clog << commands_[i];
  }
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

  std::clog << "SERVER CONNECTION ACCEPTED" << std::endl;

  // (3a) write to socket
  ssize_t bytes_wrote;

  bytes_wrote = write(socket_fd, &commands_, commands_.size() + 3);
  std::clog << "BYTES SENT: " << bytes_wrote << std::endl;
  if (bytes_wrote < 0) {
    std::cerr << strerror(errno) << std::endl;
    exit(-1);
  }

  if (bytes_wrote == 0) {
    std::clog << "SERVER DROPPED CONNECTION" << std::endl;
    exit(-2);
  }

  //  (3b) read from socket
  const size_t kBufferSize = 512;
  char kBuffer[kBufferSize];
  ssize_t bytes_read;

  bytes_read = read(socket_fd, kBuffer, kBufferSize);

  if (bytes_read == 0) {
    std::clog << "SERVER DISCONNECTED" << std::endl;
    close(socket_fd);
  } else if (bytes_read < 0) {
    std::cerr << strerror(errno) << std::endl;
    exit(-1);
  }

  std::string str_buffer(kBuffer);
  int count = 1;
  size_t i = 0;

  if (str_buffer.find("INVALID FILE") != std::string::npos) {
    std::clog << "INVALID FILE" << std::endl;
  } else if (bytes_read == 1) {
    std::clog << "BYTES RECIEVED: " << bytes_read << std::endl;
  } else {
    i = str_buffer.find_first_of('\n');
    while (str_buffer.size() > 0 && i != std::string::npos) {
      std::cout << count << "\t";
      std::cout << str_buffer.substr(0, i);
      std::cout << std::endl;
      str_buffer.erase(0, i + 1);
      count++;
      i = str_buffer.find_first_of('\n');
    }

    std::clog << "BYTES RECIEVED: " << bytes_read << std::endl;
    }
}
