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

#include "tao/TypeCode_Case_Base.h"


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
    template <typename STRING_TYPE>
    class Default_Case : public Case_Base<STRING_TYPE>
    {
    public:

      /// Constructor.
      Default_Case (char const * member_name,
                    CORBA::TypeCode_ptr * member_type);

      /// Return the IDL @c union default_case label value embedded within a
      /// @c CORBA::Any.
      virtual CORBA::Any * label (ACE_ENV_SINGLE_ARG_DECL) const;

      /// Marshal this IDL @c union member into the given output CDR
      /// stream.
      virtual bool marshal_label (TAO_OutputCDR & cdr) const;

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
