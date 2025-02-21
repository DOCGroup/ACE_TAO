// -*- C++ -*-

//=============================================================================
/**
 *  @file    AnyTypeCode_methods.h
 *
 *  Declarations of NamedValue and NVlist methods in the CORBA namespace.
 *
 *  @author Jeff Parsons <j.parsons@vanderbilt.edu>
 */
//=============================================================================

#ifndef TAO_ANYTYPECODE_METHODS_H
#define TAO_ANYTYPECODE_METHODS_H

#include /**/ "ace/pre.h"
#include "tao/AnyTypeCode/TAO_AnyTypeCode_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Basic_Types.h"
#include "tao/orbconf.h"
#include "tao/CORBA_methods.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace CORBA
{
  template<>
  TAO_AnyTypeCode_Export void release (CORBA::TypeCode_ptr);
}

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif  /* TAO_ANYTYPECODE_METHODS_H */
