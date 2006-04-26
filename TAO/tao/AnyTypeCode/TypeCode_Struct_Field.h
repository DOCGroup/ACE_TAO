// -*- C++ -*-

//=============================================================================
/**
 *  @file    TypeCode_Struct_Field.h
 *
 *  $Id$
 *
 *  Header file for @c TAO::TypeCode::Struct_Field type.
 *
 *  @author Ossama Othman <ossama@dre.vanderbilt.edu>
 *  @author Carlos O'Ryan
 */
//=============================================================================

#ifndef TAO_TYPECODE_STRUCT_FIELD_H
#define TAO_TYPECODE_STRUCT_FIELD_H

#include /**/ "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

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
     * @struct Struct_Field
     *
     * @brief Name/type pair for fields of an OMG IDL defined
     *        structure or exception.
     *
     * A @c Struct_Field contains the corresponding name and pointer
     * to the @c CORBA::TypeCode for a given OMG IDL defined type.
     * For example, the fields in following OMG IDL structure:
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
     * @c TAO::TypeCode::Struct_Field array:
     *
     * \code
     *   TAO::TypeCode::Struct_Field<char const *> _tao_fields_Foo[] =
     *     {
     *       { "the_number", &CORBA::_tc_long },
     *       { "the_string", &CORBA::_tc_string },
     *     };
     * \endcode
     *
     * The template parameter @a STRING_TYPE is either @c char
     * @c const @c * or @c CORBA::String_var.  The latter is only used
     * when creating @c CORBA::tk_struct or @c CORBA::tk_except
     * @c TypeCodes dynamically, such as through the TypeCodeFactory.
     */
    template <typename StringType, typename TypeCodeType>
    struct Struct_Field
    {
      /// The name of the field.
      StringType name;

      /// The @c CORBA::TypeCode of the field.
      TypeCodeType type;

    };

  }  // End namespace TypeCode
}  // End namespace TAO

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* TAO_TYPECODE_STRUCT_FIELD_H */
