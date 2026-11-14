/**
 * @file CPP-inserver-fancy.h
 *
 * Defines the CPP-inserver-fancy Options class. IBM's C++ template
 * auto-instantiator requires this class to be in a separate file.
 */

#ifndef __CPP_INSERVER_FANCY_H
#define __CPP_INSERVER_FANCY_H

/// Defines the options for the fancy server test.
class Options
{
public:
  Options ();

  ~Options ();

  /// Parses the command-line arguments.
  int parse_args (int argc, ACE_TCHAR *argv[]);

  /// Returns whether verbose output is enabled.
  int verbose () const;

  /// Returns the port on which the server listens.
  u_short port () const;

  /// Returns the reply message size.
  int reply_message_len () const;

private:
  /// Whether verbose output is enabled.
  int verbose_;

  /// Port on which the server listens.
  u_short port_;

  /// Size of the reply message.
  size_t reply_message_len_;
};

#endif /* __CPP_INSERVER_FANCY_H */
