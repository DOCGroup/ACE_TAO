// -*- C++ -*-
// This file defines the Options class for SSL-client-simple. IBM C++
// compiler's template auto-instantiator needs this in a separate file.

#ifndef ACE_SSL_CLIENT_SIMPLE_H
#define ACE_SSL_CLIENT_SIMPLE_H

#include "ace/SSL/SSL_SOCK_Stream.h"
#include "ace/Time_Value.h"

class Options
  // = TITLE
  //   Define the options for this test.
{
public:
  Options ();
  // Constructor.

  ~Options ();
  // Destructor.

  int parse_args (int argc, ACE_TCHAR *argv[]);
  // Parse the command-line arguments.

  const ACE_Time_Value &sleep_time () const;
  // Return the amount of time to sleep in order to implement the
  // proper transmission rates.

  u_short port () const;
  // Port of the server.

  const ACE_TCHAR *host () const;
  // Host of the server.

  const char *quit_string () const;
  // String that shuts down the client/server.

  ssize_t read (void *buf, size_t len, size_t &iterations);
  // Read from the appropriate location.

  size_t message_len () const;
  // Returns the length of the message to send.

  const void *message_buf () const;
  // Returns a pointer to the message.

  void run ();
  // Run the test

private:
  int init ();
  // Initialize the message we're sending to the user and set up the
  // barrier.

  char *shared_client_test (u_short port,
                            ACE_SSL_SOCK_Stream &cli_stream);
  // Performs the shared behavior of the oneway and twoway client
  // tests.

  void twoway_client_test ();
  // Performs the twoway test.

  void oneway_client_test ();
  // Performs the oneway test.

  const ACE_TCHAR *host_;
  // Host of the server.

  u_short port_;
  // Port of the server.

  ACE_Time_Value sleep_time_;
  // Sleep_Time value.

  enum {QUIT_STRING_SIZE = 128};
  char quit_string_[QUIT_STRING_SIZE + 1];
  // String that shuts down the client/server.

  size_t message_len_;
  // Size of the message we send to the server.

  char *message_buf_;
  // Pointer to the message we send to the server.

  ACE_HANDLE io_source_;
  // Are we reading I/O from ACE_STDIN or from our generator?

  size_t iterations_;
  // Number of iterations.

  char oneway_;
  // Are we running oneway or twoway?
};

#endif /* ACE_SSL_CLIENT_SIMPLE_H */
