// -*- C++ -*-

//=============================================================================
/**
 *  @file    TypeCode_Non_Default_Case.h
 *
 *  $Id$
 *
 *  Header file for @c TAO::TypeCode::Non_Default_Case type.
 *
 *  @author Ossama Othman
 */
//=============================================================================

#ifndef TAO_TYPECODE_NON_DEFAULT_CASE_H
#define TAO_TYPECODE_NON_DEFAULT_CASE_H

#include /**/ "ace/pre.h"

#include "ace/config.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/TypeCode_Case.h"


namespace TAO
{
  namespace TypeCode
  {

    /**
     * @class Non_Default_Case
     *
     * @brief Representation of an OMG IDL defined @c union @c case.
     *
     * A @c Non_Default_Case contains the corresponding case label value, name and
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
     * @c TAO::TypeCode::Non_Default_Case array:
     *
     * \code
     *   typedef TAO::TypeCode::Non_Default_Case<CORBA::Short,
                                                 char const *> Foo_Case;
     *   Foo_Case _tao_cases_Foo[] =
     *     {
     *       Foo_Case (0, "a", &CORBA::_tc_short),
     *       Foo_Case (1, "b", &CORBA::_tc_short),
     *       Foo_Case (2, "c", &CORBA::_tc_long)
     *     };
     * \endcode
     *
     * The @c default case is passed directly to the
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
     * The template parameter @a STRING_TYPE is either @c char @c
     * const @c * or @c CORBA::String_var.  The latter is only used
     * when creating @c CORBA::tk_union @c TypeCode dynamically, such
     * as through the TypeCodeFactory.
     */
    template <typename DISCRIMINATOR_TYPE, typename STRING_TYPE>
    class Non_Default_Case : public Case<STRING_TYPE>
    {
    public:

      /// Constructor.
      Non_Default_Case (DISCRIMINATOR_TYPE member_label,
                        char const * member_name,
                        CORBA::TypeCode_ptr const * member_type);

      /**
       * @name @c TAO::TypeCode::Case Methods
       *
       * Methods required by the @c TAO::TypeCode::Case abstract base
       * class.
       *
       * @see @c TAO::TypeCode::Case
       */
      //@{
      virtual CORBA::Any * label (ACE_ENV_SINGLE_ARG_DECL) const;
      virtual bool marshal_label (TAO_OutputCDR & cdr) const;
      virtual bool equal_label (CORBA::ULong index,
                                CORBA::TypeCode_ptr tc
                                ACE_ENV_ARG_DECL) const;
      //@}

    private:

      /// IDL @c union case/member label value.
      DISCRIMINATOR_TYPE const label_;

    };

  }  // End namespace TypeCode
}  // End namespace TAO


#ifdef __ACE_INLINE__
# include "tao/TypeCode_Non_Default_Case.inl"
#endif /* __ACE_INLINE__ */

#ifdef ACE_TEMPLATES_REQUIRE_SOURCE
# include "tao/TypeCode_Non_Default_Case.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#ifdef ACE_TEMPLATES_REQUIRE_PRAGMA
# pragma implementation ("TypeCode_Non_Default_Case.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include /**/ "ace/post.h"

#endif /* TAO_TYPECODE_NON_DEFAULT_CASE_H */
