/* -*- C++ -*- */

//=============================================================================
/**
 *  @file   Recyclable.h
 *
 *  $Id$
 *
 *  @author Doug Schmidt
 */
//=============================================================================
#ifndef ACE_RECYCLABLE_H
#define ACE_RECYCLABLE_H
#include "ace/pre.h"
#include "ace/config-all.h"


#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */



/// States of a recyclable object.
enum ACE_Recyclable_State
{
  /// Idle and can be purged.
  ACE_RECYCLABLE_IDLE_AND_PURGABLE,

  /// Idle but cannot be purged.
  ACE_RECYCLABLE_IDLE_BUT_NOT_PURGABLE,

  /// Can be purged, but is not idle (mostly for debugging).
  ACE_RECYCLABLE_PURGABLE_BUT_NOT_IDLE,

  /// Busy (i.e., cannot be recycled or purged).
  ACE_RECYCLABLE_BUSY,

  /// Closed.
  ACE_RECYCLABLE_CLOSED,

  /// Unknown state.
  ACE_RECYCLABLE_UNKNOWN
};

/**
 * @class ACE_Recyclable
 *
 * @brief
 *
 *
 */

class ACE_Export ACE_Recyclable
{
public:
  /// Destructor.
  virtual ~ACE_Recyclable (void);

  // = Set/Get the recyclable bit
  ACE_Recyclable_State recycle_state (void) const;
  void recycle_state (ACE_Recyclable_State new_state);

protected:
  /// Protected constructor.
  ACE_Recyclable (ACE_Recyclable_State initial_state);

  /// Our state.
  ACE_Recyclable_State recycle_state_;
};


#if defined (__ACE_INLINE__)
#include "ace/Utils/Recyclable.inl"
#endif /* __ACE_INLINE __ */

#include "ace/post.h"
#endif /*ACE_RECYCLABLE_STATE_H*/
