// -*- C++ -*-

//=============================================================================
/**
 *  @file    TypeCode_Default_Case.h
 *
 *  $Id$
 *
 *  Header file for @c TAO::TypeCode::Default_Case type.
 *
 *  @author Ossama Othman
 */
//=============================================================================

#ifndef TAO_TYPECODE_DEFAULT_CASE_H
#define TAO_TYPECODE_DEFAULT_CASE_H

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
     * @class Default_Case
     *
     * @brief Representation of an OMG IDL defined @c union @c default
     * @c case.
     *
     */
    template <typename StringType, typename TypeCodeType>
    class Default_Case : public Case<StringType, TypeCodeType>
    {
    public:

      /// Constructor.
      Default_Case (char const * member_name,
                    TypeCodeType member_type);

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

    };

  }  // End namespace TypeCode
}  // End namespace TAO


#ifdef __ACE_INLINE__
# include "tao/TypeCode_Default_Case.inl"
#endif /* __ACE_INLINE__ */

#ifdef ACE_TEMPLATES_REQUIRE_SOURCE
# include "tao/TypeCode_Default_Case.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#ifdef ACE_TEMPLATES_REQUIRE_PRAGMA
# pragma implementation ("TypeCode_Default_Case.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include /**/ "ace/post.h"

#endif /* TAO_TYPECODE_DEFAULT_CASE_H */
