// -*- C++ -*-

//=============================================================================
/**
 *  @file    TypeCode_Value_Field.h
 *
 *  $Id$
 *
 *  Header file for @c TAO::TypeCode::Value_Field type.
 *
 *  @author Ossama Othman <ossama@dre.vanderbilt.edu>
 */
//=============================================================================

#ifndef TAO_TYPECODE_VALUE_FIELD_H
#define TAO_TYPECODE_VALUE_FIELD_H

#include /**/ "ace/pre.h"

#include "ace/config.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */


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
     * @struct Value_Field
     *
     * @brief Name/type/visibility tuple fields of an OMG IDL defined
     *        @c valuetype or @c eventtype.
     *
     * A @c Value_Field contains the corresponding name and pointer to the
     * @c CORBA::TypeCode for a given OMG IDL defined type.  For
     * example, the fields in following OMG IDL structure:
     *
     * \code
     *   struct Foo
     *   {
     *     long   the_number;
     *     string the_string;
     *   };
     * \endcode
     *
     * would be represented using the following statically instantiated
     * @c TAO::TypeCode::Value_Field array:
     *
     * \code
     *   TAO::TypeCode::Value_Field<char const *> _tao_fields_Foo[] =
     *     {
     *       { "the_number", &CORBA::_tc_long },
     *       { "the_string", &CORBA::_tc_string },
     *     };
     * \endcode
     *
     * The template parameter @a STRING_TYPE is either @c char
     * @c const @c * or @c CORBA::String_var.  The latter is only used
     * when creating @c CORBA::tk_value or @c CORBA::tk_event
     * @c TypeCodes dynamically, such as through the TypeCodeFactory.
     */
    template <typename STRING_TYPE>
    struct Value_Field
    {
      /// Destructor.
      ~Value_Field (void);

      /// Return the name of the @c Value_Field.
      /**
       * @note This method unfortunately exists so that we can
       *       retrieve the underlying string when the @a STRING_TYPE
       *       is @c CORBA::String_var rather than the
       *       @c CORBA::String_var itself.  This is necessary to
       *       silence a warning about better conversion sequences
       *       exhibited by some C++ compilers.
       */
      char const * get_name (void) const;

      /// The name of the field.
      STRING_TYPE name;

      /// Pointer to the @c CORBA::TypeCode of the field.
      /**
       * A pointer to the @c CORBA::TypeCode_ptr rather than the
       * @c CORBA::TypeCode_ptr itself is stored since that address is
       * well-defined.  We may not know the value of the
       * @c CORBA::TypeCode_ptr when creating this @c Value_Field
       * statically at compile-time, hence the indirection.
       *
       * @note This @c TypeCode is released upon destruction of this
       *       @c Value_Field.
       */
      CORBA::TypeCode_ptr * type;

      /// The visibility of the field.
      CORBA::Visibility visibility;

    };

  }  // End namespace TypeCode
}  // End namespace TAO

// If we didn't have to worry about better conversion sequence
// warnings, and drop the Value_Field<>::get_name() method, we could
// drop the below #include directives and remove the files contained
// within them altogether.

#ifdef __ACE_INLINE__
# include "tao/TypeCode_Value_Field.inl"
#endif /* __ACE_INLINE__ */

#ifdef ACE_TEMPLATES_REQUIRE_SOURCE
# include "tao/TypeCode_Value_Field.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#ifdef ACE_TEMPLATES_REQUIRE_PRAGMA
# pragma implementation ("TypeCode_Value_Field.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include /**/ "ace/post.h"

#endif /* TAO_TYPECODE_VALUE_FIELD_H */
