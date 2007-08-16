// -*- C++ -*-

//=============================================================================
/**
 *  @file    Alias_TypeCode.h
 *
 *  $Id$
 *
 *  Header file for a @c tk_alias and @c tk_value_box
 *  @c CORBA::TypeCode.
 *
 *  @author Ossama Othman <ossama@dre.vanderbilt.edu>
 */
//=============================================================================

#ifndef TAO_ALIAS_TYPECODE_H
#define TAO_ALIAS_TYPECODE_H

#include /**/ "ace/pre.h"

#include "tao/AnyTypeCode/TypeCode.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/TypeCodeFactory_Adapter.h"
#include "tao/AnyTypeCode/TypeCode_Base_Attributes.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace TypeCode
  {
    /**
     * @class Alias
     *
     * @brief @c CORBA::TypeCode implementation for an OMG IDL
     *        @c typedef.
     *
     * This class implements a @c CORBA::TypeCode for an OMG IDL
     * @c typedef.
     */
    template <typename StringType,
              typename TypeCodeType,
              class RefCountPolicy>
    class Alias
      : public CORBA::TypeCode,
        private RefCountPolicy
    {
    public:

      /// Constructor.
      Alias (CORBA::TCKind kind,
             char const * id,
             char const * name,
#if defined (__BORLANDC__) && (__BORLANDC__ < 0x572)
             // Borland C++ currently can't handle a reference to
             // const pointer to const CORBA::TypeCode_ptr
             TypeCodeType tc);
#else
             TypeCodeType const & tc);
#endif

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
       * @c tk_alias and @c tk_value_box @c CORBA::TypeCode -specific
       * template methods.
       *
       * @see @c CORBA::TypeCode
       */
      //@{
      virtual CORBA::Boolean equal_i (CORBA::TypeCode_ptr tc) const;
      virtual CORBA::Boolean equivalent_i (CORBA::TypeCode_ptr tc) const;
      virtual CORBA::TypeCode_ptr get_compact_typecode_i (void) const;
      virtual char const * id_i (void) const;
      virtual char const * name_i (void) const;
      virtual CORBA::TypeCode_ptr content_type_i (void) const;

    private:

      /// Base attributes for this @c TypeCode containing the
      /// repository ID and name of the @c typedef.
      Base_Attributes<StringType> attributes_;

      /// The @c TypeCode corresponding to the original type upon
      /// which the IDL @c typedef was made.
      /**
       * A pointer to the @c CORBA::TypeCode_ptr rather than the
       * @c CORBA::TypeCode_ptr itself is stored since that address is
       * well-defined.  We may not know the value of the @c
       * CORBA::TypeCode_ptr when creating this @c Field statically at
       * compile-time, hence the indirection.
       *
       * @note This @c TypeCode is released upon destruction of this
       *       @c TypeCode::Alias.
       */
      TypeCodeType const content_type_;

    };

  }  // End namespace TypeCode
}  // End namespace TAO

TAO_END_VERSIONED_NAMESPACE_DECL


#ifdef __ACE_INLINE__
# include "tao/AnyTypeCode/Alias_TypeCode.inl"
#endif  /* __ACE_INLINE__ */

#ifdef ACE_TEMPLATES_REQUIRE_SOURCE
# include "tao/AnyTypeCode/Alias_TypeCode.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#ifdef ACE_TEMPLATES_REQUIRE_PRAGMA
# pragma implementation ("Alias_TypeCode.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include /**/ "ace/post.h"

#endif /* TAO_ALIAS_TYPECODE_H */
