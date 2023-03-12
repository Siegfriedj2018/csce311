/* Copyright 2023 Siegfrij */

#include <proj2/text-server.h>
// notes from recording 11 on blackboard

// TODO: move this to its own .cc and .h 
//       and add unit seperator(31) and ETX(3)
//       hardcode socket name
//       Get client/server to send hello world one way first then back
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

DomainSocketServer::DomainSocketServer(const char* socket_path) 
  : UnixDomainSocket() {
    DomainSocket(socket_path);
  }

inline void DomainSocketServer::DomainSocket(const char* socket_path, bool abstract) {
  socket_path_ = std::string(socket_path);

  sock_addr_ = {};
  sock_addr_.sun_family = AF_UNIX;

  if (abstract) {
    strncpy(sock_addr_.sun_path + 1,
            socket_path,
            sizeof(sock_addr_.sun_path) - 1);
  } else {
    strncpy(sock_addr_.sun_path,
            socket_path,
            sizeof(sock_addr_.sun_path));
    // return 1; // for not abstract for whatever reason
  }
  // errored
  // return -1;
}


void DomainSocketServer::RunServer() {
  int sock_fd;
  int client_req_sock_fd;

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

  const size_t kRead_buffer_size = 32;  // read 4 byte increaments
  // const ssize_t kWrite_buffer_size = 32;
  char read_buffer[kRead_buffer_size];
  // char write_buffer[kWrite_buffer_size];
  int bytes_read;
  // ssize_t bytes_wrote;
  const char* kServer = "hello server";
  const char* test = "Hello Client";
  const char kKill_msg[] = "quit";
  const char* kTest2 = " ";
  int bytes_wrote;

  while (true) {
    // (4) accept connection from clistatic_cast<char>(0)ent
    client_req_sock_fd = accept(sock_fd, nullptr, nullptr);
    if (client_req_sock_fd < 0) {
      std::cerr << strerror(errno) << std::endl;
      exit(EXIT_FAILURE);
    }
    std::clog << "SERVER STARTED" << std::endl;
    std::clog << "\tMAX CLIENTS: " << kNum_Proc_ << std::endl;

    std::clog << "CLIENT CONNECTED\n" << std::endl;
    bytes_read = read(client_req_sock_fd, read_buffer, kRead_buffer_size);

    while (bytes_read > 0) {

      if (strcmp(read_buffer, kKill_msg) == 0) {
        std::cout << "Server shutting down..." << std::endl;

        bytes_read = 0;
        exit(EXIT_SUCCESS);
      }
      if (strcmp(read_buffer, kServer) == 0) {
        std::clog << "CLIENT SAID HELLO\n" << std::endl;
        bytes_wrote = write(client_req_sock_fd, test, strlen(test));
        std::clog << "BYTES SENT: " << bytes_wrote << std::endl;
      } else {
        std::clog << "\nCLIENT DID NOT SAY HELLO\n" << std::endl;
        bytes_wrote = write(client_req_sock_fd, kTest2, strlen(kTest2));
        std::clog << "BYTES_SENT: " << bytes_wrote << std::endl;
      }

      // writes out how many bytes receive and what was received
      std::clog << "BYTES RECEIVED: " << bytes_read << std::endl;
      std::cout.write(read_buffer, bytes_read) << std::endl;

      bytes_read = read(client_req_sock_fd, read_buffer, kRead_buffer_size);
    }

    // client disconnected or has an error some other way
    if (bytes_read == 0) {
      std::clog << "CLIENT DISCONNECTED" << std::endl;
      close(client_req_sock_fd);
    } else if (bytes_read < 0) {
      std::cerr << strerror(errno) << std::endl;
      exit(EXIT_FAILURE);
    }
  }
}



const char kSocket_path[] = "srv_domain_socket";

int main(int argc, char *argv[]) {
  DomainSocketServer dss(kSocket_path);
  dss.RunServer();
  
  return 0;
}