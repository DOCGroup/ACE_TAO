// -*- C++ -*-

//=============================================================================
/**
 *  @file    TypeCode_Case_T.h
 *
 *  $Id$
 *
 *  Header file for @c TAO::TypeCode::Case_T template.
 *
 *  @author Ossama Othman
 */
//=============================================================================

#ifndef TAO_TYPECODE_CASE_T_H
#define TAO_TYPECODE_CASE_T_H

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
     * @class Case_T
     *
     * @brief Representation of an OMG IDL defined @c union @c case.
     *
     * A @c Case_T contains the corresponding case label value, name and
     * pointer to the @c CORBA::TypeCode for a given OMG IDL @c union
     * @c case.  For
     * example, the cases in following OMG IDL @c union:
     *
     * \code
     *   union Foo switch (short)
     *   {
     *     case 0:
     *     case 1:  short a;
     *     case 2:  long  b;
     *     default: octet c;
     *   };
     * \endcode
     *
     * would be represented using the following statically instantiated
     * @c TAO::TypeCode::Case_T array:
     *
     * \code
     *   typedef TAO::TypeCode::Case_T<CORBA::Short, char const *> Foo_Case;
     *   Foo_Case _tao_cases_Foo[] =
     *     {
     *       Foo_Case (0, "a", &CORBA::_tc_short),
     *       Foo_Case (1, "a", &CORBA::_tc_short),
     *       Foo_Case (2, "b", &CORBA::_tc_long),
     *       Foo_Case (0, "c", &CORBA::_tc_octet)
     *     };
     * \endcode
     *
     * The @c default index is passed directly to the
     * @c TAO::TypeCode::Union constructor.
     *
     * The template parameter @a DISCRIMINATOR_TYPE is the
     * corresponding C++ type for the IDL defined @c union
     * discriminator type.  For example, a @c union with an IDL @c
     * short discriminator will have a corresponding @c CORBA::Short
     * C++ type.  This template parameter is necessary to allow the
     * actual @c case label value to be stored as its actual type,
     * which is particularly important when marshaling that value into
     * a CDR stream.
     *
     * The template parameter @a StringType is either @c char @c
     * const @c * or @c CORBA::String_var.  The latter is only used
     * when creating @c CORBA::tk_union @c TypeCode dynamically, such
     * as through the TypeCodeFactory.
     */
    template <typename DiscriminatorType,
              typename StringType,
              typename TypeCodeType>
    class Case_T : public Case<StringType, TypeCodeType>
    {
    public:

      /// Constructor.
      Case_T (DiscriminatorType member_label,
              char const * member_name,
              TypeCodeType member_type);

      /// Constructor.
      /**
       * Constructor only used in the dynamically constructed @c union
       * @c TypeCode case.
       */
      Case_T (DiscriminatorType member_label);

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
                                CORBA::TypeCode_ptr tc) const;
      //@}

    private:

      /// IDL @c union case/member label value.
      DiscriminatorType const label_;

    };

  }  // End namespace TypeCode
}  // End namespace TAO

TAO_END_VERSIONED_NAMESPACE_DECL

#ifdef __ACE_INLINE__
# include "tao/AnyTypeCode/TypeCode_Case_T.inl"
#endif /* __ACE_INLINE__ */

#ifdef ACE_TEMPLATES_REQUIRE_SOURCE
# include "tao/AnyTypeCode/TypeCode_Case_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#ifdef ACE_TEMPLATES_REQUIRE_PRAGMA
# pragma implementation ("TypeCode_Case_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include /**/ "ace/post.h"

#endif /* TAO_TYPECODE_CASE_T_H */
