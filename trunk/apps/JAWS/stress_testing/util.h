#include "connection.h"

#ifndef _D_URL
#define _D_URL
class URL {

public:

  URL(char *buffer);

  char *get_protocol();
  char *get_hostname();
  char *get_filename();

private:
  char *protocol_;
  char *hostname_;
  char *filename_;
};

void cleanup(void);
void sigint(int);
int copier(connection in);

#define INDEX_NAME "/index.html"
#define INCOMING_FILE_NAME "/tmp/sumedh.web.inc"
#define TEMPORARY_FILE_NAME "/tmp/sumedh.web.tmp"
#endif




