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

#ifndef TAO_RECURSIVE_TYPE_TYPECODE_H
#define TAO_RECURSIVE_TYPE_TYPECODE_H

#include /**/ "ace/pre.h"

#include "tao/AnyTypeCode/ValueModifierC.h"
#include "tao/Basic_Types.h"
#include "tao/Typecode_typesC.h"
#include "ace/Recursive_Thread_Mutex.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

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
                      TypeCodeType const & discriminant_type,
                      MemberArrayType const & cases,
                      CORBA::ULong ncases,
                      CORBA::Long default_index);

      /// Recursive @c valuetype constructor.
      Recursive_Type (CORBA::TCKind kind,
                      char const * id,
                      char const * name,
                      CORBA::ValueModifier modifier,
                      TypeCodeType const & concrete_base,
                      MemberArrayType const & fields,
                      CORBA::ULong nfields);

      /// Dynamic @c Recursive_Type TypeCode constructor.
      Recursive_Type (CORBA::TCKind kind,
                      char const * id);

      /**
       * @name TAO-specific @c CORBA::TypeCode Methods
       *
       * Methods required by TAO's implementation of the
       * @c CORBA::TypeCode class.
       *
       * @par
       *
       * These are recursive type @c TypeCode marshaling operation
       * overrides.
       *
       * @see @c CORBA::TypeCode
       */
      //@{
      virtual bool tao_marshal_kind (TAO_OutputCDR & cdr) const;
      virtual bool tao_marshal (TAO_OutputCDR & cdr,
                                CORBA::ULong offset) const;
      //@}

    protected:

      /**
       * @name TAO @c CORBA::TypeCode Template Methods
       *
       * Recursive type @c CORBA::TypeCode -specific
       * template methods.
       *
       * @see @c CORBA::TypeCode
       */
      //@{
      virtual CORBA::Boolean equal_i (CORBA::TypeCode_ptr tc) const;
      virtual CORBA::Boolean equivalent_i (CORBA::TypeCode_ptr tc) const;
      //@}

    public:

      /// Set @c struct @c TypeCode parameters.
      bool struct_parameters (char const * name,
                              MemberArrayType const & fields,
                              CORBA::ULong nfields);

      /// Set @c union @c TypeCode parameters.
      bool union_parameters (
                             char const * name,
                             TypeCodeType const & discriminant_type,
                             MemberArrayType const & cases,
                             CORBA::ULong ncases,
                             CORBA::Long default_index);

      /// Set @c valuetype or @c eventtype @c TypeCode parameters.
      bool valuetype_parameters (char const * name,
                                 CORBA::ValueModifier modifier,
                                 TypeCodeType const & concrete_base,
                                 MemberArrayType const & fields,
                                 CORBA::ULong nfields);

    private:

      /**
       * @class Reset
       *
       * @brief Reset flag to false in an exception-safe manner.
       *
       * Reset flag to false in an exception-safe manner.
       */
      class Reset
      {
      public:
        Reset (CORBA::ULong & flag) : flag_ (flag) { }
        ~Reset (void) { this->flag_ = 0; }
      private:
        CORBA::ULong & flag_;
      };

    private:

      /// Internal state thread synchronization mutex.
      mutable TAO_SYNCH_RECURSIVE_MUTEX lock_;

      /// @c true if equality or equivalence is being determined
      /// recursively.
      /**
       * This flag is used to prevent @c TypeCode equality and
       * equivalence operations from recursing indefinitely.
       */
      mutable CORBA::ULong recursion_start_offset_;

      /// Track whether data has been initialized.
      bool data_initialized_;
    };

  }  // End namespace TypeCode
}  // End namespace TAO

TAO_END_VERSIONED_NAMESPACE_DECL


#ifdef __ACE_INLINE__
# include "tao/AnyTypeCode/Recursive_Type_TypeCode.inl"
#endif  /* __ACE_INLINE__ */

#ifdef ACE_TEMPLATES_REQUIRE_SOURCE
# include "tao/AnyTypeCode/Recursive_Type_TypeCode.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#ifdef ACE_TEMPLATES_REQUIRE_PRAGMA
# pragma implementation ("Recursive_Type_TypeCode.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include /**/ "ace/post.h"

#endif  /* TAO_RECURSIVE_TYPE_TYPECODE_H */
