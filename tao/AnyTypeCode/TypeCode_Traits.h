// -*- C++ -*-

//=============================================================================
/**
 *  @file    TypeCode_Traits.h
 *
 *  $Id$
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
    typedef char const * STATIC_TYPECODE;
    typedef CORBA::String_var DYNAMIC_TYPECODE;

    template<typename S> struct Traits;
    // ----

    template<>
    struct Traits<STATIC_TYPECODE>
    {
      typedef char const * string_type;

      /**
       * A pointer to the @c CORBA::TypeCode_ptr rather than the
       * @c CORBA::TypeCode_ptr itself is stored since that address is
       * well-defined.  We may not know the value of the
       * @c CORBA::TypeCode_ptr when creating a static object that
       * refers to it, hence the indirection.
       */
      typedef CORBA::TypeCode_ptr const * typecode_type;

      typedef TAO::Null_RefCount_Policy refcount_policy_type;

      static char const * get_string (string_type const & str)
      {
        return str;
      }

      static CORBA::TypeCode_ptr get_typecode (typecode_type const & tc)
      {
        return (tc == 0 ? CORBA::TypeCode::_nil () : *tc);
      }
    };

    template<>
    struct Traits<DYNAMIC_TYPECODE>
    {
      typedef CORBA::String_var string_type;
      typedef CORBA::TypeCode_var typecode_type;
      typedef TAO::True_RefCount_Policy refcount_policy_type;

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
      typedef Traits<STATIC_TYPECODE>::typecode_type typecode_type;

      static CORBA::TypeCode_ptr get_typecode (typecode_type const & tc)
      {
        return (tc == 0 ? CORBA::TypeCode::_nil () : *tc);
      }
    };

    template<>
    struct Traits<Traits<DYNAMIC_TYPECODE>::typecode_type>
    {
      typedef Traits<DYNAMIC_TYPECODE>::typecode_type typecode_type;

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
