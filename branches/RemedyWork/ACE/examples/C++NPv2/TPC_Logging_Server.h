/*
** $Id$
**
** This is the Thread-per-connection logging server example from Chapter 7.
**
** Copyright 2002 Addison Wesley. All Rights Reserved.
*/

#ifndef _TPC_LOGGING_SERVER_H
#define _TPC_LOGGING_SERVER_H

#include "ace/Acceptor.h"
#include "ace/INET_Addr.h"
#include "ace/Reactor.h"
#include "ace/Svc_Handler.h"
#include "ace/FILE_IO.h"
#include "Logging_Handler.h"

#include "ace/SOCK_Acceptor.h"
#include <openssl/ssl.h>

class TPC_Logging_Handler
  : public ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH> {
protected:
  ACE_FILE_IO log_file_; // File of log records.

  // Connection to peer service handler.
  Logging_Handler logging_handler_;

public:
  TPC_Logging_Handler () : logging_handler_ (log_file_) {}

  virtual int open (void *);

  virtual int svc (void) {
    for (;;)
      switch (logging_handler_.log_record ()) {
      case -1: return -1; // Error.
      case 0: return 0;   // Client closed connection.
      default: continue;  // Default case.
      }
    /* NOTREACHED */
    return 0;
  }
};


class TPC_Logging_Acceptor
  : public ACE_Acceptor<TPC_Logging_Handler, ACE_SOCK_ACCEPTOR> {
protected:
  // The SSL ``context'' data structure.
  SSL_CTX *ssl_ctx_;

  // The SSL data structure corresponding to authenticated
  // SSL connections.
  SSL *ssl_;

public:
  typedef ACE_Acceptor<TPC_Logging_Handler, ACE_SOCK_ACCEPTOR>
          PARENT;
  typedef ACE_SOCK_Acceptor::PEER_ADDR PEER_ADDR;

  // Constructor.
  TPC_Logging_Acceptor (ACE_Reactor *r)
    : PARENT (r), ssl_ctx_ (0), ssl_ (0) {}

  // Destructor frees the SSL resources.
  virtual ~TPC_Logging_Acceptor (void) {
    SSL_free (this->ssl_);
    SSL_CTX_free (this->ssl_ctx_);
  }

  // Initialize the acceptor instance.
  virtual int open
    (const ACE_SOCK_Acceptor::PEER_ADDR &local_addr,
     ACE_Reactor *reactor = ACE_Reactor::instance (),
     int flags = 0, int use_select = 1, int reuse_addr = 1);

  virtual int handle_close
    (ACE_HANDLE = ACE_INVALID_HANDLE,
     ACE_Reactor_Mask = ACE_Event_Handler::ALL_EVENTS_MASK);

  // Connection establishment and authentication hook method.
  virtual int accept_svc_handler (TPC_Logging_Handler *sh);
};

#endif /* _TPC_LOGGING_SERVER_H */
