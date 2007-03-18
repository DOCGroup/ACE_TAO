// -*- C++ -*-

//=============================================================================
/**
 *  @file    Union_TypeCode_Static.h
 *
 *  $Id$
 *
 *  Header file for a static @c tk_union CORBA::TypeCode.
 *
 *  @author Ossama Othman
 */
//=============================================================================

#ifndef TAO_UNION_TYPECODE_STATIC_H
#define TAO_UNION_TYPECODE_STATIC_H

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
    template <typename StringType,
              typename TypeCodeType,
              class CaseArrayType,
              class RefCountPolicy> class Union;
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
    template<>
    class TAO_AnyTypeCode_Export Union<char const *,
                           CORBA::TypeCode_ptr const *,
                           Case<char const *,
                                CORBA::TypeCode_ptr const *> const * const *,
                           TAO::Null_RefCount_Policy>
      : public CORBA::TypeCode,
        private TAO::Null_RefCount_Policy
    {
    public:

      /// @typedef Type of individual case array element, not the
      ///          array itself.
      typedef TAO::TypeCode::Case<char const *,
                                  CORBA::TypeCode_ptr const *> case_type;

      /// Constructor.
      Union (char const * id,
             char const * name,
             CORBA::TypeCode_ptr const * discriminant_type,
             Case<char const *,
                  CORBA::TypeCode_ptr const *> const * const * cases,
             CORBA::ULong ncases,
             CORBA::Long default_index);

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
       * @c tk_union @c CORBA::TypeCode -specific template methods.
       *
       * @see @c CORBA::TypeCode
       */
      //@{
      virtual CORBA::Boolean equal_i (CORBA::TypeCode_ptr tc
                                      ) const;
      virtual CORBA::Boolean equivalent_i (CORBA::TypeCode_ptr tc
                                           ) const;
      virtual CORBA::TypeCode_ptr get_compact_typecode_i (
        void) const;
      virtual char const * id_i (void) const;
      virtual char const * name_i (void) const;
      virtual CORBA::ULong member_count_i (void) const;
      virtual char const * member_name_i (CORBA::ULong index
                                          ) const;
      virtual CORBA::TypeCode_ptr member_type_i (CORBA::ULong index
                                                 ) const;
      virtual CORBA::Any * member_label_i (CORBA::ULong index
                                           ) const;
      virtual CORBA::TypeCode_ptr discriminator_type_i (
        void) const;
      virtual CORBA::Long default_index_i (void) const;
      //@}

    private:

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
      Base_Attributes<char const *> const base_attributes_;

      /// Type of IDL @c union discriminant.
      CORBA::TypeCode_ptr const * const discriminant_type_;

      /// Index of the default union case.
      /**
       * This value will be -1 if no default case is found in the
       * union.
       */
      CORBA::Long const default_index_;

      /// The number of cases in the OMG IDL union, excluding the
      /// @c default case.
      CORBA::ULong const ncases_;

      /// Array of @c TAO::TypeCode::Case representing structure of
      /// the OMG IDL defined @c union.
      Case<char const *,
           CORBA::TypeCode_ptr const *> const * const * const cases_;

      //@}

    };

  }  // End namespace TypeCode
}  // End namespace TAO

TAO_END_VERSIONED_NAMESPACE_DECL

#ifdef __ACE_INLINE__
# include "tao/AnyTypeCode/Union_TypeCode_Static.inl"
#endif  /* __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif /* TAO_UNION_TYPECODE_STATIC_H */
