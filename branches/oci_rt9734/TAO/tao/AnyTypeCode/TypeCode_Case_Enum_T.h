// -*- C++ -*-

//=============================================================================
/**
 *  @file    TypeCode_Case_Enum_T.h
 *
 *  $Id$
 *
 *  Header file for @c TAO::TypeCode::Case_T template.
 *
 *  @author Ossama Othman
 */
//=============================================================================

#ifndef TAO_TYPECODE_CASE_ENUM_T_H
#define TAO_TYPECODE_CASE_ENUM_T_H

#include /**/ "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/AnyTypeCode/TypeCode_Case_Base_T.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace TypeCode
  {
    /**
     * @class Case_Enum_T
     *
     * @brief Representation of an OMG IDL defined @c union @c case for
     * unions with Enum discriminators.
     *
     * When dynamically creating a union type with a enum
     * discriminator, there is not way that the template
     * parameter DiscriminatorType can be specified at runtime.
     *
     * Since enums are encoded as ULongs, DiscriminatorType has
     * been replaced with CORBA::ULong.
     */
    template <typename StringType,
              typename TypeCodeType>
    class Case_Enum_T : public Case<StringType, TypeCodeType>
    {
    public:

      /// Constructor.
      Case_Enum_T (CORBA::TypeCode_ptr discriminator_tc,
                   CORBA::ULong member_label,
                   char const * member_name,
                   TypeCodeType member_type);

      /// Constructor.
      /**
       * Constructor only used in the dynamically constructed @c union
       * @c TypeCode case.
       */
      Case_Enum_T (CORBA::TypeCode_ptr discriminator_tc,
                   CORBA::ULong member_label);

      /**
       * @name @c TAO::TypeCode::Case Methods
       *
       * Methods required by the @c TAO::TypeCode::Case abstract base
       * class.
       *
       * @see @c TAO::TypeCode::Case
       */
      //@{
      Case<StringType, TypeCodeType> * clone (void) const;
      virtual CORBA::Any * label (void) const;
      virtual bool marshal_label (TAO_OutputCDR & cdr) const;
      virtual bool equal_label (CORBA::ULong index,
                                CORBA::TypeCode_ptr tc
                                ) const;
      //@}

    private:

      /// IDL @c union case/member label value.
      ::CORBA::ULong const label_;

      /// IDL @c union discriminator typecode.
      CORBA::TypeCode_var discriminator_tc_;
    };

  }  // End namespace TypeCode
}  // End namespace TAO

TAO_END_VERSIONED_NAMESPACE_DECL


#ifdef __ACE_INLINE__
# include "tao/AnyTypeCode/TypeCode_Case_Enum_T.inl"
#endif /* __ACE_INLINE__ */

#ifdef ACE_TEMPLATES_REQUIRE_SOURCE
# include "tao/AnyTypeCode/TypeCode_Case_Enum_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#ifdef ACE_TEMPLATES_REQUIRE_PRAGMA
# pragma implementation ("TypeCode_Case_Enum_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include /**/ "ace/post.h"

#endif /* TAO_TYPECODE_CASE_T_H */
