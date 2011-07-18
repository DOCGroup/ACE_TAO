/*
** $Id$
**
** Copyright 2002 Addison Wesley. All Rights Reserved.
*/

#include "ace/os_include/os_netdb.h"
#include "ace/OS_NS_sys_time.h"
#include "ace/OS_NS_sys_socket.h"
#include "ace/OS_NS_unistd.h"
#include "ace/OS_NS_string.h"
#include "ace/Event_Handler.h"
#include "ace/INET_Addr.h"
#include "ace/Get_Opt.h"
#include "ace/Log_Record.h"
#include "ace/Truncate.h"
#include "ace/Message_Block.h"
#include "ace/Message_Queue.h"
#include "ace/Reactor.h"
#include "ace/Service_Object.h"
#include "ace/Signal.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/SOCK_Connector.h"
#include "ace/SOCK_Stream.h"
#include "ace/Thread_Manager.h"
#include "Logging_Acceptor.h"
#include "CLD_export.h"

#if !defined (FLUSH_TIMEOUT)
#define FLUSH_TIMEOUT 120 /* 120 seconds == 2 minutes. */
#endif /* FLUSH_TIMEOUT */


class CLD_Connector;

class CLD_Handler : public ACE_Event_Handler {
public:
  enum { QUEUE_MAX = sizeof (ACE_Log_Record) * ACE_IOV_MAX };

  //FUZZ: disable check_for_lack_ACE_OS
  // Initialization hook method.
  virtual int open (CLD_Connector *);
  virtual int close (); // Shut down hook method.
  //FUZZ: enable check_for_lack_ACE_OS

  // Accessor to the connection to the logging server.
  virtual ACE_SOCK_Stream &peer () { return peer_; }

  // Reactor hook methods.
  virtual int handle_input (ACE_HANDLE handle);
  virtual int handle_close (ACE_HANDLE = ACE_INVALID_HANDLE,
                            ACE_Reactor_Mask = 0);

protected:
  // Forward log records to the server logging daemon.
  virtual ACE_THR_FUNC_RETURN forward ();

  //FUZZ: disable check_for_lack_ACE_OS
  // Send the buffered log records using a gather-write operation.
  virtual int send (ACE_Message_Block *chunk[], size_t &count);
  //FUZZ: enable check_for_lack_ACE_OS

  // Entry point into forwarder thread of control.
  static ACE_THR_FUNC_RETURN run_svc (void *arg);

  // A synchronized <ACE_Message_Queue> that queues messages.
  ACE_Message_Queue<ACE_SYNCH> msg_queue_;

  // Manage the forwarder thread.
  ACE_Thread_Manager thr_mgr_;

  // Pointer to our <CLD_Connector>.
  CLD_Connector *connector_;

  // Connection to the logging server.
  ACE_SOCK_Stream peer_;
};


class CLD_Connector {
public:
  //FUZZ: disable check_for_lack_ACE_OS
  // Establish a connection to the logging server
  // at the <remote_addr>.
  int connect (CLD_Handler *handler,
               const ACE_INET_Addr &remote_addr);
  //FUZZ: enable check_for_lack_ACE_OS

  // Re-establish a connection to the logging server.
  int reconnect ();

private:
  // Pointer to the <CLD_Handler> that we're connecting.
  CLD_Handler *handler_;

  // Address at which the logging server is listening
  // for connections.
  ACE_INET_Addr remote_addr_;
};


class CLD_Acceptor : public ACE_Event_Handler {
public:
  //FUZZ: disable check_for_lack_ACE_OS
  // Initialization hook method.
  virtual int open (CLD_Handler *, const ACE_INET_Addr &,
                    ACE_Reactor * = ACE_Reactor::instance ());
  //FUZZ: enable check_for_lack_ACE_OS

  // Reactor hook methods.
  virtual int handle_input (ACE_HANDLE handle);
  virtual int handle_close (ACE_HANDLE = ACE_INVALID_HANDLE,
                            ACE_Reactor_Mask = 0);
  virtual ACE_HANDLE get_handle () const;

protected:
  // Factory that passively connects <ACE_SOCK_Stream>s.
  ACE_SOCK_Acceptor acceptor_;

  // Pointer to the handler of log records.
  CLD_Handler *handler_;
};

/****************************************************/

int CLD_Handler::handle_input (ACE_HANDLE handle)
{
  ACE_Message_Block *mblk = 0;
  Logging_Handler logging_handler (handle);

  if (logging_handler.recv_log_record (mblk) != -1)
    {
      if (msg_queue_.enqueue_tail (mblk->cont ()) != -1)
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


int CLD_Handler::handle_close (ACE_HANDLE handle,
                               ACE_Reactor_Mask)
{ return ACE_OS::closesocket (handle); }


int CLD_Handler::open (CLD_Connector *connector) {
  connector_ = connector;
  int bufsiz = ACE_DEFAULT_MAX_SOCKET_BUFSIZ;
  peer ().set_option (SOL_SOCKET, SO_SNDBUF,
                      &bufsiz, sizeof bufsiz);
  msg_queue_.high_water_mark (CLD_Handler::QUEUE_MAX);
  return thr_mgr_.spawn (&CLD_Handler::run_svc,
                         this, THR_SCOPE_SYSTEM);
}


ACE_THR_FUNC_RETURN CLD_Handler::run_svc (void *arg) {
  CLD_Handler *handler = static_cast<CLD_Handler *> (arg);
  return handler->forward ();
}


ACE_THR_FUNC_RETURN CLD_Handler::forward () {
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
    if (msg_queue_.dequeue_head (mblk, &timeout) == -1) {
      if (errno != EWOULDBLOCK) break;
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

  msg_queue_.close ();
  no_sigpipe.restore_action (SIGPIPE, original_action);
  return 0;
}


int CLD_Handler::send (ACE_Message_Block *chunk[], size_t &count) {
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


int CLD_Handler::close () {
  ACE_Message_Block *shutdown_message = 0;
  ACE_NEW_RETURN
    (shutdown_message,
     ACE_Message_Block (0, ACE_Message_Block::MB_STOP), -1);
  msg_queue_.enqueue_tail (shutdown_message);
  return thr_mgr_.wait ();
}

/**************************************************************/


int CLD_Acceptor::open (CLD_Handler *handler,
                        const ACE_INET_Addr &local_addr,
                        ACE_Reactor *r) {
  reactor (r); // Store the reactor pointer.
  handler_ = handler;
  if (acceptor_.open (local_addr) == -1
      || reactor ()->register_handler
           (this, ACE_Event_Handler::ACCEPT_MASK) == -1)
    return -1;
  return 0;
}


ACE_HANDLE CLD_Acceptor::get_handle () const
{ return acceptor_.get_handle (); }


int CLD_Acceptor::handle_input (ACE_HANDLE) {
  ACE_SOCK_Stream peer_stream;
  if (acceptor_.accept (peer_stream) == -1) return -1;
  else if (reactor ()->register_handler
             (peer_stream.get_handle (),
              handler_,
              ACE_Event_Handler::READ_MASK) == -1)
    return -1;
  else return 0;
}


int CLD_Acceptor::handle_close (ACE_HANDLE, ACE_Reactor_Mask) {
  acceptor_.close ();
  handler_->close ();
  return 0;
}


/***************************************************/


int CLD_Connector::connect
  (CLD_Handler *handler,
   const ACE_INET_Addr &remote_addr) {
  ACE_SOCK_Connector connector;

  if (connector.connect (handler->peer (), remote_addr) == -1)
    return -1;
  else if (handler->open (this) == -1)
  { handler->handle_close (); return -1; }
  handler_ = handler;
  remote_addr_ = remote_addr;
  return 0;
}


int CLD_Connector::reconnect () {
  // Maximum number of times to retry connect.
  const size_t MAX_RETRIES = 5;

  ACE_SOCK_Connector connector;
  ACE_Time_Value timeout (1); // Start with 1 second timeout.
  size_t i;
  for (i = 0; i < MAX_RETRIES; ++i) {
    if (i > 0) ACE_OS::sleep (timeout);
    if (connector.connect (handler_->peer (), remote_addr_,
                           &timeout) == -1)
      timeout *= 2; // Exponential backoff.
    else {
      int bufsiz = ACE_DEFAULT_MAX_SOCKET_BUFSIZ;
      handler_->peer ().set_option (SOL_SOCKET, SO_SNDBUF,
                                    &bufsiz, sizeof bufsiz);
      break;
    }
  }
  return i == MAX_RETRIES ? -1 : 0;
}


/*******************************************************/

class Client_Logging_Daemon : public ACE_Service_Object {
public:
  virtual ~Client_Logging_Daemon () {}    // Turn off g++ warnings.

  // Service Configurator hook methods.
  virtual int init (int argc, ACE_TCHAR *argv[]);
  virtual int fini ();
#if 0
  // Implementing these methods is left as an exercise for the reader.
  virtual int info (ACE_TCHAR **bufferp, size_t length = 0) const;
  virtual int suspend ();
  virtual int resume ();
#endif

protected:
  // Receives, processes, and forwards log records.
  CLD_Handler handler_;

  // Factory that passively connects the <CLD_Handler>.
  CLD_Acceptor acceptor_;

  // Factory that actively connects the <CLD_Handler>.
  CLD_Connector connector_;
};


int Client_Logging_Daemon::init (int argc, ACE_TCHAR *argv[]) {
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

  if (acceptor_.open (&handler_, cld_addr) == -1)
    return -1;
  else if (connector_.connect (&handler_, sld_addr) == -1)
  { acceptor_.handle_close (); return -1; }
  return 0;
}


int Client_Logging_Daemon::fini () {
  acceptor_.handle_close ();
  handler_.close ();
  return 0;
}


ACE_FACTORY_DEFINE (CLD, Client_Logging_Daemon)
