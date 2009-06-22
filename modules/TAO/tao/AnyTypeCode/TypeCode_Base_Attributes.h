// -*- C++ -*-

//=============================================================================
/**
 *  @file    TypeCode_Base_Attributes.h
 *
 *  $Id$
 *
 *  @author Ossama Othman <ossama@dre.vanderbilt.edu>
 */
//=============================================================================

#ifndef TAO_TYPECODE_BASE_ATTRIBUTES_H
#define TAO_TYPECODE_BASE_ATTRIBUTES_H

#include /**/ "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Versioned_Namespace.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace TypeCode
  {

    /**
     * @class Base_Attributes
     *
     * @brief Attributes contained by most @c TypeCodes with complex
     *        parameter lists.
     *
     * Most @c TypeCodes with complex parameter lists (see Section
     * 15.3.5.1 "TypeCode" in the CORBA specification) contain these
     * attributes, namely a repository ID and a name.
     */
    template <typename STRING_TYPE>
    class Base_Attributes
    {
    public:

      /// Constructor.
      Base_Attributes (char const * id,
                       char const * name);

      /// Constructor used by recursive @c TypeCodes.
      Base_Attributes (char const * id);

      /// Get the @c RepositoryId globally identifying the type.
      char const * id (void) const;

      /// Get the simple name identifying the type within its
      /// enclosing scope.
      char const * name (void) const;

      /// Set the simple name identifying the type within its
      /// enclosing scope.
      /**
       * @note This method only used when creating a recursive
       *       TypeCode.
       */
      void name (char const * the_name);

    private:

      /// The @c RepositoryId globally identifying the type.
      STRING_TYPE const id_;

      /// The simple name identifying the type within its enclosing
      /// scope.
      STRING_TYPE name_;

    };

  }  // End namespace TypeCode
}  // End namespace TAO

TAO_END_VERSIONED_NAMESPACE_DECL

#ifdef __ACE_INLINE__
# include "tao/AnyTypeCode/TypeCode_Base_Attributes.inl"
#endif /* __ACE_INLINE__ */

#ifdef ACE_TEMPLATES_REQUIRE_SOURCE
# include "tao/AnyTypeCode/TypeCode_Base_Attributes.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#ifdef ACE_TEMPLATES_REQUIRE_PRAGMA
# pragma implementation ("TypeCode_Base_Attributes.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */


#include /**/ "ace/post.h"

#endif /* TAO_TYPECODE_BASE_ATTRIBUTES_H */
