// -*- C++ -*-
// This file defines the Options class for SSL-client. IBM C++ compiler'd
// template auto-instantiator needs this in a separate file.

#ifndef __ACE_SSL_CLIENT_H
#define __ACE_SSL_CLIENT_H

#include "ace/Barrier.h"
#include "ace/Time_Value.h"

#include "ace/SSL/SSL_SOCK_Stream.h"

/// Define the options for this test.
class Options
{
public:
  Options ();

  ~Options ();

  // Parse the command-line arguments.
  int parse_args (int argc, ACE_TCHAR *argv[]);

  // Return the amount of time to sleep in order to implement the
  // proper transmission rates.
  const ACE_Time_Value &sleep_time () const;

  // Port of the server.
  u_short port () const;

  // Host of the server.
  const ACE_TCHAR *host () const;

  // Number of threads.
  size_t threads () const;

  /// String that shuts down the client/server.
  const char *quit_string () const;

  /// Read from the appropriate location.
  ssize_t read (void *buf, size_t len, size_t &iterations);

  /// Returns the length of the message to send.
  size_t message_len () const;

  /// Returns a pointer to the message.
  const void *message_buf () const;

  /// Returns a pointer to the entry point into the thread that runs
  /// the client test function.
  ACE_THR_FUNC thr_func ();

private:
  /// Initialize the message we're sending to the user and set up the
  /// barrier.
  int init ();

  /// Performs the shared behavior of the oneway and twoway client
  /// tests.
  char *shared_client_test (u_short port,
                            ACE_SSL_SOCK_Stream &cli_stream);

  /// Performs the twoway test.
  static void *twoway_client_test (void *);

  /// Performs the oneway test.
  static void *oneway_client_test (void *);

  /// Host of the server.
  const ACE_TCHAR *host_;

  /// Port of the server.
  u_short port_;

  /// Sleep_Time value.
  ACE_Time_Value sleep_time_;

  /// Number of threads.
  size_t threads_;

  enum {QUIT_STRING_SIZE = 128};
  /// String that shuts down the client/server.
  char quit_string_[QUIT_STRING_SIZE + 1];

  /// Size of the message we send to the server.
  size_t message_len_;

  /// Pointer to the message we send to the server.
  char *message_buf_;

  /// Are we reading I/O from ACE_STDIN or from our generator?
  ACE_HANDLE io_source_;

  /// Number of iterations.
  size_t iterations_;

  /// Are we running oneway or twoway?
  char oneway_;

  // Please leave the ; inside the parenthesis to avoid Green Hills
  // (and probably other) compiler warning about extra ;.
  /// Barrier used to synchronize the start of all the threads.
  ACE_MT (ACE_Barrier *barrier_;)
};

#endif /* __ACE_SSL_CLIENT_H */
