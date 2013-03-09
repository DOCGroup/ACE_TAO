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
 *    @c tk_home,
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

#include "tao/AnyTypeCode/TypeCode.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/AnyTypeCode/TypeCode_Base_Attributes.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace TypeCode
  {
    /**
     * @class Objref_Base
     *
     * @brief @c CORBA::TypeCode implementation for an OMG IDL
     *        @c object and object-like types.
     *
     * This class implements a @c CORBA::TypeCode for an OMG IDL
     * @c object (interface) and object-like types (abstract
     * interface, component, home, local interface and native).
     */
    template <typename StringType, class RefCountPolicy>
    class Objref
      : public CORBA::TypeCode,
        private RefCountPolicy
    {
    public:

      /// Constructor.
      Objref (CORBA::TCKind kind,
              char const * id,
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
      virtual bool tao_marshal (TAO_OutputCDR & cdr,
                                CORBA::ULong offset) const;
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
      virtual CORBA::Boolean equal_i (CORBA::TypeCode_ptr tc) const;
      virtual CORBA::Boolean equivalent_i (CORBA::TypeCode_ptr tc) const;
      virtual CORBA::TypeCode_ptr get_compact_typecode_i (void) const;
      virtual char const * id_i (void) const;
      virtual char const * name_i (void) const;

    protected:

      /// Base attributes (@c id and @c name).
      Base_Attributes<StringType> attributes_;

    };

  }  // End namespace TypeCode
}  // End namespace TAO

TAO_END_VERSIONED_NAMESPACE_DECL

#ifdef __ACE_INLINE__
# include "tao/AnyTypeCode/Objref_TypeCode.inl"
#endif  /* __ACE_INLINE__ */

#ifdef ACE_TEMPLATES_REQUIRE_SOURCE
# include "tao/AnyTypeCode/Objref_TypeCode.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#ifdef ACE_TEMPLATES_REQUIRE_PRAGMA
# pragma implementation ("Objref_TypeCode.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include /**/ "ace/post.h"

#endif /* TAO_OBJREF_TYPECODE_H */
