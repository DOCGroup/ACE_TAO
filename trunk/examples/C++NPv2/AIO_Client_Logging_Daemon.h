/*
** $Id$
**
** Copyright 2002 Addison Wesley. All Rights Reserved.
*/

#ifndef _AIO_CLIENT_LOGGING_DAEMON_H
#define _AIO_CLIENT_LOGGING_DAEMON_H

#include "ace/Asynch_Connector.h"
#include "ace/Asynch_IO.h"
#include "ace/Message_Block.h"
#include "ace/Null_Mutex.h"
#include "ace/Proactor.h"
#include "ace/Singleton.h"
#include "ace/Synch_Traits.h"
#include "ace/Task.h"

#include <openssl/ssl.h>

class AIO_CLD_Acceptor;

class AIO_Input_Handler : public ACE_Service_Handler {
public:
  AIO_Input_Handler (AIO_CLD_Acceptor *acc = 0)
    : acceptor_ (acc), mblk_ (0) {}

  virtual ~AIO_Input_Handler ();

  // Called by ACE_Asynch_Acceptor when a client connects.
  virtual void open (ACE_HANDLE new_handle,
                     ACE_Message_Block &message_block);

protected:
  enum { LOG_HEADER_SIZE = 8 };   // Length of CDR header
  AIO_CLD_Acceptor *acceptor_;    // Our creator
  ACE_Message_Block *mblk_;       // Block to receive log record
  ACE_Asynch_Read_Stream reader_; // Async read factory

  // Handle input from logging clients.
  virtual void handle_read_stream
    (const ACE_Asynch_Read_Stream::Result &result);
};


class AIO_Output_Handler
  : public ACE_Task<ACE_NULL_SYNCH>,
    public ACE_Service_Handler {
public:
  AIO_Output_Handler () : can_write_ (0) {}

  virtual ~AIO_Output_Handler ();

  // Entry point into the <AIO_Output_Handler>.
  virtual int put (ACE_Message_Block *, ACE_Time_Value * = 0);

  // Hook method called when server connection is established.
  using ACE_Service_Handler::open;
  virtual void open (ACE_HANDLE new_handle,
                     ACE_Message_Block &message_block);

protected:
  ACE_Asynch_Read_Stream  reader_;   // Detects connection loss
  ACE_Asynch_Write_Stream writer_;   // Sends to server
  int can_write_;    // Safe to begin send a log record?

  // Initiate the send of a log record
  void start_write (ACE_Message_Block *mblk = 0);

  // Handle disconnects from the logging server.
  virtual void handle_read_stream
    (const ACE_Asynch_Read_Stream::Result &result);

  // Handle completed write to logging server.
  virtual void handle_write_stream
    (const ACE_Asynch_Write_Stream::Result &result);
};

typedef ACE_Unmanaged_Singleton<AIO_Output_Handler, ACE_Null_Mutex>
        OUTPUT_HANDLER;


class AIO_CLD_Connector
  : public ACE_Asynch_Connector<AIO_Output_Handler> {
public:
  enum { INITIAL_RETRY_DELAY = 3, MAX_RETRY_DELAY = 60 };

  // Constructor.
  AIO_CLD_Connector ()
    : retry_delay_ (INITIAL_RETRY_DELAY), ssl_ctx_ (0), ssl_ (0)
  { open (); }

  // Destructor frees the SSL resources.
  virtual ~AIO_CLD_Connector (void) {
    SSL_free (ssl_);
    SSL_CTX_free (ssl_ctx_);
    proactor ()->cancel_timer (*this);
  }

  // Hook method to detect failure and validate peer before
  // opening handler.
  virtual int validate_connection
    (const ACE_Asynch_Connect::Result& result,
     const ACE_INET_Addr &remote, const ACE_INET_Addr& local);

  // Re-establish a connection to the logging server.
  int reconnect (void) { return connect (remote_addr_); }

protected:
  // Hook method called on timer expiration - retry connect
  virtual void handle_time_out
    (const ACE_Time_Value&, const void *);

  // Template method to create a new handler
  virtual AIO_Output_Handler *make_handler (void)
    { return OUTPUT_HANDLER::instance (); }

  // Address at which logging server listens for connections.
  ACE_INET_Addr remote_addr_;

  // Seconds to wait before trying the next connect
  int retry_delay_;

  // The SSL "context" data structure.
  SSL_CTX *ssl_ctx_;

  // The SSL data structure corresponding to authenticated SSL
  // connections.
  SSL *ssl_;
};

typedef ACE_Unmanaged_Singleton<AIO_CLD_Connector, ACE_Null_Mutex>
        CLD_CONNECTOR;

#endif /* _AIO_CLIENT_LOGGING_DAEMON_H */
