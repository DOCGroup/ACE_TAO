// $Id$

// This file defines the Options class for CPP-inserver-fancy.
// IBM C++ compiler'd template auto-instantiator needs this in a separate file.

#ifndef __CPP_INSERVER_FANCY_H
#define __CPP_INSERVER_FANCY_H

class Options
  // = TITLE
  //   Define the options for this test.
{
public:
  Options (void);
  // Constructor.

  ~Options (void);
  // Destructor.

  int parse_args (int argc, ACE_TCHAR *argv[]);
  // Parse the command-line arguments.

  int verbose (void) const;
  // Are we running in verbose mode?

  u_short port (void) const;
  // Port number that we are listening at.

  int reply_message_len (void) const;
  // Size of the reply message.

private:
  int verbose_;
  // Are we running in verbose mode?

  u_short port_;
  // Port number we listen at.

  size_t reply_message_len_;
  // Size of the reply message.
};

#endif /* __CPP_INSERVER_FANCY_H */
