/*
** $Id$
**
** Copyright 2002 Addison Wesley. All Rights Reserved.
*/

#include "ace/config-all.h"

#if (defined (ACE_WIN32) && !defined (ACE_HAS_WINCE)) || (defined (ACE_HAS_AIO_CALLS))

#include "ace/OS_NS_string.h"
#include "ace/OS_NS_sys_socket.h"
#include "ace/Asynch_Acceptor.h"
#include "ace/Asynch_Connector.h"
#include "ace/Asynch_IO.h"
#include "ace/CDR_Stream.h"
#include "ace/Get_Opt.h"
#include "ace/INET_Addr.h"
#include "ace/Message_Block.h"
#include "ace/Null_Condition.h"
#include "ace/Null_Mutex.h"
#include "ace/Proactor.h"
#include "ace/SOCK_Stream.h"
#include "ace/Service_Object.h"
#include "ace/Signal.h"
#include "ace/Singleton.h"
#include "ace/Task.h"
#include "ace/Thread_Manager.h"
#include "ace/Unbounded_Set.h"
#include "ace/os_include/os_netdb.h"
#include "AIO_CLD_export.h"
#include "AIO_Client_Logging_Daemon.h"
#include <openssl/ssl.h>

class AIO_CLD_Acceptor
  : public ACE_Asynch_Acceptor<AIO_Input_Handler> {
public:
  //FUZZ: disable check_for_lack_ACE_OS
  // Cancel accept and close all clients.
  void close (void);
  //FUZZ: enable check_for_lack_ACE_OS

  // Remove handler from client set.
  void remove (AIO_Input_Handler *ih)
  { clients_.remove (ih); }

protected:
  // Service handler factory method.
  virtual AIO_Input_Handler *make_handler (void);

  // Set of all connected clients
  ACE_Unbounded_Set<AIO_Input_Handler *> clients_;
};


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

  ACE_Message_Block *mb = 0;
  ACE_NEW (mb, ACE_Message_Block (1));
  reader_.read (*mb, 1);
  ACE_Sig_Action no_sigpipe ((ACE_SignalHandler) SIG_IGN);
  no_sigpipe.register_action (SIGPIPE, 0);
  can_write_ = 1;
  start_write (0);
}

void AIO_Output_Handler::start_write (ACE_Message_Block *mblk) {
  if (mblk == 0) {
    ACE_Time_Value nonblock (0);
    getq (mblk, &nonblock);
  }
  if (mblk != 0) {
    can_write_ = 0;
    if (writer_.write (*mblk, mblk->length ()) == -1)
      ungetq (mblk);
  }
}

void AIO_Output_Handler::handle_read_stream
       (const ACE_Asynch_Read_Stream::Result &result) {
  result.message_block ().release ();
  writer_.cancel ();
  ACE_OS::closesocket (result.handle ());
  handle (ACE_INVALID_HANDLE);
  can_write_ = 0;
  CLD_CONNECTOR::instance ()->reconnect ();
}

void AIO_Output_Handler::handle_write_stream
       (const ACE_Asynch_Write_Stream::Result &result) {
  ACE_Message_Block &mblk = result.message_block ();
  if (!result.success ()) {
    mblk.rd_ptr (mblk.base ());
    ungetq (&mblk);
  }
  else {
    can_write_ = handle () == result.handle ();
    if (mblk.length () == 0) {
      mblk.release ();
      if (can_write_) start_write ();
    }
    else if (can_write_) start_write (&mblk);
    else { mblk.rd_ptr (mblk.base ()); ungetq (&mblk); }
  }
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
  reader_.read (*mblk_, LOG_HEADER_SIZE);
}

void AIO_Input_Handler::handle_read_stream
    (const ACE_Asynch_Read_Stream::Result &result) {
  if (!result.success () || result.bytes_transferred () == 0)
    delete this;
  else if (result.bytes_transferred () < result.bytes_to_read ())
    reader_.read (*mblk_, result.bytes_to_read () -
                  result.bytes_transferred ());
  else if (mblk_->length () == LOG_HEADER_SIZE) {
    ACE_InputCDR cdr (mblk_);

    ACE_CDR::Boolean byte_order;
    cdr >> ACE_InputCDR::to_boolean (byte_order);
    cdr.reset_byte_order (byte_order);

    ACE_CDR::ULong length;
    cdr >> length;

    mblk_->size (length + LOG_HEADER_SIZE);
    reader_.read (*mblk_, length);
  }
  else {
    if (OUTPUT_HANDLER::instance ()->put (mblk_) == -1)
      mblk_->release ();

    ACE_NEW_NORETURN
      (mblk_, ACE_Message_Block (ACE_DEFAULT_CDR_BUFSIZE));
    ACE_CDR::mb_align (mblk_);
    reader_.read (*mblk_, LOG_HEADER_SIZE);
  }
}

/********************************************************/

void AIO_CLD_Acceptor::close (void) {
  ACE_Unbounded_Set_Iterator<AIO_Input_Handler *>
    iter (clients_.begin ());
  AIO_Input_Handler **ih;
  for (; iter.next (ih); ++iter)
    delete *ih;
}

AIO_Input_Handler * AIO_CLD_Acceptor::make_handler (void) {
  AIO_Input_Handler *ih;
  ACE_NEW_RETURN (ih, AIO_Input_Handler (this), 0);
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


int AIO_CLD_Connector::validate_connection
  (const ACE_Asynch_Connect::Result& result,
   const ACE_INET_Addr &remote, const ACE_INET_Addr&) {

  remote_addr_ = remote;
  if (!result.success ()) {
    ACE_Time_Value delay (retry_delay_);
    retry_delay_ *= 2;
    if (retry_delay_ > MAX_RETRY_DELAY)
      retry_delay_ = MAX_RETRY_DELAY;
    proactor ()->schedule_timer (*this, 0, delay);
    return -1;
  }
  retry_delay_ = INITIAL_RETRY_DELAY;

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
#if defined (ACE_WIN32)
  // ACE_WIN32 is the only platform where ACE_HANDLE is not an int.
  // See ace/config-lite.h for the typedefs.
  SSL_set_fd (ssl_, reinterpret_cast<int> (result.connect_handle ()));
#else
  SSL_set_fd (ssl_, result.connect_handle ());
#endif /* ACE_WIN32 */
  SSL_set_verify (ssl_, SSL_VERIFY_PEER, 0);

  if (SSL_connect (ssl_) == -1
      || SSL_shutdown (ssl_) == -1) return -1;
  return 0;
}

void AIO_CLD_Connector::handle_time_out
(const ACE_Time_Value&, const void *) {
  this->connect (remote_addr_);
}

/******************************************************/

int AIO_Client_Logging_Daemon::init
      (int argc, ACE_TCHAR *argv[]) {
  u_short cld_port = ACE_DEFAULT_SERVICE_PORT;
  u_short sld_port = ACE_DEFAULT_LOGGING_SERVER_PORT;
  ACE_TCHAR sld_host[MAXHOSTNAMELEN];
  ACE_OS::strcpy (sld_host, ACE_LOCALHOST);

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
      cld_port = static_cast<u_short> (ACE_OS::atoi (get_opt.opt_arg ()));
      break;
    case 'r': // Server logging daemon acceptor port number.
      sld_port = static_cast<u_short> (ACE_OS::atoi (get_opt.opt_arg ()));
      break;
    case 's': // Server logging daemon hostname.
      ACE_OS::strsncpy
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
  this->wait ();
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

#else   /* There's no AIO support on this platform */

#include "ace/Task.h"
#include "ace/Service_Object.h"
#include "ace/Synch_Traits.h"
#include "AIO_CLD_export.h"

class AIO_Client_Logging_Daemon : public ACE_Task<ACE_NULL_SYNCH> {
public:
  // Service Configurator hook methods.
  virtual int init (int, ACE_TCHAR *[]);
  virtual int fini ();
};

int AIO_Client_Logging_Daemon::init (int, ACE_TCHAR *[]) {

  ACE_ERROR_RETURN
    ((LM_ERROR, ACE_TEXT ("This service requires AIO support\n")), -1);
}

int AIO_Client_Logging_Daemon::fini () {
  return 0;
}

#endif /* (ACE_WIN32 && !ACE_HAS_WINCE) || ACE_HAS_AIO_CALLS */

ACE_FACTORY_DEFINE (AIO_CLD, AIO_Client_Logging_Daemon)


