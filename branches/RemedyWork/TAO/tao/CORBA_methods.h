// -*- C++ -*-
//=============================================================================
/**
 *  @file    CORBA_methods.h
 *
 *  $Id$
 *
 *  Declarations of common methods in the CORBA namespace.
 *
 *  @author Jeff Parsons
 */
//=============================================================================

#ifndef TAO_CORBA_METHODS_H
#define TAO_CORBA_METHODS_H

#include /**/ "ace/pre.h"

#include /**/ "tao/TAO_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Basic_Types.h"
#include "tao/orbconf.h"

#if defined (TAO_EXPORT_MACRO)
#undef TAO_EXPORT_MACRO
#endif
#define TAO_EXPORT_MACRO TAO_Export

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace CORBA
{
  class ORB;
  typedef ORB *ORB_ptr;

  /**
   * ORB Initialization
   */
  extern TAO_Export ORB_ptr ORB_init (int & argc,
                                      char * argv[],
                                      const char * orb_name = 0);
#if defined (ACE_USES_WCHAR)
  extern TAO_Export ORB_ptr ORB_init (int & argc,
                                      wchar_t * argv[],
                                      const char * orb_name = 0);

  extern TAO_Export ORB_ptr ORB_init (int & argc,
                                      wchar_t * argv[],
                                      const wchar_t * orb_name);
#endif

  template<typename T> inline Boolean is_nil (T x)
  {
    return x == 0;
  }

  template<typename T> inline void release (T x)
  {
    if (x != 0)
      {
        x->_decr_refcount ();
      }
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif  /* TAO_CORBA_METHODS_H */
