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

#include "tao/TAO_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Basic_Types.h"
#include "tao/orbconf.h"

#if defined (TAO_EXPORT_MACRO)
#undef TAO_EXPORT_MACRO
#endif
#define TAO_EXPORT_MACRO TAO_Export

namespace CORBA
{
  class ORB;
  typedef ORB *ORB_ptr;

  class Environment;
  typedef Environment *Environment_ptr;

  class NamedValue;
  typedef NamedValue *NamedValue_ptr;

  class NVList;
  typedef NVList *NVList_ptr;

  class Object;
  typedef Object *Object_ptr;

  class Principal;
  typedef Principal *Principal_ptr;

  class TypeCode;
  typedef TypeCode *TypeCode_ptr;

  /**
   * @name ORB Initialization
   *
   * These could be combined into a single version using an environment macro.
   *
   */
  //@{
  extern TAO_Export ORB_ptr ORB_init (int & argc,
                                      char * argv[],
                                      const char * orb_name = 0);

  extern TAO_Export ORB_ptr ORB_init (int & argc,
                                      char * argv[],
                                      const char * orb_name,
                                      Environment & ACE_TRY_ENV);
  //@}

  TAO_NAMESPACE_INLINE_FUNCTION Boolean is_nil (Environment_ptr);
  TAO_NAMESPACE_INLINE_FUNCTION void release (Environment_ptr);

  TAO_NAMESPACE_INLINE_FUNCTION Boolean is_nil (NamedValue_ptr);
  TAO_NAMESPACE_INLINE_FUNCTION void release (NamedValue_ptr);

  TAO_NAMESPACE_INLINE_FUNCTION Boolean is_nil (NVList_ptr);
  TAO_NAMESPACE_INLINE_FUNCTION void release (NVList_ptr);

  TAO_NAMESPACE_INLINE_FUNCTION Boolean is_nil (ORB_ptr);
  TAO_NAMESPACE_INLINE_FUNCTION void release (ORB_ptr);

  TAO_NAMESPACE_INLINE_FUNCTION Boolean is_nil (Object_ptr);
  TAO_NAMESPACE_INLINE_FUNCTION void release (Object_ptr);

  TAO_NAMESPACE_INLINE_FUNCTION Boolean is_nil (Principal_ptr);
  TAO_NAMESPACE_INLINE_FUNCTION void release (Principal_ptr);

  TAO_NAMESPACE_INLINE_FUNCTION Boolean is_nil (TypeCode_ptr);
  TAO_NAMESPACE_INLINE_FUNCTION void release (TypeCode_ptr);
}

#include /**/ "ace/post.h"

#endif  /* TAO_CORBA_METHODS_H */
