// -*- C++ -*-

//=============================================================================
/**
 *  @file    Recursive_Type_TypeCode.h
 *
 *  $Id$
 *
 *  Header file for a recursive type (@c struct, @c union or
 *  @c valuetype) CORBA::TypeCode.
 *
 *  @author Ossama Othman
 */
//=============================================================================

#ifndef TAO_RECURSIVE_TYPECODE_H
#define TAO_RECURSIVE_TYPECODE_H

#include /**/ "ace/pre.h"


namespace TAO
{
  namespace TypeCode
  {

    /**
     * @class Recursive_Type
     *
     * @brief Recursive type @c TypeCode decorator template.
     *
     * This class template decorates the underlying @c TypeCode
     * implementation @a TypeCodeBase to provide support for IDL
     * defined recursive types.  Only IDL an @c struct, @c union or
     * @c valuetype may be recursive, which is why this decorator only
     * supports constructors for the corresponding @c TypeCode
     * implementations.
     */
    template <class TypeCodeBase,
              typename TypeCodeType,
              typename MemberArrayType>
    class Recursive_Type
      : public TypeCodeBase
    {
    public:

      /// Recursive @c struct constructor.
      Recursive_Type (CORBA::TCKind kind,
                      char const * id,
                      char const * name,
                      MemberArrayType const & fields,
                      CORBA::ULong nfields);

      /// Recursive @c union constructor.
      Recursive_Type (char const * id,
                      char const * name,
#if defined (__BORLANDC__) && (__BORLANDC__ < 0x572)
                      // Borland C++ currently can't handle a
                      // reference to const pointer to const
                      // CORBA::TypeCode_ptr.
                      TypeCodeType discriminant_type,
#else
                      TypeCodeType const & discriminant_type,
#endif
                      MemberArrayType const & cases,
                      CORBA::ULong ncases,
                      CORBA::Long default_index);

      /// Recursive @c valuetype constructor.
      Recursive_Type (CORBA::TCKind kind,
                      char const * id,
                      char const * name,
                      CORBA::ValueModifier modifier,
#if defined (__BORLANDC__) && (__BORLANDC__ < 0x572)
                      // Borland C++ currently can't handle a
                      // reference to const pointer to const
                      // CORBA::TypeCode_ptr.
                      TypeCodeType concrete_base,
#else
                      TypeCodeType const & concrete_base,
#endif
                      MemberArrayType const & fields,
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
      /// Recursive type @c TypeCode marshaling operation override.
      virtual bool tao_marshal (TAO_OutputCDR & cdr) const;
      //@}

      /**
       * @name TAO @c CORBA::TypeCode Template Methods
       *
       * Recursive type @c CORBA::TypeCode -specific
       * template methods.
       *
       * @see @c CORBA::TypeCode
       */
      //@{
      virtual CORBA::Boolean equal_i (CORBA::TypeCode_ptr tc
                                      ACE_ENV_ARG_DECL) const;
      virtual CORBA::Boolean equivalent_i (CORBA::TypeCode_ptr tc
                                           ACE_ENV_ARG_DECL) const;
      //@}

    private:

      /// Internal state thread synchronization mutex.
      mutable TAO_SYNCH_RECURSIVE_MUTEX lock_;

      /// The recursive type @c TypeCode starting position/offset in
      /// the CDR stream.
      mutable CORBA::Long starting_offset_;

      /// @c true if equality or equivalence is being determined
      /// recursively.
      /**
       * This flag is used to prevent @c TypeCode equality and
       * equivalence operations from recursing indefinitely.
       */
      mutable bool in_recursion_;

    };

  }  // End namespace TypeCode
}  // End namespace TAO


#ifdef __ACE_INLINE__
# include "tao/Recursive_Type_TypeCode.inl"
#endif  /* __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif  /* TAO_RECURSIVE_TYPECODE_H */
