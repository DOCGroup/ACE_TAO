/* -*- C++ -*- */
// $Id$

#ifndef ACE_TRANSCEIVER_H
#define ACE_TRANSCEIVER_H

#include "ace/SOCK_Stream.h"
#include "ace/Svc_Handler.h"
#include "ace/Synch.h"

class Event_Transceiver : public ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>
{
  // = TITLE
  //     Generate and receives messages from the event server.
  //
  // = DESCRIPTION
  //     This class is both a consumer and supplier of events, i.e.,
  //     it's a ``transceiver.''
public:
  // = Initialization method.
  Event_Transceiver (int argc, char *argv[]);
  // Performs the actual initialization.

  Event_Transceiver (void);
  // No-op constructor (required by the <ACE_Connector>).

  // = Svc_Handler hook called by the <ACE_Connector>.
  virtual int open (void *);
  // Initialize the transceiver when we are connected.

  // = Demultplexing hooks from the <ACE_Reactor>.
  virtual int handle_input (ACE_HANDLE);
  // Receive data from STDIN or socket.

  virtual int handle_signal (int signum, siginfo_t *, ucontext_t *);
  // Close down via SIGINT.

  virtual int handle_close (ACE_HANDLE, ACE_Reactor_Mask);
  // Close down the event loop.

private:
  int receiver (void);
  // Reads data from socket and writes to ACE_STDOUT.

  int transmitter (void);
  // Writes data from ACE_STDIN to socket.

  int parse_args (int argc, char *argv[]);
  // Parse the command-line arguments.

  u_short port_number_;
  // Port number of event server.

  const char *host_name_;
  // Name of event server.

  const char *role_;
  // Are we playing the Consumer or Supplier role?
};

#endif /* ACE_TRANSCEIVER_H */
