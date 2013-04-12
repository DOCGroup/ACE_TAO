// -*- C++ -*-

//=============================================================================
/**
 *  @file    Union_TypeCode.h
 *
 *  $Id$
 *
 *  Header file for a @c tk_union CORBA::TypeCode.
 *
 *  @author Ossama Othman
 */
//=============================================================================

#ifndef TAO_UNION_TYPECODE_H
#define TAO_UNION_TYPECODE_H

#include /**/ "ace/pre.h"

#include "tao/AnyTypeCode/TypeCode.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/AnyTypeCode/TypeCode_Base_Attributes.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace TypeCode
  {
    template <typename StringType, typename TypeCodeType> class Case;

    /**
     * @class Union
     *
     * @brief @c CORBA::TypeCode implementation for an OMG IDL
     *        @c union.
     *
     * This class implements a @c CORBA::TypeCode for an OMG IDL
     * @c union.
     */
    template <typename StringType,
              typename TypeCodeType,
              class CaseArrayType,
              class RefCountPolicy>
    class Union
      : public CORBA::TypeCode,
        private RefCountPolicy
    {
    public:

      /// @typedef Type of individual case array element, not the
      ///          array itself.
      typedef TAO::TypeCode::Case<StringType, TypeCodeType> case_type;

      /// Constructor.
      Union (char const * id,
             char const * name,
             TypeCodeType const & discriminant_type,
             CaseArrayType const & cases,
             CORBA::ULong ncases,
             CORBA::Long default_index);

      /// Constructor used for recursive TypeCodes.
      Union (CORBA::TCKind kind, char const * id);

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

      /// Accessor for cases_
      const CaseArrayType& cases(void) const;

    protected:

      /**
       * @name @c TAO CORBA::TypeCode Template Methods
       *
       * @c tk_union @c CORBA::TypeCode -specific template methods.
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
      virtual CORBA::Any * member_label_i (CORBA::ULong index) const;
      virtual CORBA::TypeCode_ptr discriminator_type_i (void) const;
      virtual CORBA::Long default_index_i (void) const;
      //@}

    protected:

      /**
       * @c Union Attributes
       *
       * Attributes representing the structure of an OMG IDL
       * @c union.
       *
       * @note These attributes are declared in the order in which
       *       they are marshaled into a CDR stream in order to
       *       increase cache hits by improving spatial locality.
       */
      //@{

      /// Base attributes containing repository ID and name of
      /// union type.
      Base_Attributes<StringType> base_attributes_;

      /// Type of IDL @c union discriminant.
      TypeCodeType discriminant_type_;

      /// Index of the default union case.
      /**
       * This value will be -1 if no default case is found in the
       * union.
       */
      CORBA::Long default_index_;

      /// The number of cases in the OMG IDL union, excluding the
      /// @c default case.
      CORBA::ULong ncases_;

      /// Array of @c TAO::TypeCode::Case representing structure of
      /// the OMG IDL defined @c union.
      CaseArrayType cases_;

      //@}

    };

  }  // End namespace TypeCode
}  // End namespace TAO

TAO_END_VERSIONED_NAMESPACE_DECL


#ifdef __ACE_INLINE__
# include "tao/AnyTypeCode/Union_TypeCode.inl"
#endif  /* __ACE_INLINE__ */

#ifdef ACE_TEMPLATES_REQUIRE_SOURCE
# include "tao/AnyTypeCode/Union_TypeCode.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#ifdef ACE_TEMPLATES_REQUIRE_PRAGMA
# pragma implementation ("Union_TypeCode.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */


#include /**/ "ace/post.h"

#endif /* TAO_UNION_TYPECODE_H */
