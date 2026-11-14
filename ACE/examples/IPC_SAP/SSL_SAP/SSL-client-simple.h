// -*- C++ -*-
// This file defines the Options class for SSL-client-simple. IBM C++
// compiler's template auto-instantiator needs this in a separate file.

#ifndef ACE_SSL_CLIENT_SIMPLE_H
#define ACE_SSL_CLIENT_SIMPLE_H

#include "ace/SSL/SSL_SOCK_Stream.h"
#include "ace/Time_Value.h"

/// Defines the options for this test.
class Options
{
public:
  Options ();

  ~Options ();

  /// Parses the command-line arguments.
  int parse_args (int argc, ACE_TCHAR *argv[]);

  /// Returns how long to sleep to implement the proper transmission rates.
  const ACE_Time_Value &sleep_time () const;

  /// Returns the server port.
  u_short port () const;

  /// Returns the server host.
  const ACE_TCHAR *host () const;

  /// Returns the string that shuts down the client or server.
  const char *quit_string () const;

  /// Reads from the appropriate location.
  ssize_t read (void *buf, size_t len, size_t &iterations);

  /// Returns the length of the message to send.
  size_t message_len () const;

  /// Returns a pointer to the message.
  const void *message_buf () const;

  /// Runs the test.
  void run ();

private:
  /// Initializes the message and sets up the barrier.
  int init ();

  /// Performs behavior shared by the oneway and twoway client tests.
  char *shared_client_test (u_short port,
                            ACE_SSL_SOCK_Stream &cli_stream);

  /// Performs the twoway test.
  void twoway_client_test ();

  /// Performs the oneway test.
  void oneway_client_test ();

  /// Host of the server.
  const ACE_TCHAR *host_;

  /// Port of the server.
  u_short port_;

  /// Sleep interval.
  ACE_Time_Value sleep_time_;

  enum {QUIT_STRING_SIZE = 128};

  /// String that shuts down the client or server.
  char quit_string_[QUIT_STRING_SIZE + 1];

  /// Size of the message sent to the server.
  size_t message_len_;

  /// Message sent to the server.
  char *message_buf_;

  /// Source of input, either ACE_STDIN or the generator.
  ACE_HANDLE io_source_;

  /// Number of iterations.
  size_t iterations_;

  /// Whether the test uses oneway or twoway communication.
  char oneway_;
};

#endif /* ACE_SSL_CLIENT_SIMPLE_H */
