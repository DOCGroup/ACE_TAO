// -*- C++ -*-

//=============================================================================
/**
 *  @file    TypeCode_Enumerator.h
 *
 *  $Id$
 *
 *  Header file for @c TAO::TypeCode::Enumerator type.
 *
 *  @author Ossama Othman <ossama@dre.vanderbilt.edu>
 */
//=============================================================================

#ifndef TAO_TYPECODE_ENUMERATOR_H
#define TAO_TYPECODE_ENUMERATOR_H

#include /**/ "ace/pre.h"

#include "tao/TAO_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/orbconf.h"

namespace CORBA
{
  class TypeCode;
  typedef TypeCode* TypeCode_ptr;
}

namespace TAO
{
  namespace TypeCode
  {
    /**
     * @struct Enumerator
     *
     * @brief Enumerator of an OMG IDL defined enumeration (@c enum).
     *
     * An @c Enumerator contains the name for a given OMG IDL defined
     * enumeration.  For example, the enumerators in following OMG IDL
     * enumeration:
     *
     * \code
     *   enum Color
     *   {
     *     RED;
     *     BLUE;
     *   };
     * \endcode
     *
     * would be represented using the following @c
     * TAO::TypeCode::Enumerator array:
     *
     * \code
     *   TAO::TypeCode::Enumerator _tao_enumerators_Color[] =
     *     {
     *       "RED",
     *       "BLUE",
     *     };
     * \endcode
     */
    template <typename STRING_TYPE>
    struct Enumerator
    {

      /// Return the name of the @c Enumerator.
      /**
       * @note This method unfortunately exists so that we can
       *       retrieve the underlying string when the @a STRING_TYPE
       *       is @c CORBA::String_var rather than the
       *       @c CORBA::String_var itself.  This is necessary to
       *       silence a warning about better conversion sequences
       *       exhibited by some C++ compilers.
       */
      char const * get_name (void) const;

      /// The name of the enumerator.
      STRING_TYPE name;

    };

  }  // End namespace TypeCode
}  // End namespace TAO


#ifdef __ACE_INLINE__
# include "tao/TypeCode_Enumerator.inl"
#endif /* __ACE_INLINE__ */

// If we didn't have to worry about better conversion sequence
// warnings, and drop the Enumerator<>::get_name() method, we could
// drop the below #include directives and remove the files contained
// within them altogether.

#ifdef __ACE_INLINE__
# include "tao/TypeCode_Enumerator.inl"
#endif /* __ACE_INLINE__ */

#ifdef ACE_TEMPLATES_REQUIRE_SOURCE
# include "tao/TypeCode_Enumerator.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#ifdef ACE_TEMPLATES_REQUIRE_PRAGMA
# pragma implementation ("TypeCode_Enumerator.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include /**/ "ace/post.h"

#endif /* TAO_TYPECODE_ENUMERATOR_H */
