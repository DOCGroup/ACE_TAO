/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    Fair_Perf_Selection_Strategy.h
 *
 *  $Id$
 *
 *  @author Balachandran Natarajan <bala@cs.wustl.edu>
 */
//=============================================================================

#ifndef TAO_ACE_FAIR_PERF_SELECTION_STRATEGY_H
#define TAO_ACE_FAIR_PERF_SELECTION_STRATEGY_H
#include "ace/pre.h"


#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/**
 * @class TAO_ACE_Fair_Perf_Selection_Strategy
 *
 * @brief Provides an abstract interface for
 *
 * @todo: If the footprint blows up it could be because of this class
 *  where we inherit from Service_Object
 * @@ todo<Bala>: Write comments.
 */

class TAO_ACE_Export TAO_ACE_Fair_Perf_Selection_Strategy : public TAO_ACE_Event_Selection_Strategy
{
public:

  virtual int event_reselect (void);

  virtual ACE_HANDLE dispatchable_io_event (
      TAO_ACE_Event_Masks::Event_Masks &t);


protected:



};



#if defined (__ACE_INLINE__)
#include "Fair_Perf_Selection_Strategy.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif/*TAO_ACE_FAIR_PERF_SELECTION_STRATEGY*/
