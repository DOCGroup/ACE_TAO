// -*- C++ -*-

//=============================================================================
/**
 *  @file LF_Event_Binder.h
 *
 *  $Id$
 *
 *  @author Carlos O'Ryan <coryan@uci.edu>
 */
//=============================================================================

#ifndef TAO_LF_EVENT_BINDER_H
#define TAO_LF_EVENT_BINDER_H
#include /**/ "ace/pre.h"

#include "tao/LF_Event.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_LF_Follower;

/**
 * @brief Implement an auto_ptr-like class for the TAO_LF_Followers
 * allocated via a TAO_Leader_Follower set.
 *
 * The Leader/Follower set is a factory for TAO_LF_Follower objects
 */
class TAO_Export TAO_LF_Event_Binder
{
public:
  /// Constructor
  TAO_LF_Event_Binder (TAO_LF_Event *event,
                       TAO_LF_Follower *folloer);

  /// Destructor
  ~TAO_LF_Event_Binder (void);

private:
  /// Keep a reference to the leader follower
  TAO_LF_Event *event_;
};

#if defined (__ACE_INLINE__)
# include "tao/LF_Event_Binder.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif  /* TAO_LF_EVENT_BINDER_H */
