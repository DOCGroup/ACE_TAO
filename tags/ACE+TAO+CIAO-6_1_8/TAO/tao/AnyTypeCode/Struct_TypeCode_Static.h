// -*- C++ -*-

//=============================================================================
/**
 *  @file    Struct_TypeCode_Static.h
 *
 *  $Id$
 *
 *  Header file for static @c tk_struct and @c tk_except
 *  @c CORBA::TypeCodes.
 *
 *  @author Ossama Othman <ossama@dre.vanderbilt.edu>
 *  @author Carlos O'Ryan
 */
//=============================================================================

#ifndef TAO_STRUCT_TYPECODE_STATIC_H
#define TAO_STRUCT_TYPECODE_STATIC_H

#include /**/ "ace/pre.h"

#include "tao/AnyTypeCode/TypeCode.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/AnyTypeCode/TypeCode_Base_Attributes.h"
#include "tao/AnyTypeCode/Null_RefCount_Policy.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace TypeCode
  {
    template<typename StringType, typename TypeCodeType> struct Struct_Field;
    template <typename StringType,
              typename TypeCodeType,
              class FieldArrayType,
              class RefCountPolicy> class Struct;


    /**
     * @class Struct
     *
     * @brief @c CORBA::TypeCode implementation for an OMG IDL
     *        @c struct or @c exception.
     *
     * This class implements a @c CORBA::TypeCode for an OMG IDL
     * @c struct or @c exception.
     */
    template<>
    class TAO_AnyTypeCode_Export Struct<char const *,
                            CORBA::TypeCode_ptr const *,
                            Struct_Field<char const *,
                                         CORBA::TypeCode_ptr const *> const *,
                            TAO::Null_RefCount_Policy>
      : public CORBA::TypeCode,
        private TAO::Null_RefCount_Policy
    {
    public:

      /// Constructor.
      Struct (CORBA::TCKind kind,
              char const * id,
              char const * name,
              Struct_Field<char const *,
                           CORBA::TypeCode_ptr const *> const * fields,
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
      virtual bool tao_marshal (TAO_OutputCDR & cdr,
                                CORBA::ULong offset) const;
      virtual void tao_duplicate (void);
      virtual void tao_release (void);
      //@}

    protected:

      /**
       * @name @c TAO CORBA::TypeCode Template Methods
       *
       * @c tk_struct or @c tk_except @c CORBA::TypeCode -specific
       * template methods.
       *
       * @see @c CORBA::TypeCode
       */
      //@{
      virtual CORBA::Boolean equal_i (CORBA::TypeCode_ptr tc) const;
      virtual CORBA::Boolean equivalent_i (CORBA::TypeCode_ptr tc) const;
      virtual CORBA::TypeCode_ptr get_compact_typecode_i (void) const;
      virtual char const * id_i (void) const;
      virtual char const * name_i (void) const;
      virtual CORBA::ULong member_count_i (void) const;
      virtual char const * member_name_i (CORBA::ULong index) const;
      virtual CORBA::TypeCode_ptr member_type_i (CORBA::ULong index) const;
      //@}

    private:

      /**
       * @c Struct Attributes
       *
       * Attributes representing the structure of an OMG IDL
       * @c struct or @c exception.
       *
       * @note These attributes are declared in the order in which
       *       they are marshaled into a CDR stream in order to
       *       increase cache hits by improving spatial locality.
       */
      //@{

      /// Base attributes containing repository ID and name of
      /// structure type.
      Base_Attributes<char const *> const base_attributes_;

      /// The number of fields in the OMG IDL structure.
      CORBA::ULong const nfields_;

      /// Array of @c TAO::TypeCode fields representing structure of the
      /// OMG IDL defined @c struct.
      Struct_Field<char const *,
                   CORBA::TypeCode_ptr const *> const * const fields_;

      //@}

    };

  }  // End namespace TypeCode
}  // End namespace TAO

TAO_END_VERSIONED_NAMESPACE_DECL

#ifdef __ACE_INLINE__
# include "tao/AnyTypeCode/Struct_TypeCode_Static.inl"
#endif  /* __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif /* TAO_STRUCT_TYPECODE_H */
