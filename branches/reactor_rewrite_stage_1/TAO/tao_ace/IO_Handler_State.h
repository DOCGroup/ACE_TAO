/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    IO_Handler_State.h
 *
 *  $Id$
 *
 *  @author Balachandran Natarajan <bala@cs.wustl.edu>
 */
//=============================================================================

#ifndef TAO_ACE_IO_HANDLER_STATE_H
#define TAO_ACE_IO_HANDLER_STATE_H
#include "ace/pre.h"


#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_ACE_IO_Event_Handler;


/**
 * @class TAO_ACE_IO_Handler_State
 *
 * @brief Provides an abstract interface for handling socket events.
 *
 * @@ todo<Bala>: Write comments
 */

class TAO_ACE_Export TAO_ACE_IO_Handler_State
{
public:

  /// Ctor
  TAO_ACE_IO_Handler_State (void);

  /// Number of threads actually dispatched to the socket handler
  long thread_count_;

  /// Has the handler been marked for deletion?
  int marked_for_deletion_;


  /// The IO event handler
  TAO_ACE_IO_Event_Handler *event_handler_;
};


#include "ace/post.h"
#endif /*TAO_ACE_IO_HANDLER_STATE_H*/
