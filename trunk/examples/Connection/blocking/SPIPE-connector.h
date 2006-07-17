/* -*- C++ -*- */
// $Id$

#ifndef SP_CONNECTOR_H
#define SP_CONNECTOR_H

#include "ace/Svc_Handler.h"
#include "ace/Service_Config.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/SPIPE_Stream.h"
#include "ace/Connector.h"
#include "ace/SPIPE_Connector.h"
#include "ace/Sig_Adapter.h"

class Peer_Handler : public ACE_Svc_Handler<ACE_SPIPE_STREAM, ACE_NULL_SYNCH>
{
public:
  // = Initialization

  Peer_Handler (int iterations = 0);
  // <iterations> is the number of buffers to send.  If <iterations>
  // == 0, then read from stdin.

  ~Peer_Handler (void);

  virtual int open (void * = 0);
  // Activate the handler when connection is established.

  // = Demultiplexing hooks.
  virtual int handle_input (ACE_HANDLE);
  virtual int handle_close (ACE_HANDLE handle = ACE_INVALID_HANDLE,
                            ACE_Reactor_Mask mask = ACE_Event_Handler::ALL_EVENTS_MASK);

  virtual ACE_HANDLE get_handle (void) const;

private:
  void display_menu (void);

  int iterations_;
  // No. of buffers to send.
};

class IPC_Client : public ACE_Connector<Peer_Handler, ACE_SPIPE_CONNECTOR>
{
public:
  // Initialization
  IPC_Client (void);
  ~IPC_Client (void);

  // = Dynamic linking hooks.
  virtual int init (int argc, ACE_TCHAR *argv[]);
  // Initialize the IPC client.

  virtual int fini (void);
  // Destroy the IPC client.

  virtual int svc (void);
  // Run the svc.

  virtual int handle_close (ACE_HANDLE, ACE_Reactor_Mask);
  // Report connection errors.

private:
  int parse_args (int argc, ACE_TCHAR *argv[]);
  // Parse command-line arguments.

  int iterations_;
  // Number of times to send a buffer.

  ACE_TCHAR rendezvous_[MAXPATHLEN + 1];
  // Meeting place for pipe.

  ACE_Sig_Adapter done_handler_;
  // Keeps track of when we shut down due to receipt of the SIGINT
  // signal.
};

#endif /* SP_CONNECTOR_H */
