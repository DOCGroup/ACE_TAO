// -*- C++ -*-

//=============================================================================
/**
 *  @file    DynAnyUtils_T.h
 *
 *  $Id$
 *
 *  Declaration of templatized common code used in Dynamic Any
 *
 *  @author Jeff Parsons <j.parsons@vanderbilt.edu>
 */
//=============================================================================

#ifndef TAO_DYNANYUTILS_T_H
#define TAO_DYNANYUTILS_T_H

#include /**/ "ace/pre.h"

#include "tao/DynamicAny/DynamicAny.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/AnyTypeCode/BasicTypeTraits.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_DynCommon;

namespace TAO
{
  // Encapsulates common code for inserting and extracting basic
  // types, parameterized on the basic type.
  template<typename T>
  struct DynAnyBasicTypeUtils
  {
    static void
    insert_value (const T &val,
                  TAO_DynCommon *the_dynany);

    static typename BasicTypeTraits<T>::return_type
    get_value (TAO_DynCommon *the_dynany);
  };

  // Encapsulates code that would otherwise be repeated in
  // TAO_DynCommon::set_flag(). Parameterized on the type
  // of dynany impl class that underlies the DynAny arg.
  template<typename T>
  struct DynAnyFlagUtils
  {
    static void
    set_flag_t (DynamicAny::DynAny_ptr component,
                CORBA::Boolean destroying);
  };

  // Used by MakeDynAnyUtils below, parameterized on the type of
  // impl class and on {Any | TypeCode | inputCDR}.
  template<typename DA_IMPL, typename ANY_TC>
  struct CreateDynAnyUtils
  {
    static DynamicAny::DynAny_ptr
      create_dyn_any_t (
        ANY_TC any_tc,
        CORBA::Boolean allow_truncation= true);

    static DynamicAny::DynAny_ptr
      create_dyn_any_t (
        CORBA::TypeCode_ptr tc,
        ANY_TC any_tc,
        CORBA::Boolean allow_truncation= true);
  };

  // Code common to DynAnyFactory create_* calls, parameterized on
  // {Any | TypeCode | inputCDR}.
  namespace MakeDynAnyUtils
  {
    template<typename ANY_TC>
    DynamicAny::DynAny_ptr
      make_dyn_any_t (
        CORBA::TypeCode_ptr tc,
        ANY_TC any_tc,
        CORBA::Boolean allow_truncation= true);
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "tao/DynamicAny/DynAnyUtils_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("tao/DynamicAny/DynAnyUtils_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include /**/ "ace/post.h"

#endif /* TAO_DYNANYUTILS_T_H */
