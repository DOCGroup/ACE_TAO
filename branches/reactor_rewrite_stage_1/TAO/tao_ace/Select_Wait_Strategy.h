/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    Select_Wait_Strategy.h
 *
 *  $Id$
 *
 *  @author Balachandran Natarajan <bala@cs.wustl.edu>
 */
//=============================================================================

#ifndef TAO_ACE_SELECT_WAIT_STRATEGY_H
#define TAO_ACE_SELECT_WAIT_STRATEGY_H
#include "ace/pre.h"
#include "Event_Wait_Strategy.h"


#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "Select_IO_Handler_Repository.h"


/**
 * @class TAO_ACE_Select_Wait_Strategy
 *
 * @brief Provides an abstract interface for
 *
 * @@ todo<Bala>: Write comments
 */

class TAO_ACE_Export TAO_ACE_Select_Wait_Strategy : public TAO_ACE_Event_Wait_Strategy
{
public:

  virtual TAO_ACE_IO_Handler_Repository* handler_repository (void);

  virtual int wait_for_multiple_events (
      ACE_Time_Value *max_wait_time = 0);

  virtual ACE_HANDLE dispatchable_io_event (
      TAO_ACE_IO_Event_Masks::Event_Masks &t);

protected:

  int active_handles (void);

  int handle_error (void);

  int check_handles (void);

protected:

  TAO_ACE_Select_Strategy_Handle_Set ready_set_;

private:

  TAO_ACE_Select_IO_Handler_Repository io_handler_repo_;

  TAO_ACE_Event_Selection_Strategy *event_selection_strategy_;

};


#include "ace/post.h"
#endif /*TAO_ACE_SELECT_WAIT_STRATEGY_H*/
