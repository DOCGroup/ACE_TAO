/* -*- c++ -*- */

#include "ace/Connector.h"
#include "ace/SOCK_Connector.h"

class HTTP_Handler
  : public ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>
{
public:
  HTTP_Handler (void);
  HTTP_Handler (const char * path);

  virtual int open (void *);
  virtual int svc (void);

private:
  int send_n (const void *, size_t);
  int recv_n (void *, size_t);

private:
  char request_[BUFSIZ];
  int request_size_;

  char filename_[BUFSIZ];
  int response_size_;
};

class HTTP_Connector
{
public:
  int connect (const char * url);

private:
  int parseurl (const char *url,
		char *host,
		u_short *port,
		char *path);

private:
  ACE_Connector<HTTP_Handler, ACE_SOCK_CONNECTOR> connector_;
};

