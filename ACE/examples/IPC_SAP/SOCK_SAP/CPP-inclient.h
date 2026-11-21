/**
 * @file CPP-inclient.h
 *
 * Defines the CPP-inclient Options class. IBM's C++ template auto-instantiator
 * requires this class to be in a separate file.
 */

#ifndef __CPP_INCLIENT_H
#define __CPP_INCLIENT_H

#include "ace/SOCK_Stream.h"
#include "ace/Barrier.h"
#include "ace/Time_Value.h"

/// Defines the options for the client test.
class Options
{
public:
  Options ();

  ~Options ();

  /// Parses the command-line arguments.
  int parse_args (int argc, ACE_TCHAR *argv[]);

  /// Returns the delay used to control the message transmission rate.
  const ACE_Time_Value &sleep_time () const;

  /// Returns the server port.
  u_short port () const;

  /// Returns the server host.
  const ACE_TCHAR *host () const;

  /// Returns the number of client threads.
  size_t threads () const;

  /// Returns the string that shuts down the client and server.
  const ACE_TCHAR *quit_string () const;

  /// Reads data from the selected source.
  ssize_t read (void *buf, size_t len, size_t &iterations);

  /// Returns the length of the message to send.
  size_t message_len () const;

  /// Returns the message buffer.
  const void *message_buf () const;

  /// Returns the client thread entry point.
  ACE_THR_FUNC thr_func ();

private:
  /// Initializes the message and synchronization barrier.
  int init ();

  /// Performs behavior shared by the one-way and two-way client tests.
  char *shared_client_test (u_short port,
                            ACE_SOCK_Stream &cli_stream);

  /// Runs the two-way client test.
  static void *twoway_client_test (void *);

  /// Runs the one-way client test.
  static void *oneway_client_test (void *);

  /// Server host.
  const ACE_TCHAR *host_;

  /// Server port.
  u_short port_;

  /// Delay between messages.
  ACE_Time_Value sleep_time_;

  /// Number of client threads.
  size_t threads_;

  /// String that shuts down the client and server.
  const ACE_TCHAR *quit_string_;

  /// Size of the message sent to the server.
  size_t message_len_;

  /// Message buffer sent to the server.
  char *message_buf_;

  /// Input source, either standard input or the message generator.
  ACE_HANDLE io_source_;

  /// Number of test iterations.
  size_t iterations_;

  /// Indicates whether the client uses one-way or two-way calls.
  char oneway_;

  // Please leave the ; inside the parenthesis to avoid Green Hills
  // (and probably other) compiler warning about extra ;.
  /// Synchronizes the start of all client threads.
  ACE_MT (ACE_Barrier *barrier_;)
};

#endif /* __CPP_INCLIENT_H */
