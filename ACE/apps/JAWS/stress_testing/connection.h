#include "global.h"

#ifndef _D_connection
#define _D_connection
class connection {
public:
  connection () = default;
  connection (connection const &) = delete;
  connection (connection &&) = delete;
  connection &operator= (connection const &) = delete;
  connection &operator= (connection &&) = delete;

  int connect (char const *hostname_opt_port, int tcp_nodelay, int sockbufsiz);
  int read(void *buffer, size_t maxlen, unsigned int timeout_seconds = 60);
  int write(const void *buffer, size_t maxlen, unsigned int timeout_seconds = 60);
  int write_n(const void *buffer, size_t len, unsigned int timeout_seconds = 60);
  int read_n(void *buffer, size_t maxlen, unsigned int timeout_seconds = 60);
  int close();
  ~connection();

private:
  ACE_SOCK_Stream stream_;
};
#endif
