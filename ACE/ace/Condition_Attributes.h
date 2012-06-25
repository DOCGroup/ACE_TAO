// -*- C++ -*-

//==========================================================================
/**
 *  @file    Condition_Attributes.h
 *
 *  $Id$
 *
 *   Moved from Synch.h.
 *
 *  @author Douglas C. Schmidt <schmidt@cs.wustl.edu>
 */
//==========================================================================

#ifndef ACE_CONDITION_ATTRIBUTES_H
#define ACE_CONDITION_ATTRIBUTES_H
#include /**/ "ace/pre.h"

#include /**/ "ace/ACE_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/OS_NS_Thread.h"

#if defined (ACE_HAS_THREADS)

// ACE platform supports some form of threading.

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

class ACE_Export ACE_Condition_Attributes
{
public:
  /// Constructor
  ACE_Condition_Attributes (int type = ACE_DEFAULT_SYNCH_TYPE);

  /// Destructor
  ~ACE_Condition_Attributes (void);

private:
  friend class ACE_Condition_Thread_Mutex;

  /// The attributes
  ACE_condattr_t attributes_;

private:
  // = Prevent assignment and initialization.
  void operator= (const ACE_Condition_Attributes &);
  ACE_Condition_Attributes (const ACE_Condition_Attributes &);
};

ACE_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "ace/Condition_Attributes.inl"
#endif /* __ACE_INLINE__ */

#endif /* !ACE_HAS_THREADS */

#include /**/ "ace/post.h"
#endif /* ACE_CONDITION_ATTRIBUTES_H */
