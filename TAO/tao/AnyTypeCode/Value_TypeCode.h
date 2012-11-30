// -*- C++ -*-

//=============================================================================
/**
 *  @file    Value_TypeCode.h
 *
 *  $Id$
 *
 *  Header file for a @c tk_value and @c tk_event
 *  @c CORBA::TypeCodes.
 *
 *  @author Ossama Othman <ossama@dre.vanderbilt.edu>
 */
//=============================================================================

#ifndef TAO_VALUE_TYPECODE_H
#define TAO_VALUE_TYPECODE_H

#include /**/ "ace/pre.h"

#include "tao/AnyTypeCode/TypeCode.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/AnyTypeCode/TypeCode_Base_Attributes.h"
#include "tao/AnyTypeCode/ValueModifierC.h"
#include "tao/AnyTypeCode/VisibilityC.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace TypeCode
  {
    template<typename StringType, typename TypeCodeType> struct Value_Field;

    /**
     * @class Value
     *
     * @brief @c CORBA::TypeCode implementation for an OMG IDL
     *        @c valuetype or @c event.
     *
     * This class implements a @c CORBA::TypeCode for an OMG IDL
     * @c valuetype or @c event.
     */
    template <typename StringType,
              typename TypeCodeType,
              class FieldArrayType,
              class RefCountPolicy>
    class Value
      : public CORBA::TypeCode,
        private RefCountPolicy
    {
    public:

      /// Constructor.
      Value (CORBA::TCKind kind,
             char const * id,
             char const * name,
             CORBA::ValueModifier modifier,
             TypeCodeType const & concrete_base,
             FieldArrayType const & fields,
             CORBA::ULong nfields);

      /// Constructor used for recursive TypeCodes.
      Value (CORBA::TCKind kind,
              char const * id);

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

      /// Accessor for concrete_base_
      const TypeCodeType& concrete_base(void) const;

      /// Accessor for fields_
      const FieldArrayType& fields() const;

    protected:

      /**
       * @name @c TAO CORBA::TypeCode Template Methods
       *
       * @c tk_value or @c tk_event @c CORBA::TypeCode -specific
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
      virtual CORBA::ULong member_count_i (void) const;
      virtual char const * member_name_i (CORBA::ULong index) const;
      virtual CORBA::TypeCode_ptr member_type_i (CORBA::ULong index) const;
      virtual CORBA::Visibility member_visibility_i (CORBA::ULong index) const;
      virtual CORBA::ValueModifier type_modifier_i (void) const;
      virtual CORBA::TypeCode_ptr concrete_base_type_i (void) const;
      //@}

    protected:

      /**
       * @c Valuetype Attributes
       *
       * Attributes representing the structure of an OMG IDL
       * @c valuetype or @c event.
       *
       * @note These attributes are declared in the order in which
       *       they are marshaled into a CDR stream in order to
       *       increase cache hits by improving spatial locality.
       */
      //@{

      /// Base attributes containing repository ID and name of
      /// @c valuetype.
      Base_Attributes<StringType> base_attributes_;

      /// The @c ValueModifier of the @c valuetype of @c eventtype
      /// represented by this @c TypeCode.
      CORBA::ValueModifier type_modifier_;

      /// The @c TypeCode corresponding to the concrete base
      /// @c valuetype or @c eventtype.
      TypeCodeType concrete_base_;

      /// The number of fields in the OMG IDL value.
      CORBA::ULong nfields_;

      /// Array of @c TAO::TypeCode fields representing structure of the
      /// OMG IDL defined @c value.
      FieldArrayType fields_;

      //@}

    };

  }  // End namespace TypeCode
}  // End namespace TAO

TAO_END_VERSIONED_NAMESPACE_DECL

#ifdef __ACE_INLINE__
# include "tao/AnyTypeCode/Value_TypeCode.inl"
#endif  /* __ACE_INLINE__ */

#ifdef ACE_TEMPLATES_REQUIRE_SOURCE
# include "tao/AnyTypeCode/Value_TypeCode.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#ifdef ACE_TEMPLATES_REQUIRE_PRAGMA
# pragma implementation ("Value_TypeCode.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */


#include /**/ "ace/post.h"

#endif /* TAO_VALUE_TYPECODE_H */
