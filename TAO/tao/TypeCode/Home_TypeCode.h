// -*- C++ -*-

//=============================================================================
/**
 *  @file    Home_TypeCode.h
 *
 *  $Id$
 *
 *  Header file for a @c tk_home CORBA::TypeCode.
 *
 *  @author Ossama Othman <ossama@dre.vanderbilt.edu>
 */
//=============================================================================

#ifndef TAO_HOME_TYPECODE_H
#define TAO_HOME_TYPECODE_H

#include /**/ "ace/pre.h"

#include "tao/Objref_TypeCode.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */


namespace TAO
{
  namespace TypeCode
  {

    /**
     * @class Home
     *
     * @brief @c CORBA::TypeCode implementation for an OMG IDL
     *        @c home type.
     *
     * This class implements a @c CORBA::TypeCode for an OMG IDL
     * @c home type.
     */
    template <typename StringType, class RefCountPolicy>
    class Home
      : public Objref<StringType, RefCountPolicy>
    {
    public:

      /// Constructor.
      Home (char const * id,
            char const * name);

    protected:

      /**
       * @name @c TAO CORBA::TypeCode Template Methods
       *
       * @c tk_home @c CORBA::TypeCode -specific template methods.
       *
       * @see @c CORBA::TypeCode
       */
      //@{
      virtual CORBA::TCKind kind_i (ACE_ENV_SINGLE_ARG_DECL) const;
      virtual CORBA::TypeCode_ptr get_compact_typecode_i (
        ACE_ENV_SINGLE_ARG_DECL) const;

    };

  }  // End namespace TypeCode
}  // End namespace TAO


#ifdef __ACE_INLINE__
# include "tao/Home_TypeCode.inl"
#endif  /* __ACE_INLINE__ */

#ifdef ACE_TEMPLATES_REQUIRE_SOURCE
# include "tao/Home_TypeCode.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#ifdef ACE_TEMPLATES_REQUIRE_PRAGMA
# pragma implementation ("Home_TypeCode.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include /**/ "ace/post.h"

#endif /* TAO_HOME_TYPECODE_H */
