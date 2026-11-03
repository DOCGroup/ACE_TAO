#include "connection.h"
#include <string>

#ifndef _D_URL
#define _D_URL
class URL {
public:
  explicit URL (char const *buffer);

  std::string const &get_protocol () const;
  std::string const &get_hostname () const;
  std::string const &get_filename () const;

private:
  std::string protocol_;
  std::string hostname_;
  std::string filename_;
};

void cleanup();
void sigint(int);
int copier(connection in);

#define INDEX_NAME "/index.html"
#define INCOMING_FILE_NAME "/tmp/sumedh.web.inc"
#define TEMPORARY_FILE_NAME "/tmp/sumedh.web.tmp"
#endif



