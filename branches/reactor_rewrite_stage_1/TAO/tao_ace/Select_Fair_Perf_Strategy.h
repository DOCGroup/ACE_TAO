/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    Select_Fair_Perf_Strategy.h
 *
 *  $Id$
 *
 *  @author Balachandran Natarajan <bala@cs.wustl.edu>
 */
//=============================================================================

#ifndef TAO_ACE_SELECT_FAIR_PERF_STRATEGY_H
#define TAO_ACE_SELECT_FAIR_PERF_STRATEGY_H
#include "ace/pre.h"


#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/**
 * @class TAO_ACE_Select_Fair_Perf_Strategy
 *
 * @brief Provides an abstract interface for
 *
 * @todo: If the footprint blows up it could be because of this class
 *  where we inherit from Service_Object
 * @@ todo<Bala>: Write comments.
 */

class TAO_ACE_Export TAO_ACE_Select_Fair_Perf_Strategy : public TAO_ACE_Event_Selection_Strategy
{
public:

  virtual ACE_HANDLE dispatchable_io_event (
      TAO_ACE_Event_Masks::Event_Masks &t);

private:

  void clear_handle (ACE_HANDLE handle);

  ACE_HANDLE iterate (ACE_Handle_Set_Iterator &handle_iter);

private:

  ACE_Handle_Set_Iterator rd_mask_iter_;
  ACE_Handle_Set_Iterator wr_mask_iter_;
  ACE_Handle_Set_Iterator ex_mask_iter_;

  TAO_ACE_Select_Strategy_Handle_Set &read_handle_set_;
};


#if defined (__ACE_INLINE__)
#include "Select_Fair_Perf_Strategy.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /*TAO_ACE_SELECT_FAIR_PERF_STRATEGY_H*/
