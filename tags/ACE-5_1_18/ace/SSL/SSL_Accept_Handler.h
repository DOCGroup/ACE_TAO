// -*- C++ -*-

//=============================================================================
/**
 *  @file SSL_Accept_Handler.h
 *
 *  $Id$
 *
 *  @author Ossama Othman <ossama@uci.edu>
 */
//=============================================================================


#ifndef ACE_SSL_ACCEPT_HANDLER_H
#define ACE_SSL_ACCEPT_HANDLER_H

#include "ace/pre.h"

#include "SSL_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Event_Handler.h"

class ACE_SSL_SOCK_Stream;

/**
 * @class ACE_SSL_Accept_Handler
 *
 * @brief Event handler designed to complete non-blocking passive
 *        connections.
 *
 *
 */
class ACE_SSL_Accept_Handler : public ACE_Event_Handler
{
public:

  /// Constructor.
  ACE_SSL_Accept_Handler (ACE_SSL_SOCK_Stream &ssl_stream);

  /// Destructor.
  virtual ~ACE_SSL_Accept_Handler (void);

  /// Get the handle associated with this event handler.
  virtual ACE_HANDLE get_handle (void) const;

  /// Called when input events occur (e.g., connection or data).
  virtual int handle_input (ACE_HANDLE fd = ACE_INVALID_HANDLE);

  /// Called when output events are possible (e.g., flow control
  /// abates).
  virtual int handle_output (ACE_HANDLE fd = ACE_INVALID_HANDLE);

  /// Called when a handle_*() method returns -1 or when the
  /// remove_handler() method is called on an ACE_Reactor.  The
  /// close_mask indicates which event has triggered the
  /// handle_close() method callback on a particular handle.
  virtual int handle_close (ACE_HANDLE handle,
                            ACE_Reactor_Mask close_mask);

private:

  /// Perform the SSL accept.
  int ssl_accept (void);

private:

  /// Reference to the SSL_SOCK_Stream for which the passive
  /// connection is being established.
  ACE_SSL_SOCK_Stream &ssl_stream_;
};


#include "ace/post.h"

#endif	/* ACE_SSL_ACCEPT_HANDLER_H */
