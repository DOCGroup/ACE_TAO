// -*- C++ -*-

//=============================================================================
/**
 *  @file    AnyTypeCode_methods.h
 *
 *  $Id$
 *
 *  Declarations of NamedValue and NVlist methods in the CORBA namespace.
 *
 *  @author Jeff Parsons <j.parsons@vanderbilt.edu>
 */
//=============================================================================

#ifndef TAO_ANYTYPECODE_METHODS_H
#define TAO_ANYTYPECODE_METHODS_H

#include /**/ "ace/pre.h"
#include "tao/AnyTypeCode/TAO_AnyTypeCode_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined (TAO_EXPORT_MACRO)
#undef TAO_EXPORT_MACRO
#endif

#define TAO_EXPORT_MACRO TAO_AnyTypeCode_Export

namespace CORBA
{
  class NamedValue;
  typedef NamedValue *NamedValue_ptr;

  class NVList;
  typedef NVList *NVList_ptr;

  TAO_NAMESPACE_INLINE_FUNCTION void release (NamedValue_ptr);
  TAO_NAMESPACE_INLINE_FUNCTION Boolean is_nil (NamedValue_ptr);

  TAO_NAMESPACE_INLINE_FUNCTION void release (NVList_ptr);
  TAO_NAMESPACE_INLINE_FUNCTION Boolean is_nil (NVList_ptr );
}

#include /**/ "ace/post.h"

#endif  /* TAO_ANYTYPECODE_METHODS_H */
