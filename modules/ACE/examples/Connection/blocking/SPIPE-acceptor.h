/* -*- C++ -*- */
// $Id$

#ifndef SP_ACCEPTOR_H
#define SP_ACCEPTOR_H

#include "ace/Svc_Handler.h"
#include "ace/Service_Config.h"

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
  Svc_Handler (void);
  ~Svc_Handler (void);

  virtual int open (void *);

  virtual void handle_read_stream (const ACE_Asynch_Read_Stream::Result &result);
  // This is called when asynchronous read from the socket complete
  // Handle data from the client.

private:
  ACE_Asynch_Read_Stream ar_;
  ACE_Message_Block mb_;
};

class IPC_Server : public ACE_Oneshot_Acceptor<Svc_Handler, ACE_SPIPE_ACCEPTOR>
{
public:
  IPC_Server (void);
  ~IPC_Server (void);

  // = Dynamic linking hooks.
  virtual int init (int argc, ACE_TCHAR *argv[]);
  // Initialize the network server.

  virtual int fini (void);
  // Close down the server.

  virtual int svc (void);
  // Run the interative service.

private:
  int parse_args (int argc, ACE_TCHAR *argv[]);
  // Parse command-line arguments.

  int n_threads_;
  // Size of thread pool to use.

  ACE_TCHAR rendezvous_[MAXPATHLEN + 1];
  // Meeting place for pipe.

  int shutdown_;
  // Keeps track of when we shut down due to receipt of the SIGINT
  // signal.

  int handle_signal (int signum, siginfo_t *, ucontext_t *);
  // Signal handler method.
};

#endif /* ACE_HAS_WIN32_OVERLAPPED_IO || ACE_HAS_AIO_CALLS */
#endif /* SP_ACCEPTOR_H */

