/*
** $Id$
**
** Copyright 2002 Addison Wesley. All Rights Reserved.
*/

#include "ace/OS.h"
#include "ace/Asynch_Acceptor.h"
#include "ace/Asynch_Connector.h"
#include "ace/Asynch_IO.h"
#include "ace/CDR_Stream.h"
#include "ace/Get_Opt.h"
#include "ace/INET_Addr.h"
#include "ace/Message_Block.h"
#include "ace/Proactor.h"
#include "ace/SOCK_Stream.h"
#include "ace/Service_Object.h"
#include "ace/Signal.h"
#include "ace/Singleton.h"
#include "ace/Synch.h"
#include "ace/Task.h"
#include "ace/Thread_Manager.h"
#include "ace/Unbounded_Set.h"
#include "AIO_CLD_export.h"
#include <openssl/ssl.h>


class AIO_Output_Handler
  : public ACE_Task<ACE_NULL_SYNCH>,
    public ACE_Service_Handler {
public:
  AIO_Output_Handler () : can_write_ (0) {}

  virtual ~AIO_Output_Handler ();

  // Entry point into the <AIO_Output_Handler>.
  virtual int put (ACE_Message_Block *, ACE_Time_Value * = 0);

  // Hook method called when server connection is established.
  virtual void open (ACE_HANDLE new_handle,
                     ACE_Message_Block &message_block);

protected:
  ACE_Asynch_Read_Stream  reader_;
  ACE_Asynch_Write_Stream writer_;
  int can_write_;

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


class AIO_CLD_Acceptor;

class AIO_Input_Handler : public ACE_Service_Handler {
public:
  AIO_Input_Handler (AIO_CLD_Acceptor *acc = 0)
    : acceptor_ (acc), mblk_ (0) {}

  // Cancel I/O and Close socket and release msg block
  // Remove from the list of input handlers?
  virtual ~AIO_Input_Handler ();

  // Called by ACE_Asynch_Acceptor when a client connects.
  virtual void open (ACE_HANDLE new_handle,
                     ACE_Message_Block &message_block);

protected:
  AIO_CLD_Acceptor *acceptor_;
  ACE_Message_Block *mblk_;
  ACE_Asynch_Read_Stream  reader_;

  // Handle input from logging clients.
  virtual void handle_read_stream
    (const ACE_Asynch_Read_Stream::Result &result);
};


class AIO_CLD_Acceptor
  : public ACE_Asynch_Acceptor<AIO_Input_Handler> {
public:
  // Cancel accept and close all clients.
  void close (void);

  // Remoce handler from client set.
  void remove (AIO_Input_Handler *ih)
    { clients_.remove (ih); return; }

protected:
  // Service handler factory method.
  virtual AIO_Input_Handler *make_handler (void);

  ACE_Unbounded_Set<AIO_Input_Handler *> clients_;
};


class AIO_CLD_Connector
  : public ACE_Asynch_Connector<AIO_Output_Handler> {
public:
  typedef ACE_Asynch_Connector<AIO_Output_Handler> PARENT;

  enum { INITIAL_RETRY_DELAY = 3, MAX_RETRY_DELAY = 60 };

  // Constructor.
  AIO_CLD_Connector ()
    : retry_delay_ (3), ssl_ctx_ (0), ssl_ (0)
  { open (0, ACE_Proactor::instance (), 1); }
  //    Pass addr                     Validate

  // Destructor frees the SSL resources.
  virtual ~AIO_CLD_Connector (void) {
    SSL_free (ssl_);
    SSL_CTX_free (ssl_ctx_);
    proactor ()->cancel_timer (*this);
  }

  // Hook method to validate peer before opening handler.
  virtual int validate_new_connection
    (const ACE_INET_Addr &remote_address);

  // Re-establish a connection to the logging server.
  int reconnect (void) { return connect (remote_addr_); }

protected:
  // Hook method called on connect completion.
  virtual void handle_connect
    (const ACE_Asynch_Connect::Result &result);

  // Hook method called on timer expiration - retry connect
  virtual void handle_time_out
    (const ACE_Time_Value&, const void *);

  // Template method to create a new handler
  virtual AIO_Output_Handler *make_handler (void)
    { return OUTPUT_HANDLER::instance (); }

  // Handle for new connection
  ACE_HANDLE  new_handle_;

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


class AIO_Client_Logging_Daemon : public ACE_Task<ACE_NULL_SYNCH> {
protected:
  ACE_INET_Addr cld_addr_;
  ACE_INET_Addr sld_addr_;

  // Factory that passively connects the <AIO_Input_Handler>.
  AIO_CLD_Acceptor acceptor_;

public:
  // Service Configurator hook methods.
  virtual int init (int argc, ACE_TCHAR *argv[]);
  virtual int fini ();
  virtual int svc (void);
};

/******************************************************/

AIO_Output_Handler::~AIO_Output_Handler () {
  reader_.cancel ();
  writer_.cancel ();
  ACE_OS::closesocket (handle ());
}

int AIO_Output_Handler::put (ACE_Message_Block *mb,
                             ACE_Time_Value *timeout) {
  if (can_write_) { start_write (mb); return 0; }
  return putq (mb, timeout);
}

void AIO_Output_Handler::open
  (ACE_HANDLE new_handle, ACE_Message_Block &) {
  ACE_SOCK_Stream peer (new_handle);
  int bufsiz = ACE_DEFAULT_MAX_SOCKET_BUFSIZ;
  peer.set_option (SOL_SOCKET, SO_SNDBUF,
                   &bufsiz, sizeof bufsiz);

  reader_.open (*this, new_handle, 0, proactor ());
  writer_.open (*this, new_handle, 0, proactor ());

  ACE_Message_Block *mb;
  ACE_NEW (mb, ACE_Message_Block (1));
  reader_.read (*mb, 1);
  // ??? Is SIGPIPE blocking necessary/effective in async I/O?
  ACE_Sig_Action no_sigpipe ((ACE_SignalHandler) SIG_IGN);
  no_sigpipe.register_action (SIGPIPE, 0);
  can_write_ = 1;
  start_write (0);
  return;
}

void AIO_Output_Handler::start_write (ACE_Message_Block *mblk) {
  if (mblk == 0) {
    ACE_Time_Value nonblock (ACE_OS::gettimeofday ());
    getq (mblk, &nonblock);
  }
  if (mblk != 0) {
    if (writer_.write (*mblk, mblk->length ()) == 0)
      can_write_ = 0;
    else
      ungetq (mblk);
  }
  return;
}

void AIO_Output_Handler::handle_read_stream
       (const ACE_Asynch_Read_Stream::Result &result) {
  result.message_block ().release ();
  ACE_OS::closesocket (result.handle ());
  writer_.cancel ();
  can_write_ = 0;
  CLD_CONNECTOR::instance ()->reconnect ();
  return;
}

// ??? Are partial writes possible with async writes?
void AIO_Output_Handler::handle_write_stream
       (const ACE_Asynch_Write_Stream::Result &result) {
  ACE_Message_Block &mblk = result.message_block ();
  if (result.success ()) {
    can_write_ = 1;
    if (mblk.length () == 0) {
      mblk.release ();
      start_write ();
    }
    else start_write (&mblk);
  }
  else ungetq (&mblk);
  return;
}

/******************************************************/

AIO_Input_Handler::~AIO_Input_Handler () {
  reader_.cancel ();
  ACE_OS::closesocket (handle ());
  if (mblk_ != 0) mblk_->release ();
  mblk_ = 0;
  acceptor_->remove (this);
}

void AIO_Input_Handler::open
  (ACE_HANDLE new_handle, ACE_Message_Block &) {
  reader_.open (*this, new_handle, 0, proactor ());
  ACE_NEW_NORETURN
    (mblk_, ACE_Message_Block (ACE_DEFAULT_CDR_BUFSIZE));
  // Align the Message Block for a CDR stream
  ACE_CDR::mb_align (mblk_);
  reader_.read (*mblk_, 8);
  return;
}

void AIO_Input_Handler::handle_read_stream
  (const ACE_Asynch_Read_Stream::Result &result) {
  if (!result.success ()) {
    delete this;
    return;
  }

  // Try again if not all requested bytes received.
  if (result.bytes_transferred () < result.bytes_to_read ()) {
    reader_.read (*mblk_, result.bytes_to_read () -
                          result.bytes_transferred ());
    return;
  }

  // All bytes requested are complete. If we read 8, it's the header,
  // else it's the payload.
  if (mblk_->length () == 8) {
    // Create a CDR stream to parse the 8-byte header.
    ACE_InputCDR cdr (mblk_);

    // Extract the byte-order and use helper methods to
    // disambiguate octet, booleans, and chars.
    ACE_CDR::Boolean byte_order;
    cdr >> ACE_InputCDR::to_boolean (byte_order);

    // Set the byte-order on the stream...
    cdr.reset_byte_order (byte_order);

    // Extract the length
    ACE_CDR::ULong length;
    cdr >> length;

    // Ensure there's sufficient room for log record payload.
    mblk_->size (length + 8);
    reader_.read (*mblk_, length);
    return;
  }

  if (OUTPUT_HANDLER::instance ()->put (mblk_) == -1)
    mblk_->release ();

  ACE_NEW_NORETURN
    (mblk_, ACE_Message_Block (ACE_DEFAULT_CDR_BUFSIZE));
  ACE_CDR::mb_align (mblk_);
  reader_.read (*mblk_, 8);
  return;
}

/********************************************************/

void AIO_CLD_Acceptor::close (void) {
  ACE_Unbounded_Set_Iterator<AIO_Input_Handler *>
    iter (clients_.begin ());
  AIO_Input_Handler **ih;
  while (iter.next (ih))
    delete *ih;
  return;
}

AIO_Input_Handler * AIO_CLD_Acceptor::make_handler (void) {
  AIO_Input_Handler *ih;
  ACE_NEW_RETURN (ih, AIO_Input_Handler, 0);
  if (clients_.insert (ih) == -1)
    { delete ih; return 0; }
  return ih;
}

/*******************************************************/

#if !defined (CLD_CERTIFICATE_FILENAME)
#  define CLD_CERTIFICATE_FILENAME "cld-cert.pem"
#endif /* !CLD_CERTIFICATE_FILENAME */
#if !defined (CLD_KEY_FILENAME)
#  define CLD_KEY_FILENAME "cld-key.pem"
#endif /* !CLD_KEY_FILENAME */


int AIO_CLD_Connector::validate_new_connection
  (const ACE_INET_Addr &remote_address) {

  if (ssl_ctx_ == 0) {
    OpenSSL_add_ssl_algorithms ();
    ssl_ctx_ = SSL_CTX_new (SSLv3_client_method ());
    if (ssl_ctx_ == 0) return -1;

    if (SSL_CTX_use_certificate_file (ssl_ctx_,
                                      CLD_CERTIFICATE_FILENAME,
                                      SSL_FILETYPE_PEM) <= 0
       || SSL_CTX_use_PrivateKey_file (ssl_ctx_,
                                       CLD_KEY_FILENAME,
                                       SSL_FILETYPE_PEM) <= 0
       || !SSL_CTX_check_private_key (ssl_ctx_)) {
      SSL_CTX_free (ssl_ctx_);
      ssl_ctx_ = 0;
      return -1;
    }
    ssl_ = SSL_new (ssl_ctx_);
    if (ssl_ == 0) {
      SSL_CTX_free (ssl_ctx_); ssl_ctx_ = 0;
      return -1;
    }
  }

  SSL_clear (ssl_);
  SSL_set_fd (ssl_, new_handle_);

  SSL_set_verify (ssl_, SSL_VERIFY_PEER, 0);

  if (SSL_connect (ssl_) == -1
      || SSL_shutdown (ssl_) == -1) return -1;
  remote_addr_ = remote_address;
  return 0;
}

void AIO_CLD_Connector::handle_connect
  (const ACE_Asynch_Connect::Result &result) {

  // If the connect succeeds, stash the handle for validate, above.
  // If it fails, then wait and retry.
  if (result.success () && result.error () == 0) {
    new_handle_ = result.connect_handle ();
    retry_delay_ = INITIAL_RETRY_DELAY;
    PARENT::handle_connect (result);
  }
  else {
    ACE_Time_Value delay (retry_delay_);
    retry_delay_ *= 2;
    if (retry_delay_ > MAX_RETRY_DELAY)
      retry_delay_ = MAX_RETRY_DELAY;
    proactor ()->schedule_timer (*this, 0, delay);
  }
  return;
}

void AIO_CLD_Connector::handle_time_out
(const ACE_Time_Value&, const void *) {
  connect (remote_addr_);
  return;
}

/******************************************************/

int AIO_Client_Logging_Daemon::init
      (int argc, ACE_TCHAR *argv[]) {
  u_short cld_port = ACE_DEFAULT_SERVICE_PORT;
  u_short sld_port = ACE_DEFAULT_LOGGING_SERVER_PORT;
  ACE_TCHAR sld_host[MAXHOSTNAMELEN];
  ACE_OS_String::strcpy (sld_host, ACE_LOCALHOST);

  ACE_Get_Opt get_opt (argc, argv, ACE_TEXT ("p:r:s:"), 0);
  get_opt.long_option (ACE_TEXT ("client_port"), 'p',
                       ACE_Get_Opt::ARG_REQUIRED);
  get_opt.long_option (ACE_TEXT ("server_port"), 'r',
                       ACE_Get_Opt::ARG_REQUIRED);
  get_opt.long_option (ACE_TEXT ("server_name"), 's',
                       ACE_Get_Opt::ARG_REQUIRED);

  for (int c; (c = get_opt ()) != -1;)
    switch (c) {
    case 'p': // Client logging daemon acceptor port number.
      cld_port = ACE_static_cast
        (u_short, ACE_OS::atoi (get_opt.opt_arg ()));
      break;
    case 'r': // Server logging daemon acceptor port number.
      sld_port = ACE_static_cast
        (u_short, ACE_OS::atoi (get_opt.opt_arg ()));
      break;
    case 's': // Server logging daemon hostname.
      ACE_OS_String::strsncpy
        (sld_host, get_opt.opt_arg (), MAXHOSTNAMELEN);
      break;
    }

  if (cld_addr_.set (cld_port) == -1 ||
      sld_addr_.set (sld_port, sld_host) == -1)
    return -1;
  return activate ();
}

int AIO_Client_Logging_Daemon::fini () {
  ACE_Proactor::instance ()->proactor_end_event_loop ();
  wait ();
  return 0;
}

int AIO_Client_Logging_Daemon::svc (void) {
  if (acceptor_.open (cld_addr_) == -1) return -1;
  if (CLD_CONNECTOR::instance ()->connect (sld_addr_) == 0)
    ACE_Proactor::instance ()->proactor_run_event_loop ();
  acceptor_.close ();
  CLD_CONNECTOR::close ();
  OUTPUT_HANDLER::close ();
  return 0;
}

ACE_FACTORY_DEFINE (AIO_CLD, AIO_Client_Logging_Daemon)
