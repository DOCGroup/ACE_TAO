/* -*- C++ -*- */
#ifndef SP_ACCEPTOR_H
#define SP_ACCEPTOR_H

#include "ace/Svc_Handler.h"
#include "ace/Service_Config.h"
#include <string>

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Acceptor.h"
#include "ace/SPIPE_Stream.h"
#include "ace/SPIPE_Acceptor.h"
#include "ace/Asynch_IO.h"

// This only works on Win32 platforms and on Unix platforms
// supporting POSIX aio calls.
#if defined (ACE_HAS_WIN32_OVERLAPPED_IO) || defined (ACE_HAS_AIO_CALLS)

// This is the class that does the work once the ACE_Oneshot_Acceptor
// has accepted a connection.

class Svc_Handler : public ACE_Svc_Handler <ACE_SPIPE_STREAM, ACE_NULL_SYNCH>, public ACE_Handler
{
public:
  Svc_Handler ();
  ~Svc_Handler () override = default;
  Svc_Handler (Svc_Handler const &) = delete;
  Svc_Handler (Svc_Handler &&) = delete;
  Svc_Handler &operator= (Svc_Handler const &) = delete;
  Svc_Handler &operator= (Svc_Handler &&) = delete;

  int open (void *) override;

  /// This is called when asynchronous read from the socket complete
  /// Handle data from the client.
  void handle_read_stream (const ACE_Asynch_Read_Stream::Result &result) override;

private:
  ACE_Asynch_Read_Stream ar_;
  ACE_Message_Block mb_;
};

class IPC_Server : public ACE_Oneshot_Acceptor<Svc_Handler, ACE_SPIPE_ACCEPTOR>
{
public:
  IPC_Server () = default;
  ~IPC_Server () override = default;
  IPC_Server (IPC_Server const &) = delete;
  IPC_Server (IPC_Server &&) = delete;
  IPC_Server &operator= (IPC_Server const &) = delete;
  IPC_Server &operator= (IPC_Server &&) = delete;

  /// @name Dynamic linking hooks
  /// @{
  /// Initialize the network server.
  int init (int argc, ACE_TCHAR *argv[]) override;

  /// Close down the server.
  int fini () override;
  /// @}

  /// Run the interative service.
  virtual int svc ();

private:
  /// Parse command-line arguments.
  int parse_args (int argc, ACE_TCHAR *argv[]);

  /// Size of thread pool to use.
  int n_threads_ {1};

  /// Meeting place for pipe.
  std::basic_string<ACE_TCHAR> rendezvous_ {ACE_TEXT ("acepipe")};

  /// Keeps track of when we shut down due to receipt of the SIGINT
  /// signal.
  int shutdown_ {};

  /// Signal handler method.
  int handle_signal (int signum, siginfo_t *, ucontext_t *) override;
};

#endif /* ACE_HAS_WIN32_OVERLAPPED_IO || ACE_HAS_AIO_CALLS */
#endif /* SP_ACCEPTOR_H */

