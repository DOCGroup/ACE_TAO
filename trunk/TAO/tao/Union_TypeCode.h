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
     * @class Union
     *
     * @brief @c CORBA::TypeCode implementation for an OMG IDL
     *        @c union.
     *
     * This class implements a @c CORBA::TypeCode for an OMG IDL
     * @c union.
     */
    template <typename StringType, class CaseArrayType, class RefCountPolicy>
    class Union
      : public CORBA::TypeCode,
        private RefCountPolicy
    {
    public:

      /// @typedef Type of individual case array element, not the
      ///          array itself.
      typedef TAO::TypeCode::Case<StringType> const case_type;

      /// Constructor.
      Union (char const * id,
             char const * name,
             CORBA::TypeCode_ptr const * discriminant_type,
             case_type * const * cases,
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
      virtual bool tao_marshal (TAO_OutputCDR & cdr) const;
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
      virtual CORBA::Any * member_label_i (CORBA::ULong index
                                           ACE_ENV_ARG_DECL) const;
      virtual CORBA::TypeCode_ptr discriminator_type_i (
        ACE_ENV_SINGLE_ARG_DECL) const;
      virtual CORBA::Long default_index_i (ACE_ENV_SINGLE_ARG_DECL) const;
      //@}

    private:

      /// Get pointer to the underlying @c Case array.
//       case_type const * cases (void) const;

      /// Return the number of cases in the IDL @c union, including
      /// the @c default case.
      CORBA::ULong case_count (void) const;

      /// Return @c union case corresponding to given member (not
      /// @c case_type array) index.
      /**
       * @param index The zero-based index of the @c union member,
       *              including the @c default case.  For example, if
       *              the @c default case is the second @union
       *              case/member, the @a index would be @c 1.
       *
       * @return Reference to @c union case/member corresponding to
       *         the given member zero-based @a index value.
       *
       * @note This method handles the @c default case.  Do not
       *       attempt to perform special handling for the @c default
       *       case by shifting the index value by one, for example.
       */
      case_type const & the_case (CORBA::ULong index) const;

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
      Base_Attributes<StringType> const base_attributes_;

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
      CaseArrayType const cases_;

      //@}

    };

  }  // End namespace TypeCode
}  // End namespace TAO


#ifdef __ACE_INLINE__
# include "tao/Union_TypeCode.inl"
#endif  /* __ACE_INLINE__ */

#ifdef ACE_TEMPLATES_REQUIRE_SOURCE
# include "tao/Union_TypeCode.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#ifdef ACE_TEMPLATES_REQUIRE_PRAGMA
# pragma implementation ("Union_TypeCode.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */


#include /**/ "ace/post.h"

#endif /* TAO_UNION_TYPECODE_H */
