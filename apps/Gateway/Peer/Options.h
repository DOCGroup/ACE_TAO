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

  u_short port (void) const;
  // What is our listening port number?

  long timeout (void) const;
  // What is our timeout?

  long max_queue_size (void) const;
  // What is the maximum size of the queue?

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

  u_short port_;
  // Our port number.

  long timeout_;
  // The amount of time to wait before disconnecting from the Peerd.

  long max_queue_size_;
  // The maximum size that the queue can grow to.
};

#endif /* OPTIONS_H */
