// -*- C++ -*-

//=============================================================================
/**
 *  @file    TypeCode_Case_Base.h
 *
 *  $Id$
 *
 *  Header file for @c TAO::TypeCode::Case_Base type.
 *
 *  @author Ossama Othman
 */
//=============================================================================

#ifndef TAO_TYPECODE_CASE_BASE_H
#define TAO_TYPECODE_CASE_BASE_H

#include /**/ "ace/pre.h"

#include "ace/config.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */


namespace CORBA
{
  class TypeCode;
  typedef TypeCode* TypeCode_ptr;

  class Any;
}

namespace TAO
{
  namespace TypeCode
  {

    /**
     * @class Case_Base
     *
     * @brief Abstract base class for that represents an IDL @c union
     *        case/member.
     *
     * This class hides the actual IDL @c union member label value
     * from the @c TAO::TypeCode::Union class by relying on a
     * CORBA::Any return value that corresponds to the @c
     * CORBA::TypeCode::member_label() return type.  It also allows
     * the @c TAO::TypeCode::Union class to marshal the member label
     * values into a CDR stream without knowledge of the underlying
     * member label values.
     */
    template <typename STRING_TYPE>
    class Case_Base
    {
    public:

      /// Constructor.
      Case_Base (char const * name,
                 CORBA::TypeCode_ptr * tc);

      /// Destructor.
      virtual ~Case_Base (void);

      /// Return the IDL @c union case label value embedded within a
      /// @c CORBA::Any.
      virtual CORBA::Any * label (ACE_ENV_SINGLE_ARG_DECL) const = 0;

      /// Get the name of the @c union case/member.
      char const * name (void) const;

      /// Get the @c CORBA::TypeCode of the @c union case/member.
      /**
       * @note The reference count is not manipulated by this method,
       *       i.e., ownership is retained by this class.
       */
      CORBA::TypeCode_ptr type (void) const;

      /// Marshal this IDL @c union member into the given output CDR
      /// stream.
      bool marshal (TAO_OutputCDR & cdr) const;

    protected:

      /// Marshal the IDL @c union @c case label value into the given
      /// output CDR stream.
      virtual bool marshal_label (TAO_OutputCDR & cdr) const = 0;

    private:

      /// The name of the case.
      STRING_TYPE const name_;

      /// Pointer to the @c CORBA::TypeCode of the case.
      /**
       * A pointer to the @c CORBA::TypeCode_ptr rather than the
       * @c CORBA::TypeCode_ptr itself is stored since that address is
       * well-defined.  We may not know the value of the @c
       * CORBA::TypeCode_ptr when creating this @c Case statically at
       * compile-time, hence the indirection.
       *
       * @note This @c TypeCode is released upon destruction of this
       *       @c Case.
       */
      CORBA::TypeCode_ptr * const type_;

    };

  }  // End namespace TypeCode
}  // End namespace TAO


#ifdef __ACE_INLINE__
# include "tao/TypeCode_Case_Base.inl"
#endif /* __ACE_INLINE__ */

#ifdef ACE_TEMPLATES_REQUIRE_SOURCE
# include "tao/TypeCode_Case_Base.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#ifdef ACE_TEMPLATES_REQUIRE_PRAGMA
# pragma implementation ("TypeCode_Case_Base.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include /**/ "ace/post.h"

#endif /* TAO_TYPECODE_CASE_BASE_H */
