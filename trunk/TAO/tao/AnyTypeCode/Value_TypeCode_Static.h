// -*- C++ -*-

//=============================================================================
/**
 *  @file    Value_TypeCode_Static.h
 *
 *  $Id$
 *
 *  Header file for static @c tk_value and @c tk_event
 *  @c CORBA::TypeCodes.
 *
 *  @author Ossama Othman <ossama@dre.vanderbilt.edu>
 */
//=============================================================================

#ifndef TAO_VALUE_TYPECODE_STATIC_H
#define TAO_VALUE_TYPECODE_STATIC_H

#include /**/ "ace/pre.h"

#include "tao/AnyTypeCode/TypeCode.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/AnyTypeCode/TypeCode_Base_Attributes.h"
#include "tao/AnyTypeCode/ValueModifierC.h"
#include "tao/AnyTypeCode/VisibilityC.h"
#include "tao/AnyTypeCode/Null_RefCount_Policy.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace TypeCode
  {
    template<typename StringType, typename TypeCodeType> struct Value_Field;
    template <typename StringType,
              typename TypeCodeType,
              class FieldArrayType,
              class RefCountPolicy> class Value;

    /**
     * @class Value
     *
     * @brief @c CORBA::TypeCode implementation for an OMG IDL
     *        @c valuetype or @c event.
     *
     * This class implements a @c CORBA::TypeCode for an OMG IDL
     * @c valuetype or @c event.
     */
    template<>
    class TAO_AnyTypeCode_Export Value<char const *,
                           CORBA::TypeCode_ptr const *,
                           Value_Field<char const *,
                                       CORBA::TypeCode_ptr const *> const *,
                           TAO::Null_RefCount_Policy>
      : public CORBA::TypeCode,
        private TAO::Null_RefCount_Policy
    {
    public:

      /// Constructor.
      Value (CORBA::TCKind kind,
             char const * id,
             char const * name,
             CORBA::ValueModifier modifier,
             CORBA::TypeCode_ptr const * concrete_base,
             Value_Field<char const *,
                         CORBA::TypeCode_ptr const *> const * fields,
             CORBA::ULong nfields);

      /**
       * @name TAO-specific @c CORBA::TypeCode Methods
       *
       * Methods required by TAO's implementation of the
       * @c CORBA::TypeCode class.
       *
       * @see @c CORBA::TypeCode
       */
      //@{
      virtual bool tao_marshal (TAO_OutputCDR & cdr, CORBA::ULong offset) const;
      virtual void tao_duplicate (void);
      virtual void tao_release (void);
      //@}

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

    private:

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
      Base_Attributes<char const *> const base_attributes_;

      /// The @c ValueModifier of the @c valuetype of @c eventtype
      /// represented by this @c TypeCode.
      CORBA::ValueModifier const type_modifier_;

      /// The @c TypeCode corresponding to the concrete base
      /// @c valuetype or @c eventtype.
      CORBA::TypeCode_ptr const * const concrete_base_;

      /// The number of fields in the OMG IDL value.
      CORBA::ULong const nfields_;

      /// Array of @c TAO::TypeCode fields representing structure of the
      /// OMG IDL defined @c value.
      Value_Field<char const *,
                  CORBA::TypeCode_ptr const *> const * const fields_;

      //@}

    };

  }  // End namespace TypeCode
}  // End namespace TAO

TAO_END_VERSIONED_NAMESPACE_DECL

#ifdef __ACE_INLINE__
# include "tao/AnyTypeCode/Value_TypeCode_Static.inl"
#endif  /* __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif /* TAO_VALUE_TYPECODE_STATIC_H */
