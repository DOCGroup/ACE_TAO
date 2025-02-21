/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    Logging_Handler.h
 *
 *  @author Doug Schmidt
 */
//=============================================================================

#ifndef _CLIENT_HANDLER_H
#define _CLIENT_HANDLER_H

#include "ace/Event_Handler.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/INET_Addr.h"
#include "ace/SOCK_Stream.h"
#include "ace/os_include/os_netdb.h"

/**
 * @class Logging_Handler
 *
 * @brief Receive client message from the remote clients.
 *
 * This class demonstrates how to receive messages from remote
 * clients using the notification mechanisms in the
 * <ACE_Reactor>.  In addition, it also illustrates how to
 * utilize the <ACE_Reactor> timer mechanisms, as well.
 */
class Logging_Handler : public ACE_Event_Handler
{
public:
  Logging_Handler () = default;

  // = Hooks for opening and closing handlers.
  virtual int open ();
  virtual int close ();

  /// Conversion operators.
  ACE_SOCK_Stream &peer ();

protected:
  // = Demultiplexing hooks.
  ACE_HANDLE get_handle () const override;
  int handle_input (ACE_HANDLE) override;
  int handle_close (ACE_HANDLE = ACE_INVALID_HANDLE,
                    ACE_Reactor_Mask = ACE_Event_Handler::ALL_EVENTS_MASK) override;
  int handle_timeout (const ACE_Time_Value &tv, const void *arg) override;

  // = Really should be private...
  /// Ensure dynamic allocation.
  virtual ~Logging_Handler ();

private:
  /// Host we are connected to.
  char host_name_[MAXHOSTNAMELEN + 1];

  /// Connection with client
  ACE_SOCK_Stream cli_stream_;
};

#endif /* _CLIENT_HANDLER_H */
