/* -*- C++ -*- */

//=============================================================================
/**
 *  @file   Refcountable.h
 *
 *  $Id$
 *
 *  @author Doug Schmidt
 */
//=============================================================================
#ifndef ACE_REFCOUNTABLE_H
#define ACE_REFCOUNTABLE_H
#include "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */


/**
 * @class ACE_Refcountable
 *
 * @brief
 *
 *
 */

class ACE_Export ACE_Refcountable
{
public:
  /// Destructor.
  virtual ~ACE_Refcountable (void);

  // = Increment/Decrement refcount
  int increment (void);
  int decrement (void);

  /// Returns the current refcount.
  int refcount (void) const;

protected:
  /// Protected constructor.
  ACE_Refcountable (int refcount);

  /// Current refcount.
  int refcount_;
};


#if defined (__ACE_INLINE__)
#include "ace/Refcountable.inl"
#endif /* __ACE_INLINE __ */

#include "ace/post.h"
#endif /*ACE_REFCOUNTABLE_H*/
