// -*- C++ -*-

//=============================================================================
/**
 *  @file    Struct_TypeCode.h
 *
 *  $Id$
 *
 *  Header file for a @c tk_struct CORBA::TypeCode.
 *
 *  @author Ossama Othman <ossama@dre.vanderbilt.edu>
 *  @author Carlos O'Ryan
 */
//=============================================================================

#ifndef TAO_STRUCT_TYPECODE_H
#define TAO_STRUCT_TYPECODE_H

#include /**/ "ace/pre.h"

#include "tao/TypeCode.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/TypeCode_Base_Attributes.h"


namespace TAO
{
  namespace TypeCode
  {

    /**
     * @class Struct
     *
     * @brief @c CORBA::TypeCode implementation for an OMG IDL
     *        @c struct.
     *
     * This class implements a @c CORBA::TypeCode for an OMG IDL
     * @c struct.
     */
    template <typename StringType, class FieldArrayType, class RefCountPolicy>
    class Struct
      : public CORBA::TypeCode,
        private RefCountPolicy
    {
    public:

      /// Constructor.
      Struct (char const * id,
              char const * name,
              Field<StringType> const * fields,
              CORBA::ULong nfields);

      /**
       * @name TAO-specific @c CORBA::TypeCode Methods
       *
       * Methods required by TAO's implementation of the
       * @c CORBA::TypeCode class.
       *
       * @see @c CORBA::TypeCode
       */
      //@{
      virtual bool tao_marshal (TAO_OutputCDR & cdr) const;
      virtual void tao_duplicate (void);
      virtual void tao_release (void);
      //@}

    protected:

      /**
       * @name @c TAO CORBA::TypeCode Template Methods
       *
       * @c tk_struct @c CORBA::TypeCode -specific template methods.
       *
       * @see @c CORBA::TypeCode
       */
      //@{
      virtual CORBA::Boolean equal_i (CORBA::TypeCode_ptr tc
                                      ACE_ENV_ARG_DECL) const;
      virtual CORBA::Boolean equivalent_i (CORBA::TypeCode_ptr tc
                                           ACE_ENV_ARG_DECL) const;
      virtual CORBA::TCKind kind_i (ACE_ENV_SINGLE_ARG_DECL) const;
      virtual CORBA::TypeCode_ptr get_compact_typecode_i (
        ACE_ENV_SINGLE_ARG_DECL) const;
      virtual char const * id_i (ACE_ENV_SINGLE_ARG_DECL) const;
      virtual char const * name_i (ACE_ENV_SINGLE_ARG_DECL) const;
      virtual CORBA::ULong member_count_i (ACE_ENV_SINGLE_ARG_DECL) const;
      virtual char const * member_name_i (CORBA::ULong index
                                          ACE_ENV_ARG_DECL) const;
      virtual CORBA::TypeCode_ptr member_type_i (CORBA::ULong index
                                                 ACE_ENV_ARG_DECL) const;
      //@}

    private:

      /// Get pointer to the underlying @c Field array.
      Field<STRING_TYPE> const * fields (void) const;

    private:

      /// Base attributes containing repository ID and name of
      /// structure type.
      Base_Attributes<StringType> const base_attributes_;

      /// Array of @c TAO::TypeCode fields representing structure of the
      /// OMG IDL defined @c struct.
      FieldArrayType const fields_;

      /// The number of fields in the OMG IDL structure.
      CORBA::ULong const nfields_;

    };

  }  // End namespace TypeCode
}  // End namespace TAO


#ifdef __ACE_INLINE__
# include "tao/Struct_TypeCode.inl"
#endif  /* __ACE_INLINE__ */

#ifdef ACE_TEMPLATES_REQUIRE_SOURCE
# include "tao/Struct_TypeCode.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#ifdef ACE_TEMPLATES_REQUIRE_PRAGMA
# pragma implementation ("Struct_TypeCode.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */


#include /**/ "ace/post.h"

#endif /* TAO_STRUCT_TYPECODE_H */
