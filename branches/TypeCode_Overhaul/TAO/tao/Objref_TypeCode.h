// -*- C++ -*-

//=============================================================================
/**
 *  @file    Objref_TypeCode.h
 *
 *  $Id$
 *
 *  Header file for
 *    @c tk_abstract_interface,
 *    @c tk_component,
 *    @c tk_local_interface,
 *    @c tk_native and
 *    @c tk_objref
 *  @c CORBA::TypeCodes.
 *
 *  @author Ossama Othman <ossama@dre.vanderbilt.edu>
 */
//=============================================================================

#ifndef TAO_OBJREF_TYPECODE_H
#define TAO_OBJREF_TYPECODE_H

#include /**/ "ace/pre.h"

#include "tao/TypeCode/TypeCode.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/TypeCodeFactory_Adapter.h"

#include "tao/TypeCode/TypeCode_Base_Attributes.h"


namespace TAO
{
  namespace TypeCode
  {
    template <CORBA::TCKind KIND> struct Objref_Traits;

    template <>
    struct Objref_Traits<CORBA::tk_abstract_interface>
    {
      static
      CORBA::TypeCode_ptr
      create_compact_typecode (TAO_TypeCodeFactory_Adapter * factory,
                               char const * id
                               ACE_ENV_ARG_DECL)
      {
        return factory->create_abstract_interface_tc (id,
                                                      ""  /* empty name */
                                                      ACE_ENV_ARG_PARAMETER);
      }
    };

    template <>
    struct Objref_Traits<CORBA::tk_component>
    {
      static
      CORBA::TypeCode_ptr
      create_compact_typecode (TAO_TypeCodeFactory_Adapter * factory,
                               char const * id
                               ACE_ENV_ARG_DECL)
      {
        return factory->create_component_tc (id,
                                             ""  /* empty name */
                                             ACE_ENV_ARG_PARAMETER);
      }
    };


    template <>
    struct Objref_Traits<CORBA::tk_home>
    {
      static
      CORBA::TypeCode_ptr
      create_compact_typecode (TAO_TypeCodeFactory_Adapter * factory,
                               char const * id
                               ACE_ENV_ARG_DECL)
      {
        return factory->create_home_tc (id,
                                       ""  /* empty name */
                                       ACE_ENV_ARG_PARAMETER);
      }
    };

    template <>
    struct Objref_Traits<CORBA::tk_local_interface>
    {
      static
      CORBA::TypeCode_ptr
      create_compact_typecode (TAO_TypeCodeFactory_Adapter * factory,
                               char const * id
                               ACE_ENV_ARG_DECL)
      {
        return factory->create_local_interface_tc (id,
                                                   ""  /* empty name */
                                                   ACE_ENV_ARG_PARAMETER);
      }
    };

    template <>
    struct Objref_Traits<CORBA::tk_native>
    {
      static CORBA::TypeCode_ptr
      create_compact_typecode (TAO_TypeCodeFactory_Adapter * factory,
                               char const * id
                               ACE_ENV_ARG_DECL)
      {
        return factory->create_native_tc (id,
                                          ""  /* empty name */
                                          ACE_ENV_ARG_PARAMETER);
      }
    };

    template <>
    struct Objref_Traits<CORBA::tk_objref>
    {
      static
      CORBA::TypeCode_ptr
      create_compact_typecode (TAO_TypeCodeFactory_Adapter * factory,
                               char const * id
                               ACE_ENV_ARG_DECL)
      {
        return factory->create_interface_tc (id,
                                             ""  /* empty name */
                                             ACE_ENV_ARG_PARAMETER);
      }
    };

    /**
     * @class Objref
     *
     * @brief @c CORBA::TypeCode implementation for an OMG IDL
     *        @c object and object-like types
     *
     * This class implements a @c CORBA::TypeCode for an OMG IDL
     * @c object (interface) and object-like types (abstract
     * interface, component, local interface and native).
     */
    template <typename StringType, CORBA::TCKind Kind, class RefCountPolicy>
    class Objref
      : public CORBA::TypeCode,
        private RefCountPolicy
    {
    public:

      /// Constructor.
      Objref (char const * id,
              char const * name);

      /**
       * @name TAO-specific @c CORBA::TypeCode Methods
       *
       * Methods required by TAO's implementation of the
       * @c CORBA::TypeCode class.
       *
       * @see @c CORBA::TypeCode
       */
      //@{
      virtual bool tao_marshal (TAO_OutputCDR & cdr) const;
      virtual void tao_duplicate (void);
      virtual void tao_release (void);
      //@}

    protected:

      /**
       * @name @c TAO CORBA::TypeCode Template Methods
       *
       * @c tk_abstract_interface, @c tk_component, @c
       * tk_local_interface, @c tk_native and @c tk_objref
       * @c CORBA::TypeCode -specific template methods.
       *
       * @see @c CORBA::TypeCode
       */
      //@{
      virtual CORBA::Boolean equal_i (CORBA::TypeCode_ptr tc
                                      ACE_ENV_ARG_DECL) const;
      virtual CORBA::Boolean equivalent_i (CORBA::TypeCode_ptr tc
                                           ACE_ENV_ARG_DECL) const;
      virtual CORBA::TCKind kind_i (ACE_ENV_SINGLE_ARG_DECL) const;
      virtual CORBA::TypeCode_ptr get_compact_typecode_i (
        ACE_ENV_SINGLE_ARG_DECL) const;
      virtual char const * id_i (ACE_ENV_SINGLE_ARG_DECL) const;
      virtual char const * name_i (ACE_ENV_SINGLE_ARG_DECL) const;

    private:

      /// Base attributes (@c id and @c name).
      Base_Attributes<StringType> attributes_;

    };

  }  // End namespace TypeCode
}  // End namespace TAO


#ifdef __ACE_INLINE__
# include "tao/TypeCode/Objref_TypeCode.inl"
#endif  /* __ACE_INLINE__ */

#ifdef ACE_TEMPLATES_REQUIRE_SOURCE
# include "tao/TypeCode/Objref_TypeCode.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#ifdef ACE_TEMPLATES_REQUIRE_PRAGMA
# pragma implementation ("Objref_TypeCode.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include /**/ "ace/post.h"

#endif /* TAO_OBJREF_TYPECODE_H */
