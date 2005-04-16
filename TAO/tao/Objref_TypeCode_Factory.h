// -*- C++ -*-

//=============================================================================
/**
 *  @file    Objref_TypeCode_Factory.h
 *
 *  $Id$
 *
 *  Header file for
 *    @c tk_abstract_interface,
 *    @c tk_component,
 *    @c tk_local_interface,
 *    @c tk_native and
 *    @c tk_objref
 *  @c CORBA::TypeCode factories.
 *
 *  @author Ossama Othman <ossama@dre.vanderbilt.edu>
 */
//=============================================================================

#ifndef TAO_OBJREF_TYPECODE_FACTORY_H
#define TAO_OBJREF_TYPECODE_FACTORY_H

#include /**/ "ace/pre.h"


#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Objref_TypeCode.h"
#include "tao/True_RefCount_Policy.h"

namespace TAO
{
  namespace TypeCodeFactory
  {

    template <CORBA::TCKind Kind> struct Objref_Traits;

    template<>
    struct Objref_Traits<CORBA::tk_abstract_interface>
    {
      static char const * tc_constant_id (void)
      {
        return "";
      }

      static CORBA::TypeCode_ptr tc_constant (void)
      {
        return CORBA::TypeCode::_nil ();
      }

      static CORBA::TypeCode_ptr create_typecode (char const * id,
                                                  char const * name)
      {
        typedef TAO::TypeCode::Objref<
          CORBA::String_var,
          CORBA::tk_abstract_interface,
          TAO::True_RefCount_Policy> typecode_type;

        CORBA::TypeCode_ptr tc = CORBA::TypeCode::_nil ();
        ACE_NEW_RETURN (tc,
                        typecode_type (id, name),
                        tc);

        return tc;
      }
    };

    template <>
    struct Objref_Traits<CORBA::tk_component>
    {
      static char const * tc_constant_id (void)
      {
        return "IDL:omg.org/CORBA/CCMObject:1.0";
      }

      static CORBA::TypeCode_ptr tc_constant (void)
      {
        return CORBA::_tc_Component;
      }

      static CORBA::TypeCode_ptr create_typecode (char const * id,
                                                  char const * name)
      {
        typedef TAO::TypeCode::Objref<
          CORBA::String_var,
          CORBA::tk_component,
          TAO::True_RefCount_Policy> typecode_type;

        CORBA::TypeCode_ptr tc = CORBA::TypeCode::_nil ();
        ACE_NEW_RETURN (tc,
                        typecode_type (id, name),
                        tc);

        return tc;
      }

    };

    template <>
    struct Objref_Traits<CORBA::tk_home>
    {
      static char const * tc_constant_id (void)
      {
        return "IDL:omg.org/CORBA/CCMHome:1.0";
      }

      static CORBA::TypeCode_ptr tc_constant (void)
      {
        return CORBA::_tc_Home;
      }

      static CORBA::TypeCode_ptr create_typecode (char const * id,
                                                  char const * name)
      {
        typedef TAO::TypeCode::Objref<
          CORBA::String_var,
          CORBA::tk_home,
          TAO::True_RefCount_Policy> typecode_type;

        CORBA::TypeCode_ptr tc = CORBA::TypeCode::_nil ();
        ACE_NEW_RETURN (tc,
                        typecode_type (id, name),
                        tc);

        return tc;
      }
    };

    template<>
    struct Objref_Traits<CORBA::tk_local_interface>
    {
      static char const * tc_constant_id (void)
      {
        return "";
      }

      static CORBA::TypeCode_ptr tc_constant (void)
      {
        return CORBA::TypeCode::_nil ();
      }

      static CORBA::TypeCode_ptr create_typecode (char const * id,
                                                  char const * name)
      {
        typedef TAO::TypeCode::Objref<
          CORBA::String_var,
          CORBA::tk_local_interface,
          TAO::True_RefCount_Policy> typecode_type;

        CORBA::TypeCode_ptr tc = CORBA::TypeCode::_nil ();
        ACE_NEW_RETURN (tc,
                        typecode_type (id, name),
                        tc);

        return tc;
      }
    };

    template<>
    struct Objref_Traits<CORBA::tk_native>
    {
      static char const * tc_constant_id (void)
      {
        return "";
      }

      static CORBA::TypeCode_ptr tc_constant (void)
      {
        return CORBA::TypeCode::_nil ();
      }

      static CORBA::TypeCode_ptr create_typecode (char const * id,
                                                  char const * name)
      {
        typedef TAO::TypeCode::Objref<
          CORBA::String_var,
          CORBA::tk_native,
          TAO::True_RefCount_Policy> typecode_type;

        CORBA::TypeCode_ptr tc = CORBA::TypeCode::_nil ();
        ACE_NEW_RETURN (tc,
                        typecode_type (id, name),
                        tc);

        return tc;
      }
    };

    template <>
    struct Objref_Traits<CORBA::tk_objref>
    {
      static char const * tc_constant_id (void)
      {
        return "IDL:omg.org/CORBA/Object:1.0";
      }

      static CORBA::TypeCode_ptr tc_constant (void)
      {
        return CORBA::_tc_Object;
      }

      static CORBA::TypeCode_ptr create_typecode (char const * id,
                                                  char const * name)
      {
        typedef TAO::TypeCode::Objref<
          CORBA::String_var,
          CORBA::tk_objref,
          TAO::True_RefCount_Policy> typecode_type;

        CORBA::TypeCode_ptr tc = CORBA::TypeCode::_nil ();
        ACE_NEW_RETURN (tc,
                        typecode_type (id, name),
                        tc);

        return tc;
      }
    };

    // --------------------------------------------------------

    /**
     * @func tc_objref_factory
     *
     * @brief TypeCode factory function template for an OMG IDL
     *        @c object TypeCode and TypeCodes for object-like types.
     *
     * This function template @c CORBA::TypeCode factory for an OMG
     * IDL @c object (interface) and object-like types (abstract
     * interface, component, local interface and native).
     */
    template<CORBA::TCKind Kind>
    bool
    tc_objref_factory (TAO_InputCDR & cdr,
                       CORBA::TypeCode_ptr & tc)
    {
      CORBA::Boolean byte_order;

      // The remainder of a tk_objref TypeCode is encoded in a CDR
      // encapsulation.
      if (!(cdr >> TAO_InputCDR::to_boolean (byte_order)))
        return false;

      cdr.reset_byte_order (byte_order);

      // Extract the repository ID and name.
      CORBA::String_var id, name;
      if (!(cdr >> TAO_InputCDR::to_string (id.out (), 0)
            && cdr >> TAO_InputCDR::to_string (name.out (), 0)))
        return false;

      if (ACE_OS::strcmp (id.in (),  // len >= 0!!!
                          Objref_Traits<Kind>::tc_constant_id ()) == 0)
        {
          // No need to create a TypeCode.  Just use the TypeCode
          // constant.
          tc =
            CORBA::TypeCode::_duplicate (Objref_Traits<Kind>::tc_constant ());

          return true;
        }

      return Objref_Traits<Kind>::create_typecode (id.in (),
                                                   name.in ());
    }

  }  // End namespace TypeCodeFactory
}  // End namespace TAO


#include /**/ "ace/post.h"

#endif /* TAO_OBJREF_TYPECODE_FACTORY_H */
