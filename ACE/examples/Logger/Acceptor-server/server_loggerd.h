// Define classes used with templates in server_loggerd.h.

#ifndef __SERVER_LOGGERD_H
#define __SERVER_LOGGERD_H

#include "ace/Default_Constants.h"
#include "ace/SOCK_Stream.h"
#include "ace/Svc_Handler.h"
#include <string>

/// Keeps track of the options.
class Options
{
public:
  Options () = default;
  Options (Options const &) = delete;
  Options (Options &&) = delete;
  Options &operator= (Options const &) = delete;
  Options &operator= (Options &&) = delete;

  void parse_args (int argc, ACE_TCHAR *argv[]);
  u_short port () const;

private:
  u_short port_ {ACE_DEFAULT_SERVER_PORT};
};

/**
 * @brief Receives messages from remote clients.
 *
 * This class demonstrates how to receive messages from remote clients using
 * the notification mechanisms in the ACE_Reactor. It also illustrates how
 * to use the ACE_Reactor timer mechanisms.
 */
class Logging_Handler : public ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>
{
public:
  Logging_Handler () = default;
  Logging_Handler (Logging_Handler const &) = delete;
  Logging_Handler (Logging_Handler &&) = delete;
  Logging_Handler &operator= (Logging_Handler const &) = delete;
  Logging_Handler &operator= (Logging_Handler &&) = delete;

  /// @name Hooks for opening and closing handlers
  /// @{
  int open (void *) override;
  /// @}

protected:
  /// @name Demultiplexing hooks
  /// @{
  int handle_input (ACE_HANDLE) override;
  int handle_timeout (ACE_Time_Value const &tv,
                      void const *arg) override;
  /// @}

private:
  /// Host we are connected to.
  std::string peer_name_;
};

#endif /* __SERVER_LOGGERD_H */
