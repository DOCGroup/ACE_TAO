/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    IO_Peer_Handler.h
 *
 *  $Id$
 *
 *  @author Balachandran Natarajan <bala@cs.wustl.edu>
 */
//=============================================================================

#ifndef TAO_ACE_IO_PEER_HANDLER_H
#define TAO_ACE_IO_PEER_HANDLER_H
#include "ace/pre.h"

#include "IO_Event_Handler.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */


/**
 * @class TAO_ACE_IO_Peer_Handler
 *
 * @brief Provides an abstract interface for handling socket events.
 *
 * @@ todo<Bala>: Write comments
 */

template <ACE_PEER_STREAM_1>
class TAO_ACE_Export TAO_ACE_IO_Peer_Handler: public TAO_ACE_IO_Event_Handler
{
public:

  /// Ctor
  TAO_ACE_IO_Peer_Handler (void);

  /// Return the underlying peer stream
  ACE_PEER_STREAM &peer (void) const;

  /// Get/Set the handle of the underlying socket
  ACE_HANDLE get_handle (void) const;
  void set_handle (ACE_HANDLE h);

protected:

  /// Called when input events occur (e.g., connection or data).
  virtual int handle_input (ACE_HANDLE fd = ACE_INVALID_HANDLE);

  /// Called when output events are possible (e.g., when flow control
  /// abates or non-blocking connection completes).
  virtual int handle_output (ACE_HANDLE fd = ACE_INVALID_HANDLE);

  /// Called when an exceptional events occur (e.g., SIGURG).
  virtual int handle_exception (ACE_HANDLE fd = ACE_INVALID_HANDLE);

private:

  /// Maintain connection with peer
  ACE_PEER_STREAM peer_;
};



#if defined (__ACE_INLINE__)
#include "IO_Peer_Handler.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /*TAO_ACE_IO_PEER_HANDLER_H*/
