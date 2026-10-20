// -*- C++ -*-

//=============================================================================
/**
 *  @file    TypeCode_Traits.h
 *
 *  @author Ossama Othman <ossama@dre.vanderbilt.edu>
 */
//=============================================================================

#ifndef TAO_TYPECODE_TRAITS_H
#define TAO_TYPECODE_TRAITS_H

#include /**/ "ace/pre.h"

#include "tao/CORBA_String.h"
#include "tao/AnyTypeCode/TypeCode.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  class Null_RefCount_Policy;
  class True_RefCount_Policy;

  namespace TypeCode
  {
//     enum Storage { STATIC_TYPECODE, DYNAMIC_TYPECODE };

//     template<Storage S> struct Traits;

    // ----
    // @@ Work around limitations in MSVC++ 6.
    using STATIC_TYPECODE = char const *;
    using DYNAMIC_TYPECODE = CORBA::String_var;

    template<typename S> struct Traits;
    // ----

    template<>
    struct Traits<STATIC_TYPECODE>
    {
      using string_type = char const *;

      /**
       * A pointer to the @c CORBA::TypeCode_ptr rather than the
       * @c CORBA::TypeCode_ptr itself is stored since that address is
       * well-defined.  We may not know the value of the
       * @c CORBA::TypeCode_ptr when creating a static object that
       * refers to it, hence the indirection.
       */
      using typecode_type = CORBA::TypeCode_ptr const *;

      using refcount_policy_type = TAO::Null_RefCount_Policy;

      static char const * get_string (string_type const & str)
      {
        return str;
      }

      static CORBA::TypeCode_ptr get_typecode (typecode_type const & tc)
      {
        return (tc == nullptr ? CORBA::TypeCode::_nil () : *tc);
      }
    };

    template<>
    struct Traits<DYNAMIC_TYPECODE>
    {
      using string_type = CORBA::String_var;
      using typecode_type = CORBA::TypeCode_var;
      using refcount_policy_type = TAO::True_RefCount_Policy;

      static char const * get_string (string_type const & str)
      {
        return str.in ();
      }

      static CORBA::TypeCode_ptr get_typecode (typecode_type const & tc)
      {
        return tc.in ();
      }
    };

    // ---------------

    // Traits based on TypeCode type template parameters.

    template<>
    struct Traits<Traits<STATIC_TYPECODE>::typecode_type>
    {
      using typecode_type = Traits<STATIC_TYPECODE>::typecode_type;

      static CORBA::TypeCode_ptr get_typecode (typecode_type const & tc)
      {
        return (tc == nullptr ? CORBA::TypeCode::_nil () : *tc);
      }
    };

    template<>
    struct Traits<Traits<DYNAMIC_TYPECODE>::typecode_type>
    {
      using typecode_type = Traits<DYNAMIC_TYPECODE>::typecode_type;

      static CORBA::TypeCode_ptr get_typecode (typecode_type const & tc)
      {
        return tc.in ();
      }

    };
  }  // End namespace TypeCode
}  // End namespace TAO

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* TAO_TYPECODE_TRAITS_H */
