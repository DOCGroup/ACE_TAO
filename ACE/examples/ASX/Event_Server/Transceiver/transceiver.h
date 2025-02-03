/* -*- C++ -*- */
#ifndef ACE_TRANSCEIVER_H
#define ACE_TRANSCEIVER_H

#include "ace/SOCK_Stream.h"
#include "ace/Svc_Handler.h"

// Generate and receives messages from the event server.
//
// This class is both a consumer and supplier of events, i.e.,
// it's a ``transceiver.''
class Event_Transceiver : public ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>
{
public:
  // Performs the actual initialization.
  Event_Transceiver (int argc, ACE_TCHAR *argv[]);

  // No-op constructor (required by the <ACE_Connector>).
  Event_Transceiver ();

  // = Svc_Handler hook called by the <ACE_Connector>.
  // Initialize the transceiver when we are connected.
  virtual int open (void *);

  // = Demultplexing hooks from the <ACE_Reactor>.
  // Receive data from STDIN or socket.
  virtual int handle_input (ACE_HANDLE);

  // Close down via SIGINT.
  virtual int handle_signal (int signum, siginfo_t *, ucontext_t *);

  // Close down the event loop.
  virtual int handle_close (ACE_HANDLE, ACE_Reactor_Mask);

private:
  // Reads data from socket and writes to ACE_STDOUT.
  int receiver ();

  // Writes data from ACE_STDIN to socket.
  int transmitter ();

  // Parse the command-line arguments.
  int parse_args (int argc, ACE_TCHAR *argv[]);

  // Port number of event server.
  u_short port_number_;

  // Name of event server.
  const ACE_TCHAR *host_name_;

  // Are we playing the Consumer or Supplier role?
  const ACE_TCHAR *role_;
};

#endif /* ACE_TRANSCEIVER_H */
