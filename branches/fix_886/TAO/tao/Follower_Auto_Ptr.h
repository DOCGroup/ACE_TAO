// -*- C++ -*-

//=============================================================================
/**
 *  @file Follower_Auto_Ptr.h
 *
 *  $Id$
 *
 *  @author Carlos O'Ryan <coryan@uci.edu>
 */
//=============================================================================

#ifndef TAO_FOLLOWER_AUTO_PTR_H
#define TAO_FOLLOWER_AUTO_PTR_H
#include "ace/pre.h"

#include "tao/orbconf.h"
#include "tao/Leader_Follower.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_Follower;

/**
 * @brief Implement an auto_ptr-like class for the TAO_Followers
 * allocated via a TAO_Leader_Follower set.
 *
 * The Leader/Follower set is a factory for TAO_Follower objects
 */
class TAO_Export TAO_LF_Follower_Auto_Ptr
{
public:
  /// Constructor
  TAO_LF_Follower_Auto_Ptr (TAO_Leader_Follower &);

  /// Destructor
  ~TAO_LF_Follower_Auto_Ptr (void);

  /// Implement the smart pointer methods
  TAO_Follower *get (void);
  TAO_Follower *operator->(void);
  operator TAO_Follower *(void);

private:
  /// Keep a reference to the leader follower
  TAO_Leader_Follower &leader_follower_;

  /// The follower
  TAO_Follower *follower_;
};

#if defined (__ACE_INLINE__)
# include "tao/Follower_Auto_Ptr.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif  /* TAO_FOLLOWER_AUTO_PTR_H */
