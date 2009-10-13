// -*- C++ -*-

//=============================================================================
/**
 *  @file LF_Follower_Auto_Adder.h
 *
 *  $Id$
 *
 *  @author Carlos O'Ryan <coryan@uci.edu>
 */
//=============================================================================

#ifndef TAO_LF_FOLLOWER_AUTO_ADDER_H
#define TAO_LF_FOLLOWER_AUTO_ADDER_H

#include /**/ "ace/pre.h"

#include "tao/LF_Follower.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Leader_Follower.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @brief Implement an auto_adder-like class for the TAO_LF_Followers
 * allocated via a TAO_Leader_Follower set.
 *
 * The Leader/Follower set is a factory for TAO_LF_Follower objects
 */
class TAO_Export TAO_LF_Follower_Auto_Adder
{
public:
  /// Constructor
  TAO_LF_Follower_Auto_Adder (TAO_Leader_Follower &, TAO_LF_Follower *follower);

  /// Destructor
  ~TAO_LF_Follower_Auto_Adder (void);

private:
  /// Keep a reference to the leader follower
  TAO_Leader_Follower &leader_follower_;

  /// The follower
  TAO_LF_Follower *follower_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
# include "tao/LF_Follower_Auto_Adder.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif  /* TAO_LF_FOLLOWER_AUTO_ADDER_H */
