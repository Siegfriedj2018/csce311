#include <text-server.h>
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
explicit UnixDomainSocket::UnixDomainSocket(const char *socket_path, bool abstract) {
  socket_path_ = std::string(socket_path);

  sock_addr_ = {};
  sock_addr_.sun_family = AF_UNIX;

  if (abstract)
    strncpy(sock_addr_.sun_path + 1,
            socket_path,
            sizeof(sock_addr_.sun_path) - 1);
  else
    strncpy(sock_addr_.sun_path,
            socket_path,
            sizeof(sock_addr_.sun_path));
}

DomainSocketServer::DomainSocketServer() {
  kNum_Proc_ = (get_nprocs_conf() - 1);
}

void DomainSocketServer::RunServer() const {
  int sock_fd;
  int client_req_sock_fd;

  // (1) Create a socket
  sock_fd = socket(AF_UNIX, SOCK_STREAM, 0);
  if (sock_fd < 0) {
    std::cerr << strerror(errno) << std::endl;
    exit(-1);
  }

  // (2) bind socket to address for the server
  unlink(socket_path_.c_str());

  int success = bind(sock_fd,
                     reinterpret_cast<const sockaddr *>(&sock_addr_),
                     sizeof(sock_addr_));
  
  if (success < 0) {
    std::cerr << strerror(errno) << std::endl;
    exit(-1);
  }

  // (3) listen for connections from client
  success = listen(sock_fd, kNum_Proc_);
  if (success < 0) {
    std::cerr << strerror(errno) << std::endl;
    exit(-1);
  }

  const size_t kRead_buffer_size = 32;  // read 4 byte increaments
  char read_buffer[kRead_buffer_size];
  int bytes_read;

  while (true) {
    // (4) accept connection from client
    client_req_sock_fd = accept(sock_fd, nullptr, nullptr);
    if (client_req_sock_fd < 0) {
      std::cerr << strerror(errno) << std::endl;
      exit(-1);
    }

    std::clog << "SERVER STARTED" << std::endl;
    std::clog << "\tMAX CLIENTS: " << kNum_Proc_ << std::endl;

    
  }
}