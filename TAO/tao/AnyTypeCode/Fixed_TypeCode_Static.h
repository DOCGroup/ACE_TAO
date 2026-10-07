// -*- C++ -*-

//=============================================================================
/**
 *  @file    Fixed_TypeCode_Static.h
 *
 *  Header file for static @c CORBA::tk_fixed @c CORBA::TypeCodes.
 *
 *  @author Ossama Othman <ossama@dre.vanderbilt.edu>
 */
//=============================================================================

#ifndef TAO_FIXED_TYPECODE_STATIC_H
#define TAO_FIXED_TYPECODE_STATIC_H

#include /**/ "ace/pre.h"

#include "tao/AnyTypeCode/TypeCode.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/AnyTypeCode/Null_RefCount_Policy.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace TypeCode
  {
    template <class RefCountPolicy> class Fixed;

    /**
     * @class Fixed
     *
     * @brief @c CORBA::TypeCode implementation for static OMG IDL
     *        @c fixed types.
     */
    template <>
    class TAO_AnyTypeCode_Export Fixed<TAO::Null_RefCount_Policy>
      : public ::CORBA::TypeCode
      , private TAO::Null_RefCount_Policy
    {
    public:
      Fixed (::CORBA::UShort digits, ::CORBA::UShort scale);

      virtual bool tao_marshal (TAO_OutputCDR & cdr, ::CORBA::ULong offset) const;
      virtual void tao_duplicate ();
      virtual void tao_release ();

    protected:
      virtual ::CORBA::Boolean equal_i (::CORBA::TypeCode_ptr tc) const;
      virtual ::CORBA::Boolean equivalent_i (::CORBA::TypeCode_ptr tc) const;
      virtual ::CORBA::TypeCode_ptr get_compact_typecode_i () const;
      virtual ::CORBA::UShort fixed_digits_i () const;
      virtual ::CORBA::UShort fixed_scale_i () const;

    private:
      ::CORBA::UShort const digits_;
      ::CORBA::UShort const scale_;
    };
  }  // End namespace TypeCode
}  // End namespace TAO

TAO_END_VERSIONED_NAMESPACE_DECL

#ifdef __ACE_INLINE__
# include "tao/AnyTypeCode/Fixed_TypeCode_Static.inl"
#endif  /* __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif /* TAO_FIXED_TYPECODE_STATIC_H */
