/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    Event_Wait_Strategy.h
 *
 *  $Id$
 *
 *  @author Balachandran Natarajan <bala@cs.wustl.edu>
 */
//=============================================================================

#ifndef TAO_ACE_EVENT_WAIT_STRATEGY_H
#define TAO_ACE_EVENT_WAIT_STRATEGY_H
#include "ace/pre.h"
#include "TAO_ACE_export.h"


#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */
#include "IO_Event_Masks.h"
#include "ace/Global_Macros.h"


class TAO_ACE_IO_Handler_Repository;
class ACE_Time_Value;
class TAO_ACE_Event_Selection_Strategy;

/**
 * @class TAO_ACE_Event_Wait_Strategy
 *
 * @brief Provides an abstract interface for
 *
 * @todo: If the footprint blows up it could be because of this class
 *  where we inherit from Service_Object
 * @@ todo<Bala>: Write comments.
 */

class TAO_ACE_Export TAO_ACE_Event_Wait_Strategy
{
public:

  virtual ~TAO_ACE_Event_Wait_Strategy (void);

  virtual int wait_for_multiple_events (ACE_Time_Value *time = 0);


  virtual TAO_ACE_IO_Handler_Repository *handler_repository (void) = 0;

  virtual ACE_HANDLE dispatchable_io_event (
      TAO_ACE_IO_Event_Masks::Event_Masks &t) = 0;

protected:

  TAO_ACE_Event_Wait_Strategy (void);

  TAO_ACE_Event_Selection_Strategy *event_selection_strategy_;

private:


};


#if defined (__ACE_INLINE__)
#include "Event_Wait_Strategy.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /*TAO_ACE_EVENT_WAIT_STRATEGY_H*/
