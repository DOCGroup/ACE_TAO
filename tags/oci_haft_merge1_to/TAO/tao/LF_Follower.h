// -*- C++ -*-

//=============================================================================
/**
 *  @file LF_Follower.h
 *
 *  $Id$
 *
 *  @author Carlos O'Ryan <coryan@uci.edu>
 */
//=============================================================================

#ifndef TAO_LF_FOLLOWER_H
#define TAO_LF_FOLLOWER_H
#include "ace/pre.h"

#include "tao/orbconf.h"
#include "tao/TAO_Export.h"
#include "ace/Synch.h"
#include "ace/Intrusive_List_Node.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_Leader_Follower;

/**
 * @class TAO_LF_Follower
 *
 * @brief Represent a thread blocked, as a follower, in the
 *        Leader/Followers set.
 *
 * @todo Currently this class offers little abstraction, the follower
 * loop should be implemented by this class.
 *
 */
class TAO_Export TAO_LF_Follower : public ACE_Intrusive_List_Node<TAO_LF_Follower>
{
public:
  /// Constructor
  TAO_LF_Follower (TAO_Leader_Follower &leader_follower);

  /// Destructor
  ~TAO_LF_Follower (void);

  /// Access the leader follower that owns this follower
  TAO_Leader_Follower &leader_follower (void);

  /// Wait until on the underlying condition variable
  int wait (ACE_Time_Value *tv);

  /// Signal the underlying condition variable
  int signal (void);

private:
  /// The Leader/Follower set this Follower belongs to
  TAO_Leader_Follower &leader_follower_;

  /// Condition variable used to
  ACE_SYNCH_CONDITION condition_;
};

#if defined (__ACE_INLINE__)
# include "tao/LF_Follower.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif  /* TAO_LF_FOLLOWER_H */
