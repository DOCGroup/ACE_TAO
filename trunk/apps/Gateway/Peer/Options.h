/* -*- C++ -*- */
// $Id$

#if !defined (OPTIONS_H)
#define OPTIONS_H

#include "ace/OS.h"

class Options
  // = TITLE
  //     Options Singleton for the peerd.
{
public:
  enum
  {
    // = Options that can be enabled/disabled.
    VERBOSE = 01,
    ACCEPTOR = 02,
    CONNECTOR = 04
  };

  static Options *instance (void);
  // Return Singleton.

  void parse_args (int argc, char *argv[]);
  // Parse the arguments.

  // = Accessor methods.
  int enabled (int option) const;
  // Determine if an option is enabled.

  u_short acceptor_port (void) const;
  // Our acceptor port number, i.e., the one that we passively listen
  // on for connections to arrive from a gatewayd.

  u_short connector_port (void) const;
  // Our connector port number, i.e., the one that we use to actively
  // establish connections with a gatewayd.

  const char *connector_host (void) const;
  // Our connector port host, i.e., the host running the gatewayd.

  long timeout (void) const;
  // Duration between disconnects.

  long max_queue_size (void) const;
  // The maximum size of the queue.

private:
  enum
  {
    MAX_QUEUE_SIZE = 1024 * 1024 * 16,
    // We'll allow up to 16 megabytes to be queued per-output
    // channel!!!!  This is clearly a policy in search of
    // refinement...

    DEFAULT_TIMEOUT = 60
    // By default, disconnect the peer every minute.
  };

  Options (void);
  // Ensure Singleton.

  static Options *instance_;
  // Singleton.

  void print_usage_and_die (void);
  // Explain usage and exit.

  u_long options_;
  // Flag to indicate if we want verbose diagnostics.

  u_short acceptor_port_;
  // Our acceptor port number, i.e., the one that we passively listen
  // on for connections to arrive from a gatewayd.

  u_short connector_port_;
  // Our connector port number, i.e., the one that we use to actively
  // establish connections with a gatewayd.

  char *connector_host_;
  // Our connector host.

  long timeout_;
  // The amount of time to wait before disconnecting from the Peerd.

  long max_queue_size_;
  // The maximum size that the queue can grow to.
};

#endif /* OPTIONS_H */
