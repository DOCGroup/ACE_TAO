// This file defines the class needed for iostream_server.cpp - it's a separate
// file to satisfy IBM's xlC template instantiation.

#ifndef __IOSTREAM_SERVER_H
#define __IOSTREAM_SERVER_H

#include "ace/INET_Addr.h"
#include "ace/IOStream.h"
#include "ace/SOCK_Stream.h"
#include "ace/Svc_Handler.h"

#if !defined (ACE_LACKS_ACE_IOSTREAM)

// Declare a new type which will case an ACE_SOCK_Stream to behave
// like an iostream.  The new ACE_SOCK_IOStream type can be used
// anywhere an ACE_SOCK_Stream is used.

using ACE_SOCK_IOStream = ACE_IOStream<ACE_SOCK_Stream>;

/**
 * @brief Extends ACE_Svc_Handler for the iostream/SOCK_Stream hybrid.
 *
 * The handler follows the usual pattern until @c handle_input, where it
 * uses the iostream characteristics of its peer.
 */
class Handler : public ACE_Svc_Handler<ACE_SOCK_IOStream, ACE_NULL_SYNCH>
{
public:
  Handler ();
  Handler (Handler const &) = delete;
  Handler (Handler &&) = delete;
  Handler &operator= (Handler const &) = delete;
  Handler &operator= (Handler &&) = delete;
  ~Handler () override;

  /// @name Svc_Handler hooks
  /// @{
  int open (void *) override;
  /// @}

  /// @name Event_Handler hooks
  /// @{
  int handle_input (ACE_HANDLE) override;
  /// @}
};

#endif /* ACE_LACKS_ACE_IOSTREAM */

#endif /* __IOSTREAM_SERVER_H */
