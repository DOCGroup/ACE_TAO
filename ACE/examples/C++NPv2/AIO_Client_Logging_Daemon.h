/*
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
  explicit AIO_Input_Handler (AIO_CLD_Acceptor *acc = nullptr)
    : acceptor_ (acc) {}

  AIO_Input_Handler (const AIO_Input_Handler &) = delete;
  AIO_Input_Handler (AIO_Input_Handler &&) = delete;
  AIO_Input_Handler &operator= (const AIO_Input_Handler &) = delete;
  AIO_Input_Handler &operator= (AIO_Input_Handler &&) = delete;

  ~AIO_Input_Handler () override;

  // Called by ACE_Asynch_Acceptor when a client connects.
  void open (ACE_HANDLE new_handle,
             ACE_Message_Block &message_block) override;

protected:
  enum { LOG_HEADER_SIZE = 8 };   // Length of CDR header
  AIO_CLD_Acceptor *acceptor_;    // Our creator
  ACE_Message_Block_Unique_Ptr mblk_ {}; // Block to receive log record
  ACE_Asynch_Read_Stream reader_; // Async read factory

  // Handle input from logging clients.
  void handle_read_stream (const ACE_Asynch_Read_Stream::Result &result) override;
};


class AIO_Output_Handler
  : public ACE_Task<ACE_NULL_SYNCH>,
    public ACE_Service_Handler {
public:
  AIO_Output_Handler () : can_write_ (false) {}

  AIO_Output_Handler (const AIO_Output_Handler &) = delete;
  AIO_Output_Handler (AIO_Output_Handler &&) = delete;
  AIO_Output_Handler &operator= (const AIO_Output_Handler &) = delete;
  AIO_Output_Handler &operator= (AIO_Output_Handler &&) = delete;

  ~AIO_Output_Handler () override;

  // Entry point into the <AIO_Output_Handler>.
  int put (ACE_Message_Block *, ACE_Time_Value * = nullptr) override;

  // Hook method called when server connection is established.
  void open (ACE_HANDLE new_handle,
             ACE_Message_Block &message_block) override;

  // A trivial implementation that is needed to suppress compile
  // warnings/errors.
  int open (void *args) override
  {
    return ACE_Task<ACE_NULL_SYNCH>::open (args);
  }

protected:
  ACE_Asynch_Read_Stream  reader_;   // Detects connection loss
  ACE_Asynch_Write_Stream writer_;   // Sends to server
  bool can_write_;   // Safe to begin send a log record?

  // Initiate the send of a log record
  void start_write (ACE_Message_Block *mblk = nullptr);

  // Handle disconnects from the logging server.
  void handle_read_stream (const ACE_Asynch_Read_Stream::Result &result) override;

  // Handle completed write to logging server.
  void handle_write_stream (const ACE_Asynch_Write_Stream::Result &result) override;
};

using OUTPUT_HANDLER = ACE_Unmanaged_Singleton<AIO_Output_Handler, ACE_Null_Mutex>;


class AIO_CLD_Connector
  : public ACE_Asynch_Connector<AIO_Output_Handler> {
public:
  enum { INITIAL_RETRY_DELAY = 3, MAX_RETRY_DELAY = 60 };

  // Constructor.
  AIO_CLD_Connector ()
    : retry_delay_ (INITIAL_RETRY_DELAY), ssl_ctx_ (nullptr), ssl_ (nullptr)
  { open (); }

  AIO_CLD_Connector (const AIO_CLD_Connector &) = delete;
  AIO_CLD_Connector (AIO_CLD_Connector &&) = delete;
  AIO_CLD_Connector &operator= (const AIO_CLD_Connector &) = delete;
  AIO_CLD_Connector &operator= (AIO_CLD_Connector &&) = delete;

  // Destructor frees the SSL resources.
  ~AIO_CLD_Connector () override {
    SSL_free (ssl_);
    SSL_CTX_free (ssl_ctx_);
    proactor ()->cancel_timer (*this);
  }

  // Hook method to detect failure and validate peer before
  // opening handler.
  int validate_connection
    (const ACE_Asynch_Connect::Result& result,
     const ACE_INET_Addr &remote, const ACE_INET_Addr& local) override;

  // Re-establish a connection to the logging server.
  int reconnect () { return connect (remote_addr_); }

protected:
  // Hook method called on timer expiration - retry connect
  void handle_time_out (const ACE_Time_Value&, const void *) override;

  // Template method to create a new handler
  AIO_Output_Handler *make_handler () override
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

using CLD_CONNECTOR = ACE_Unmanaged_Singleton<AIO_CLD_Connector, ACE_Null_Mutex>;

#endif /* _AIO_CLIENT_LOGGING_DAEMON_H */
