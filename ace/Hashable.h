/* -*- C++ -*- */

//=============================================================================
/**
 *  @file   Hashable.h
 *
 *  $Id$
 *
 *  @author Doug Schmidt
 */
//=============================================================================
#ifndef ACE_HASHABLE_H
#define ACE_HASHABLE_H
#include "ace/pre.h"

// @@todo: Hate to do this. Looks like we have less luck here as
// compilers complain for type u_long unknowns
#include "ace/OS.h"


#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */



/**
 * @class ACE_Recyclable
 *
 * @brief
 *
 *
 */

class ACE_Export ACE_Hashable
{
public:

  /// Destructor.
  virtual ~ACE_Hashable (void);

  /// Computes and returns hash value.  This "caches" the hash value to
  /// improve performance.
  virtual u_long hash (void) const;

protected:
  /// Protected constructor.
  ACE_Hashable (void);

  /// This is the method that actually performs the non-cached hash
  /// computation.
  virtual u_long hash_i (void) const = 0;

  /// Pre-computed hash-value.
  u_long hash_value_;
};


#if defined (__ACE_INLINE__)
#include "ace/Hashable.inl"
#endif /* __ACE_INLINE __ */

#include "ace/post.h"
#endif /*ACE_HASHABLE_H*/
