/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    Event_Dispatcher.h
 *
 *  $Id$
 *
 *  @author Balachandran Natarajan <bala@cs.wustl.edu>
 */
//=============================================================================

#ifndef TAO_ACE_EVENT_DISPATCHER_H
#define TAO_ACE_EVENT_DISPATCHER_H
#include "ace/pre.h"

#include "IO_Handler_Repository.h"


#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_ACE_Event_Reactor_Impl;

/**
 * @class TAO_ACE_Event_Dispatcher
 *
 * @brief Provides an interface for dispatching various types of event.
 *
 *  This also hold the various strategies that are used for dispatching.
 *
 * @@ todo<Bala>: Write comments
 */

class TAO_ACE_Export TAO_ACE_Event_Dispatcher
{
public:

  TAO_ACE_Event_Dispatcher (TAO_ACE_Event_Reactor_Impl *reactor);


  int dispatch_io_events (TAO_ACE_IO_Handler_Repository::IO_Handler_State &state,
                          TAO_ACE_IO_Event_Masks::Event_Masks &t);


private:

  TAO_ACE_Event_Reactor_Impl *reactor_impl_;

  /// Reference to the lock that the Reactor has

};

#include "ace/post.h"
#endif /*TAO_ACE_EVENT_DISPATCHER_H*/
