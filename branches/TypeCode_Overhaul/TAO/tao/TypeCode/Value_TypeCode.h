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
 *  @author Carlos O'Ryan
 */
//=============================================================================

#ifndef TAO_VALUE_TYPECODE_H
#define TAO_VALUE_TYPECODE_H

#include /**/ "ace/pre.h"

#include "tao/TypeCode.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/TypeCode_Base_Attributes.h"


namespace TAO
{
  namespace TypeCode
  {
    template <CORBA::TCKind KIND> Value_Traits;

    template <>
    struct Value_Traits<CORBA::tk_value>
    {
      enum { kind = CORBA::tk_value };
    };

    template <>
    struct Value_Traits<CORBA::tk_event>
    {
      enum { kind = CORBA::tk_event };
    };


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
              class FieldArrayType,
              CORBA::TCKind Kind,
              class RefCountPolicy>
    class Value
      : public CORBA::TypeCode,
        private RefCountPolicy
    {
    public:

      /// Constructor.
      Value (char const * id,
             char const * name,
             CORBA::ValueModifer modifier,
             CORBA::TypeCode_ptr * concrete_base,
             Field<StringType> const * fields,
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
      virtual bool tao_marshal (TAO_OutputCDR & cdr) const;
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
      virtual CORBA::Boolean equal_i (CORBA::TypeCode_ptr tc
                                      ACE_ENV_ARG_DECL) const;
      virtual CORBA::Boolean equivalent_i (CORBA::TypeCode_ptr tc
                                           ACE_ENV_ARG_DECL) const;
      virtual CORBA::TCKind kind_i (ACE_ENV_SINGLE_ARG_DECL) const;
      virtual CORBA::TypeCode_ptr get_compact_typecode_i (
        ACE_ENV_SINGLE_ARG_DECL) const;
      virtual char const * id_i (ACE_ENV_SINGLE_ARG_DECL) const;
      virtual char const * name_i (ACE_ENV_SINGLE_ARG_DECL) const;
      virtual CORBA::ULong member_count_i (ACE_ENV_SINGLE_ARG_DECL) const;
      virtual char const * member_name_i (CORBA::ULong index
                                          ACE_ENV_ARG_DECL) const;
      virtual CORBA::TypeCode_ptr member_type_i (CORBA::ULong index
                                                 ACE_ENV_ARG_DECL) const;
      virtual CORBA::Visibility member_visibility_i (ULong index
                                                     ACE_ENV_ARG_DECL) const;
      virtual CORBA::ValueModifier type_modifier (
        ACE_ENV_SINGLE_ARG_DECL) const;
      virtual CORBA::TypeCode_ptr concrete_base_type (
        ACE_ENV_SINGLE_ARG_DECL) const;
      //@}

    private:

      /// Get pointer to the underlying @c Field array.
      Field<StringType> const * fields (void) const;

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
      Base_Attributes<StringType> const base_attributes_;

      /// The @c ValueModifier of the @c valuetype of @c eventtype
      /// represented by this @c TypeCode.
      CORBA::ValueModifer const value_modifier_;

      /// The @c TypeCode corresponding to the concrete base
      /// @c valuetype or @c eventtype.
      CORBA::TypeCode_ptr * const concrete_base_;

      /// The number of fields in the OMG IDL value.
      CORBA::ULong const nfields_;

      /// Array of @c TAO::TypeCode fields representing structure of the
      /// OMG IDL defined @c value.
      FieldArrayType const fields_;

      //@}

    };

  }  // End namespace TypeCode
}  // End namespace TAO


#ifdef __ACE_INLINE__
# include "tao/Value_TypeCode.inl"
#endif  /* __ACE_INLINE__ */

#ifdef ACE_TEMPLATES_REQUIRE_SOURCE
# include "tao/Value_TypeCode.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#ifdef ACE_TEMPLATES_REQUIRE_PRAGMA
# pragma implementation ("Value_TypeCode.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */


#include /**/ "ace/post.h"

#endif /* TAO_VALUE_TYPECODE_H */
