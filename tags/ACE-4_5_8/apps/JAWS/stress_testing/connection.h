// $Id$

#include "global.h"

#ifndef _D_connection
#define _D_connection
class connection {

public:
  int connect(char *hostname_opt_port, int tcp_nodelay, int sockbufsiz);
  read(void *buffer, size_t maxlen, unsigned int timeout_seconds = 60);
  write(const void *buffer, size_t maxlen, unsigned int timeout_seconds = 60);
  write_n(const void *buffer, size_t len, unsigned int timeout_seconds = 60);
  read_n(void *buffer, size_t maxlen, unsigned int timeout_seconds = 60);
  int close(void);
  ~connection(void);

private:
  ACE_SOCK_Stream stream_;
  char sockbuf[66000];
};
#endif
