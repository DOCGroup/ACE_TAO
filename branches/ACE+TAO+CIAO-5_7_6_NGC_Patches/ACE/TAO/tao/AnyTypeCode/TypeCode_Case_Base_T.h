// -*- C++ -*-

//=============================================================================
/**
 *  @file    TypeCode_Case_Base_T.h
 *
 *  $Id$
 *
 *  Header file for @c TAO::TypeCode::Case type.
 *
 *  @author Ossama Othman
 */
//=============================================================================

#ifndef TAO_TYPECODE_CASE_H
#define TAO_TYPECODE_CASE_H

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

  class Any;
}

namespace TAO
{
  namespace TypeCode
  {

    /**
     * @class Case
     *
     * @brief Abstract base class for that represents an IDL @c union
     *        case/member.
     *
     * This class hides the actual IDL @c union member label value
     * from the @c TAO::TypeCode::Union class by relying on a
     * CORBA::Any return value that corresponds to the @c
     * CORBA::TypeCode::member_label() return type.  It also allows
     * the @c TAO::TypeCode::Union class to marshal the member label
     * values into a CDR stream without knowledge of the underlying
     * member label values.
     */
    template <typename StringType, typename TypeCodeType>
    class Case
    {
    public:

      /// Constructor.
      /**
       * Constructor used when creating static @c union @c TypeCodes.
       */
      Case (char const * name, TypeCodeType tc);

      /// Constructor.
      /**
       * Constructor used when creating dynamic @c union @c TypeCodes.
       */
      Case (void);

      /// Destructor.
      virtual ~Case (void);

      /// Cloning/copying operation.
      virtual Case * clone (void) const = 0;

      /// Return the IDL @c union case label value embedded within a
      /// @c CORBA::Any.
      virtual CORBA::Any * label (void) const = 0;

      /// Get the name of the @c union case/member.
      char const * name (void) const;

      /// Set the name of the @c union case/member.
      void name (char const * the_name);

      /// Get the @c CORBA::TypeCode of the @c union case/member.
      /**
       * @note The reference count is not manipulated by this method,
       *       i.e., ownership is retained by this class.
       */
      CORBA::TypeCode_ptr type (void) const;

      /// Set the @c CORBA::TypeCode of the @c union case/member.
      /**
       * @note @c CORBA::TypeCode::_duplicate() is called on the
       *       @c TypeCode @a tc.
       */
      void type (CORBA::TypeCode_ptr tc);

      /// Marshal this IDL @c union member into the given output CDR
      /// stream.
      bool marshal (TAO_OutputCDR & cdr, CORBA::ULong offset) const;

      /// Check for equality of the @c case attributes contained by this
      /// class and the corresponding member attributes at index "@a
      /// index" in the given @c TypeCode @a tc.
      bool equal (CORBA::ULong index, CORBA::TypeCode_ptr tc) const;

      /// Check for equivalence of the @c case attributes contained by
      /// this class and the corresponding member attributes at index
      /// "@a index" in the given @c TypeCode @a tc.
      bool equivalent (CORBA::ULong index, CORBA::TypeCode_ptr tc) const;

    protected:

      /// Marshal the IDL @c union @c case label value into the given
      /// output CDR stream.
      virtual bool marshal_label (TAO_OutputCDR & cdr) const = 0;

      /// Verify equality of member labels.
      /**
       * Performing member label equality comparisons in the @c Case
       * subclass allows us to avoid performing interpretive
       * extraction of the value from the @c Any returned from the
       * "right hand side" operand @c TypeCode since the @c Case
       * subclass already knows what type and value should be
       * extracted from the @c Any.
       *
       * @param index Member index of given @c TypeCode @a tc being
       *              tested.
       * @param tc    The @c TypeCode whose member "@a index" label is
       *              being tested.
       */
      virtual bool equal_label (CORBA::ULong index,
                                CORBA::TypeCode_ptr tc) const = 0;

    private:

      /// The name of the case.
      StringType name_;

      /// Pointer to the @c CORBA::TypeCode of the case.
      /**
       * A pointer to the @c CORBA::TypeCode_ptr rather than the
       * @c CORBA::TypeCode_ptr itself is stored since that address is
       * well-defined.  We may not know the value of the @c
       * CORBA::TypeCode_ptr when creating this @c Case statically at
       * compile-time, hence the indirection.
       *
       * @note This @c TypeCode is released upon destruction of this
       *       @c Case.
       */
      TypeCodeType type_;

    };

    typedef Case<CORBA::String_var, CORBA::TypeCode_var> Case_Dynamic;

  }  // End namespace TypeCode
}  // End namespace TAO

TAO_END_VERSIONED_NAMESPACE_DECL

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  /// @see ace/Value_Ptr.h.
  template <typename T> struct VP_traits;

  template <>
  struct TAO_AnyTypeCode_Export VP_traits<TAO::TypeCode::Case_Dynamic>
  {
    /// Copy the given object.
    static TAO::TypeCode::Case_Dynamic * clone (
      TAO::TypeCode::Case_Dynamic const * p)
    {
      return p->clone ();
    }
  };

} // End namespace ACE.

ACE_END_VERSIONED_NAMESPACE_DECL


#ifdef __ACE_INLINE__
# include "tao/AnyTypeCode/TypeCode_Case_Base_T.inl"
#endif /* __ACE_INLINE__ */

#ifdef ACE_TEMPLATES_REQUIRE_SOURCE
# include "tao/AnyTypeCode/TypeCode_Case_Base_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#ifdef ACE_TEMPLATES_REQUIRE_PRAGMA
# pragma implementation ("TypeCode_Case_Base_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include /**/ "ace/post.h"

#endif /* TAO_TYPECODE_CASE_H */
