// -*- C++ -*-

//==========================================================================
/**
 *  @file    TSS_Adapter.h
 *
 *  $Id$
 *
 *   Originally in Synch.h
 *
 *  @author Douglas C. Schmidt <schmidt@cs.wustl.edu>
 */
//==========================================================================

#ifndef ACE_TSS_ADAPTER_H
#define ACE_TSS_ADAPTER_H
#include /**/ "ace/pre.h"

#include "ace/ACE_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/**
 * @class ACE_TSS_Adapter
 *
 * @brief This class encapsulates a TSS object and its associated
 * C++ destructor function.  It is used by the ACE_TSS...
 * methods (in Synch_T.cpp) in order to allow an extern
 * "C" cleanup routine to be used.  Needed by the "frigging"
 * MVS C++ compiler.
 *
 * Objects of this class are stored in thread specific
 * storage. ts_obj_ points to the "real" object and
 * func_ is a pointer to the C++ cleanup function for ts_obj_.
 */
class ACE_Export ACE_TSS_Adapter
{
public:
  /// Initialize the adapter.
  ACE_TSS_Adapter (void *object, ACE_THR_DEST f);

  /// Default dtor.
  ~ACE_TSS_Adapter (void);

  /// Perform the cleanup operation.
  void cleanup (void);

//private:

  /// The real TS object.
  void *ts_obj_;

  /// The real cleanup routine for ts_obj;
  ACE_THR_DEST func_;
};

#if defined (__ACE_INLINE__)
#include "ace/TSS_Adapter.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /* ACE_TSS_ADAPTER_H */
