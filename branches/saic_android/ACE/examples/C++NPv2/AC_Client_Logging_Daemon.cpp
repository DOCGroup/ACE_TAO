/*
** $Id$
**
** Copyright 2002 Addison Wesley. All Rights Reserved.
*/

#include "ace/OS_NS_string.h"
#include "ace/OS_NS_unistd.h"
#include "ace/OS_NS_sys_socket.h"
#include "ace/OS_NS_sys_time.h"
#include "ace/INET_Addr.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/SOCK_Connector.h"
#include "ace/SOCK_Stream.h"
#include "ace/Acceptor.h"
#include "ace/Connector.h"
#include "ace/Get_Opt.h"
#include "ace/Handle_Set.h"
#include "ace/Log_Record.h"
#include "ace/Truncate.h"
#include "ace/Message_Block.h"
#include "ace/Reactor.h"
#include "ace/Service_Object.h"
#include "ace/Signal.h"
#include "ace/Svc_Handler.h"
#include "ace/Thread_Manager.h"
#include "ace/os_include/os_netdb.h"
#include "Logging_Handler.h"
#include "AC_CLD_export.h"

#include "AC_Client_Logging_Daemon.h"

#include <openssl/ssl.h>


class AC_CLD_Acceptor
  : public ACE_Acceptor<AC_Input_Handler, ACE_SOCK_ACCEPTOR> {
public:
  // Constructor.
  AC_CLD_Acceptor (AC_Output_Handler *handler = 0)
    : output_handler_ (handler), input_handler_ (handler) {}

protected:
  typedef ACE_Acceptor<AC_Input_Handler, ACE_SOCK_ACCEPTOR>
          PARENT;

  // <ACE_Acceptor> factory method.
  virtual int make_svc_handler (AC_Input_Handler *&sh);

  // <ACE_Reactor> close hook method.
  virtual int handle_close (ACE_HANDLE = ACE_INVALID_HANDLE,
                            ACE_Reactor_Mask = 0);

  // Pointer to the output handler.
  AC_Output_Handler *output_handler_;

  // Single input handler.
  AC_Input_Handler input_handler_;
};

class AC_CLD_Connector
  : public ACE_Connector<AC_Output_Handler, ACE_SOCK_CONNECTOR> {
public:
  typedef ACE_Connector<AC_Output_Handler, ACE_SOCK_CONNECTOR>
          PARENT;

  // Constructor.
  AC_CLD_Connector (AC_Output_Handler *handler = 0)
    : handler_ (handler), ssl_ctx_ (0), ssl_ (0) {}

  // Destructor frees the SSL resources.
  virtual ~AC_CLD_Connector (void) {
    SSL_free (ssl_);
    SSL_CTX_free (ssl_ctx_);
  }

  //FUZZ: disable check_for_lack_ACE_OS
  // Initialize the Connector.
  virtual int open (ACE_Reactor *r = ACE_Reactor::instance (),
                    int flags = 0);
  //FUZZ: enable check_for_lack_ACE_OS

  // Re-establish a connection to the logging server.
  int reconnect ();

protected:
  // Connection establishment and authentication hook method.
  virtual int connect_svc_handler
    (AC_Output_Handler *&svc_handler,
     const ACE_SOCK_Connector::PEER_ADDR &remote_addr,
     ACE_Time_Value *timeout,
     const ACE_SOCK_Connector::PEER_ADDR &local_addr,
     int reuse_addr, int flags, int perms);

  virtual int connect_svc_handler
    (AC_Output_Handler *&svc_handler,
     AC_Output_Handler *&sh_copy,
     const ACE_SOCK_Connector::PEER_ADDR &remote_addr,
     ACE_Time_Value *timeout,
     const ACE_SOCK_Connector::PEER_ADDR &local_addr,
     int reuse_addr, int flags, int perms);

  // Pointer to <AC_Output_Handler> we're connecting.
  AC_Output_Handler *handler_;

  // Address at which logging server listens for connections.
  ACE_INET_Addr remote_addr_;

  // The SSL "context" data structure.
  SSL_CTX *ssl_ctx_;

  // The SSL data structure corresponding to authenticated SSL
  // connections.
  SSL *ssl_;
};

class AC_Client_Logging_Daemon : public ACE_Service_Object {
protected:
  // Factory that passively connects the <AC_Input_Handler>.
  AC_CLD_Acceptor acceptor_;

  // Factory that actively connects the <AC_Output_Handler>.
  AC_CLD_Connector connector_;

  // The <AC_Output_Handler> connected by <AC_CLD_Connector>.
  AC_Output_Handler output_handler_;

public:
  // Constructor.
  AC_Client_Logging_Daemon ()
    : acceptor_ (&output_handler_),
      connector_ (&output_handler_) {}

  // Service Configurator hook methods.
  virtual int init (int argc, ACE_TCHAR *argv[]);
  virtual int fini ();
  //virtual int info (ACE_TCHAR **bufferp, size_t length = 0) const;
  //virtual int suspend ();
  //virtual int resume ();
};

/******************************************************/

#if !defined (FLUSH_TIMEOUT)
#define FLUSH_TIMEOUT 120 /* 120 seconds == 2 minutes. */
#endif /* FLUSH_TIMEOUT */

int AC_Output_Handler::open (void *connector) {
  connector_ =
    static_cast<AC_CLD_Connector *> (connector);
  int bufsiz = ACE_DEFAULT_MAX_SOCKET_BUFSIZ;
  peer ().set_option (SOL_SOCKET, SO_SNDBUF,
                      &bufsiz, sizeof bufsiz);
  if (reactor ()->register_handler
       (this, ACE_Event_Handler::READ_MASK) == -1)
    return -1;
  if (msg_queue ()->activate ()
      == ACE_Message_Queue_Base::ACTIVATED) {
    msg_queue ()->high_water_mark (QUEUE_MAX);
    return activate (THR_SCOPE_SYSTEM);
  } else return 0;
}

int AC_Output_Handler::put (ACE_Message_Block *mb,
                            ACE_Time_Value *timeout) {
  int retval;
  while ((retval = putq (mb, timeout)) == -1) {
    if (msg_queue ()->state () != ACE_Message_Queue_Base::PULSED)
      break;
  }
  return retval;
}

int AC_Output_Handler::handle_input (ACE_HANDLE h) {
  peer ().close ();
  reactor ()->remove_handler
    (h, ACE_Event_Handler::READ_MASK
        | ACE_Event_Handler::DONT_CALL);
  msg_queue ()->pulse ();
  return 0;
}

int AC_Output_Handler::svc () {
  ACE_Message_Block *chunk[ACE_IOV_MAX];
  size_t message_index = 0;
  ACE_Time_Value time_of_last_send (ACE_OS::gettimeofday ());
  ACE_Time_Value timeout;
  ACE_Sig_Action no_sigpipe ((ACE_SignalHandler) SIG_IGN);
  ACE_Sig_Action original_action;
  no_sigpipe.register_action (SIGPIPE, &original_action);

  for (;;) {
    if (message_index == 0) {
      timeout = ACE_OS::gettimeofday ();
      timeout += FLUSH_TIMEOUT;
    }
    ACE_Message_Block *mblk = 0;
    if (getq (mblk, &timeout) == -1) {
      if (errno == ESHUTDOWN) {
        if (connector_->reconnect () == -1) break;
        continue;
      } else if (errno != EWOULDBLOCK) break;
      else if (message_index == 0) continue;
    } else {
      if (mblk->size () == 0
          && mblk->msg_type () == ACE_Message_Block::MB_STOP)
        { mblk->release (); break; }
      chunk[message_index] = mblk;
      ++message_index;
    }
    if (message_index >= ACE_IOV_MAX ||
        (ACE_OS::gettimeofday () - time_of_last_send
         >= ACE_Time_Value(FLUSH_TIMEOUT))) {
      if (this->send (chunk, message_index) == -1) break;
      time_of_last_send = ACE_OS::gettimeofday ();
    }
  }

  if (message_index > 0)
    this->send (chunk, message_index);
  no_sigpipe.restore_action (SIGPIPE, original_action);
  return 0;
}

int AC_Output_Handler::send (ACE_Message_Block *chunk[], size_t &count) {
  iovec iov[ACE_IOV_MAX];
  size_t iov_size;
  int result = 0;

  for (iov_size = 0; iov_size < count; ++iov_size) {
    iov[iov_size].iov_base = chunk[iov_size]->rd_ptr ();
    iov[iov_size].iov_len =
      ACE_Utils::truncate_cast<u_long> (chunk[iov_size]->length ());
  }
  while (peer ().sendv_n (iov, ACE_Utils::truncate_cast<int> (iov_size)) == -1)
    if (connector_->reconnect () == -1) {
      result = -1;
      break;
    }

  while (iov_size > 0) {
    chunk[--iov_size]->release (); chunk[iov_size] = 0;
  }
  count = iov_size;
  return result;
}

/******************************************************/

int AC_Input_Handler::open (void *) {
  ACE_HANDLE handle = peer ().get_handle ();
  if (reactor ()->register_handler
        (handle, this, ACE_Event_Handler::READ_MASK) == -1)
    return -1;
  connected_clients_.set_bit (handle);
  return 0;
}

int AC_Input_Handler::close (u_long) {
  ACE_Message_Block *shutdown_message = 0;
  ACE_NEW_RETURN
    (shutdown_message,
     ACE_Message_Block (0, ACE_Message_Block::MB_STOP), -1);
  output_handler_->put (shutdown_message);

  reactor ()->remove_handler
    (connected_clients_, ACE_Event_Handler::READ_MASK);
  return output_handler_->wait ();
}

int AC_Input_Handler::handle_input (ACE_HANDLE handle) {
  ACE_Message_Block *mblk = 0;
  Logging_Handler logging_handler (handle);

  if (logging_handler.recv_log_record (mblk) != -1)
    {
      if (output_handler_->put (mblk->cont ()) != -1)
        {
          mblk->cont (0);
          mblk->release ();
          return 0; // Success return.
        }
      else
        {
          mblk->release ();
        }
    }

  return -1; // Error return.
}

int AC_Input_Handler::handle_close (ACE_HANDLE handle,
                                    ACE_Reactor_Mask) {
  connected_clients_.clr_bit (handle);
  return ACE_OS::closesocket (handle);
}

/********************************************************/

int AC_CLD_Acceptor::make_svc_handler (AC_Input_Handler *&sh)
{ sh = &input_handler_; return 0; }


int AC_CLD_Acceptor::handle_close (ACE_HANDLE,
                                   ACE_Reactor_Mask) {
  PARENT::handle_close ();
  input_handler_.close ();
  return 0;
}

/*******************************************************/

#if !defined (CLD_CERTIFICATE_FILENAME)
#  define CLD_CERTIFICATE_FILENAME "cld-cert.pem"
#endif /* !CLD_CERTIFICATE_FILENAME */
#if !defined (CLD_KEY_FILENAME)
#  define CLD_KEY_FILENAME "cld-key.pem"
#endif /* !CLD_KEY_FILENAME */

int AC_CLD_Connector::open (ACE_Reactor *r, int flags) {
  if (PARENT::open (r, flags) != 0) return -1;
  OpenSSL_add_ssl_algorithms ();
  ssl_ctx_ = SSL_CTX_new (SSLv3_client_method ());
  if (ssl_ctx_ == 0) return -1;

  if (SSL_CTX_use_certificate_file (ssl_ctx_,
                                    CLD_CERTIFICATE_FILENAME,
                                    SSL_FILETYPE_PEM) <= 0
     || SSL_CTX_use_PrivateKey_file (ssl_ctx_,
                                     CLD_KEY_FILENAME,
                                     SSL_FILETYPE_PEM) <= 0
     || !SSL_CTX_check_private_key (ssl_ctx_))
    return -1;

  ssl_ = SSL_new (ssl_ctx_);
  if (ssl_ == 0) return -1;
  return 0;
}

int AC_CLD_Connector::connect_svc_handler
    (AC_Output_Handler *&svc_handler,
     const ACE_SOCK_Connector::PEER_ADDR &remote_addr,
     ACE_Time_Value *timeout,
     const ACE_SOCK_Connector::PEER_ADDR &local_addr,
     int reuse_addr, int flags, int perms) {
  if (PARENT::connect_svc_handler
      (svc_handler, remote_addr, timeout,
       local_addr, reuse_addr, flags, perms) == -1) return -1;
  SSL_clear (ssl_);
#if defined (ACE_WIN32)
  // ACE_WIN32 is the only platform where ACE_HANDLE is not an int.
  // See ace/config-lite.h for the typedefs.
  SSL_set_fd (ssl_,
              reinterpret_cast<int> (svc_handler->get_handle ()));
#else
  SSL_set_fd (ssl_, svc_handler->get_handle ());
#endif /* ACE_WIN32 */

  SSL_set_verify (ssl_, SSL_VERIFY_PEER, 0);

  if (SSL_connect (ssl_) == -1
      || SSL_shutdown (ssl_) == -1) return -1;
  remote_addr_ = remote_addr;
  return 0;
}

int AC_CLD_Connector::connect_svc_handler
    (AC_Output_Handler *&svc_handler,
     AC_Output_Handler *&sh_copy,
     const ACE_SOCK_Connector::PEER_ADDR &remote_addr,
     ACE_Time_Value *timeout,
     const ACE_SOCK_Connector::PEER_ADDR &local_addr,
     int reuse_addr, int flags, int perms) {
  sh_copy = svc_handler;
  return this->connect_svc_handler (svc_handler, remote_addr, timeout,
                                    local_addr, reuse_addr, flags, perms);
}

int AC_CLD_Connector::reconnect () {
  // Maximum number of times to retry connect.
  const size_t MAX_RETRIES = 5;
  ACE_Time_Value timeout (1);
  size_t i;
  for (i = 0; i < MAX_RETRIES; ++i) {
    ACE_Synch_Options options (ACE_Synch_Options::USE_TIMEOUT,
                               timeout);
    if (i > 0) ACE_OS::sleep (timeout);
    if (this->connect (handler_, remote_addr_, options) == 0)
      break;
    timeout *= 2; // Exponential backoff.
  }
  return i == MAX_RETRIES ? -1 : 0;
}

/******************************************************/

int AC_Client_Logging_Daemon::init
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

  ACE_INET_Addr cld_addr (cld_port);
  ACE_INET_Addr sld_addr (sld_port, sld_host);

  if (acceptor_.open (cld_addr) == -1) return -1;
  AC_Output_Handler *oh = &output_handler_;
  if (connector_.connect (oh, sld_addr) == -1)
  { acceptor_.close (); return -1; }
  return 0;
}

int AC_Client_Logging_Daemon::fini ()
{ return acceptor_.close (); }

ACE_FACTORY_DEFINE (AC_CLD, AC_Client_Logging_Daemon)
