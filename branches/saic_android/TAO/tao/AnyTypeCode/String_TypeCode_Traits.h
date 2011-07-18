// -*- C++ -*-

//=============================================================================
/**
 *  @file    String_TypeCode_Traits.h
 *
 *  $Id$
 *
 *  Header file for bound
 *    @c tk_string,
 *    @c tk_wstring,
 *  @c CORBA::TypeCode factories.
 *
 *  @author Ossama Othman <ossama@dre.vanderbilt.edu>
 */
//=============================================================================

#ifndef TAO_STRING_TYPECODE_TRAITS_H
#define TAO_STRING_TYPECODE_TRAITS_H

#include /**/ "ace/pre.h"


#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/AnyTypeCode/String_TypeCode.h"
#include "tao/AnyTypeCode/True_RefCount_Policy.h"


TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace TypeCodeFactory
  {
    template <typename T> struct String_Traits;

    template<>
    struct String_Traits<CORBA::Any::from_string>
    {
      static CORBA::TypeCode_ptr create_typecode (CORBA::TCKind kind,
                                                  CORBA::ULong bound)
      {
        typedef TAO::TypeCode::String<TAO::True_RefCount_Policy> typecode_type;

        CORBA::TypeCode_ptr tc = CORBA::TypeCode_ptr ();
        ACE_NEW_RETURN (tc,
                        typecode_type (kind, bound),
                        tc);

        return tc;
      }
    };

    template <>
    struct String_Traits<CORBA::Any::from_wstring>
    {
      static CORBA::TypeCode_ptr create_typecode (CORBA::TCKind kind,
                                                  CORBA::ULong bound)
      {
        typedef TAO::TypeCode::String<TAO::True_RefCount_Policy> typecode_type;

        CORBA::TypeCode_ptr tc = CORBA::TypeCode_ptr ();
        ACE_NEW_RETURN (tc,
                        typecode_type (kind, bound),
                        tc);

        return tc;
      }
    };

  }  // End namespace TypeCodeFactory
}  // End namespace TAO

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* TAO_STRING_TYPECODE_TRAITS_H */
