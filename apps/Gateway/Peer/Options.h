/* -*- C++ -*- */
// $Id$

#if !defined (OPTIONS_H)
#define OPTIONS_H

#include "ace/OS.h"

class Options
  // = TITLE
  //   Options Singleton.
{
public:
  static Options *instance (void);
  // Return Singleton.

  void parse_args (int argc, char *argv[]);
  // Parse the arguments.

  // = Accessor methods.
  int verbose (void) const;
  // Are we in verbose mode?

  u_short port (void) const;
  // What is our listening port number?

  long timeout (void) const;
  // What is our timeout?

private:
  enum
  {
    DEFAULT_TIMEOUT = 60
    // By default, disconnect the peer every minute.
  }

  Options (void);
  // Ensure Singleton.

  static Options *instance_;
  // Singleton.

  void print_usage_and_die (void);
  // Explain usage and exit.

  int verbose_;
  // Flag to indicate if we want verbose diagnostics.

  u_short port_;
  // Our port number.

  long timeout_;
  // The amount of time to wait before disconnecting from the Peerd.
};

#endif /* OPTIONS_H */
