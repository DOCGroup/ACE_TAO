// -*- C++ -*-

//=============================================================================
/**
 *  @file    Except_TypeCode.h
 *
 *  $Id$
 *
 *  Header file for a @c tk_except CORBA::TypeCode.
 *
 *  @author Ossama Othman <ossama@dre.vanderbilt.edu>
 */
//=============================================================================

#ifndef TAO_EXCEPT_TYPECODE_H
#define TAO_EXCEPT_TYPECODE_H

#include /**/ "ace/pre.h"

#include "tao/Struct_TypeCode.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

namespace TAO
{
  namespace TypeCode
  {

    /**
     * @class Except
     *
     * @brief @c CORBA::TypeCode implementation for an OMG IDL
     *        @c except.
     *
     * This class implements a @c CORBA::TypeCode for an OMG IDL @c
     * except.
     */
    template <typename StringType, class FieldArrayType, class RefCountPolicy>
    class Except
      : public Struct<AttrType, RefCountPolicy>
    {
    public:

      /// Constructor.
      Except (char const * id,
              char const * name,
              Field<StringType> const * fields,
              CORBA::ULong nfields);

    protected:

      /**
       * @name @c TAO CORBA::TypeCode Template Methods
       *
       * @c tk_except @c CORBA::TypeCode -specific template methods.
       *
       * @c tk_except @c TypeCodes have the same internal structure as
       * @c tk_struct @c TypeCodes.  All that need be done is to
       * subclass @c TAO::TypeCode::Struct and override the
       * @c kind_i() method so that it returns @c tk_except.
       *
       * @see @c TAO::TypeCode::Struct
       */
      //@{
      virtual CORBA::TCKind kind_i (ACE_ENV_SINGLE_ARG_DECL) const;
      //@}

    };

  }  // End namespace TypeCode
}  // End namespace TAO


#ifdef __ACE_INLINE__
# include "tao/Except_TypeCode.inl"
#endif  /* __ACE_INLINE__ */

#ifdef ACE_TEMPLATES_REQUIRE_SOURCE
# include "tao/Except_TypeCode.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#ifdef ACE_TEMPLATES_REQUIRE_PRAGMA
# pragma implementation ("Except_TypeCode.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */


#include /**/ "ace/post.h"

#endif /* TAO_EXCEPT_TYPECODE_H */
