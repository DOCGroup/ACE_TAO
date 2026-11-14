// -*- C++ -*-
// This file defines the Options class for SSL-server-fancy.
// IBM C++ compiler'd template auto-instantiator needs this in a separate file.

#ifndef __ACE_SSL_SERVER_FANCY_H
#define __ACE_SSL_SERVER_FANCY_H

/// Defines the options for this test.
class Options
{
public:
  Options ();

  ~Options ();

  /// Parses the command-line arguments.
  int parse_args (int argc, ACE_TCHAR *argv[]);

  /// Returns whether verbose mode is enabled.
  int verbose () const;

  /// Returns the listening port.
  u_short port () const;

  /// Returns the reply message size.
  int reply_message_len () const;

private:
  /// Whether verbose mode is enabled.
  int verbose_;

  /// Listening port.
  u_short port_;

  /// Reply message size.
  size_t reply_message_len_;
};

#endif /* __ACE_SSL_SERVER_FANCY_H */
