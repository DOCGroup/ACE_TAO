/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    Shared_Object.h
 *
 *  $Id$
 *
 *  @author Douglas C. Schmidt <schmidt@cs.wustl.edu>
 */
//=============================================================================

#ifndef ACE_SHARED_OBJECT_H
#define ACE_SHARED_OBJECT_H
#include "ace/pre.h"

#include "ace/ACE.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/**
 * @class ACE_Shared_Object
 *
 * @brief Provide the abstract base class used to access dynamic
 * linking facilities.
 */
class ACE_Export ACE_Shared_Object
{
public:
  ACE_Shared_Object (void);

  /// Initializes object when dynamic linking occurs.
  virtual int init (int argc, ACE_TCHAR *argv[]);

  /// Terminates object when dynamic unlinking occurs.
  virtual int fini (void);

  /// Returns information on a service object.
  virtual int info (ACE_TCHAR **info_string, size_t length = 0) const;

  virtual ~ACE_Shared_Object (void);
};

#if defined (__ACE_INLINE__)
#include "ace/Shared_Object.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* ACE_SHARED_OBJECT_H */
