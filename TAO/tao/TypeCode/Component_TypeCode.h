// -*- C++ -*-

//=============================================================================
/**
 *  @file    Component_TypeCode.h
 *
 *  $Id$
 *
 *  Header file for a @c tk_component CORBA::TypeCode.
 *
 *  @author Ossama Othman <ossama@dre.vanderbilt.edu>
 */
//=============================================================================

#ifndef TAO_COMPONENT_TYPECODE_H
#define TAO_COMPONENT_TYPECODE_H

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
     * @class Component
     *
     * @brief @c CORBA::TypeCode implementation for an OMG IDL
     *        @c component type.
     *
     * This class implements a @c CORBA::TypeCode for an OMG IDL
     * @c component type.
     */
    template <typename StringType, class RefCountPolicy>
    class Component
      : public Objref<StringType, RefCountPolicy>
    {
    public:

      /// Constructor.
      Component (char const * id,
                 char const * name);

    protected:

      /**
       * @name @c TAO CORBA::TypeCode Template Methods
       *
       * @c tk_component @c CORBA::TypeCode -specific template
       * methods.
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
# include "tao/Component_TypeCode.inl"
#endif  /* __ACE_INLINE__ */

#ifdef ACE_TEMPLATES_REQUIRE_SOURCE
# include "tao/Component_TypeCode.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#ifdef ACE_TEMPLATES_REQUIRE_PRAGMA
# pragma implementation ("Component_TypeCode.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include /**/ "ace/post.h"

#endif /* TAO_COMPONENT_TYPECODE_H */
