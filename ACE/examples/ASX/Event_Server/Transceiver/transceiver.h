/* -*- C++ -*- */
#ifndef ACE_TRANSCEIVER_H
#define ACE_TRANSCEIVER_H

#include "ace/SOCK_Stream.h"
#include "ace/Svc_Handler.h"
#include <string>

/**
 * @class Event_Transceiver
 * @brief Sends and receives messages through the event server.
 *
 * This class acts as both a consumer and supplier of events.
 */
class Event_Transceiver : public ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>
{
public:
  /// Perform the actual initialization.
  Event_Transceiver (int argc, ACE_TCHAR *argv[]);

  /// No-op constructor required by ACE_Connector.
  Event_Transceiver () = default;

  Event_Transceiver (Event_Transceiver const &) = delete;
  Event_Transceiver (Event_Transceiver &&) = delete;
  Event_Transceiver &operator= (Event_Transceiver const &) = delete;
  Event_Transceiver &operator= (Event_Transceiver &&) = delete;

  /// @name Service handler hooks
  /// @{

  /// Initialize the transceiver when it is connected.
  int open (void *) override;

  /// @}

  /// @name Reactor hooks
  /// @{

  /// Receive data from standard input or the socket.
  int handle_input (ACE_HANDLE) override;

  /// Close down via SIGINT.
  int handle_signal (int signum, siginfo_t *, ucontext_t *) override;

  /// Close down the event loop.
  int handle_close (ACE_HANDLE, ACE_Reactor_Mask) override;

  /// @}

private:
  enum class Role
  {
    Consumer,
    Supplier
  };

  struct Options
  {
    u_short port_number {ACE_DEFAULT_SERVER_PORT};
    std::basic_string<ACE_TCHAR> host_name {ACE_DEFAULT_SERVER_HOST};
    Role role {Role::Supplier};
  };

  /// Read data from the socket and write it to ACE_STDOUT.
  int receiver ();

  /// Write data from ACE_STDIN to the socket.
  int transmitter ();

  /// Parse the command-line arguments.
  int parse_args (int argc, ACE_TCHAR *argv[]);

  ACE_TCHAR const *role_name () const;

  Options options_;
};

#endif /* ACE_TRANSCEIVER_H */
