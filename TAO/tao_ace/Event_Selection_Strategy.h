/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    Event_Selection_Strategy.h
 *
 *  $Id$
 *
 *  @author Balachandran Natarajan <bala@cs.wustl.edu>
 */
//=============================================================================

#ifndef TAO_ACE_EVENT_SELECTION_STRATEGY_H
#define TAO_ACE_EVENT_SELECTION_STRATEGY_H
#include "ace/pre.h"

#include "IO_Event_Masks.h"


#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/**
 * @class TAO_ACE_Event_Selection_Strategy
 *
 * @brief Provides an abstract interface for
 *
 * @todo: If the footprint blows up it could be because of this class
 *  where we inherit from Service_Object
 * @@ todo<Bala>: Write comments.
 */

class TAO_ACE_Export TAO_ACE_Event_Selection_Strategy
{
public:

  virtual int event_reselect (void) = 0;

  virtual ACE_HANDLE dispatchable_io_event (
      TAO_ACE_IO_Event_Masks::Event_Masks &t) = 0;

protected:

  TAO_ACE_Event_Selection_Strategy (void);

private:

};

#include "ace/post.h"
#endif /*TAO_ACE_EVENT_SLECETION_STRATEGY */
