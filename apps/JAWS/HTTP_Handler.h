// HTTP_Service.h

#ifndef HTTP_SERVICE_H_
#define HTTP_SERVICE_H_

#include "HTTP_Helpers.h"
#include "HTTP_VFS.h"

#include "ace/Svc_Handler.h"

class HTTP_Handler
  : public ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>
{

public:
  HTTP_Handler(void);

  virtual int open (void *);
  virtual int svc (void);

  //  operator ACE_SOCK_Stream & ()
  //  { return (ACE_SOCK_Stream &) this->peer(); }

private:
  int parse_request (void);
  void fix_path (char *path);

  int serve_error (int status_code);
  int serve_directory (void);
  int serve_file (HTTP_VFS_Node * &vf);

private:

  // I/O routines for the socket
  int sockgetc(void);
  char *sockgets(char *s, int n);
  int sockputc(char const c);
  int sockputs(char const *s);

private:
  // New HTTP parsing stuff.
  enum {METHODSIZ = 10, VERSIONSIZ = 10};

  char method_[METHODSIZ];
  char requestURI_[MAXPATHLEN + 1];
  char HTTPversion_[VERSIONSIZ];

private:
  char buf_[BUFSIZ];
  char path_[MAXPATHLEN + 1];

private:
  // sock I/O stuff

  char sockbuf_[BUFSIZ];
  char *sockbufp_;
  int sockbufn_;
};


#endif

/// Local Variables:
/// mode: c++
/// End:
