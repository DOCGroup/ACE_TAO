// -*- C++ -*-

//=============================================================================
/**
 *  @file    Any_Special_Impl_T.h
 *
 *  @author  Carlos O'Ryan and Jeff Parsons
 */
//=============================================================================


#ifndef TAO_ANY_SPECIAL_IMPL_T_H
#define TAO_ANY_SPECIAL_IMPL_T_H

#include /**/ "ace/pre.h"

#include "tao/AnyTypeCode/Any_Impl.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  /**
   * @class Any_Special_Impl_T
   *
   * @brief Template Any class for bounded IDL (w)strings.
   */
  template<typename T, typename from_T, typename to_T>
  class Any_Special_Impl_T : public Any_Impl
  {
  public:
    Any_Special_Impl_T (_tao_destructor destructor,
                        CORBA::TypeCode_ptr,
                        T * const,
                        CORBA::ULong bound);
    ~Any_Special_Impl_T () override = default;

    static void insert (CORBA::Any &,
                        _tao_destructor destructor,
                        CORBA::TypeCode_ptr,
                        T * const,
                        CORBA::ULong bound);
    static CORBA::Boolean extract (CORBA::Any const &,
                                   _tao_destructor,
                                   CORBA::TypeCode_ptr,
                                   T const *&,
                                   CORBA::ULong bound);

    CORBA::Boolean marshal_value (TAO_OutputCDR &) override;
    CORBA::Boolean demarshal_value (TAO_InputCDR &);
    void _tao_decode (TAO_InputCDR &) override;

    void const *value () const;
    void free_value () override;

  private:
    T *value_ {};
    CORBA::ULong bound_;
  };
}

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
# include "tao/AnyTypeCode/Any_Special_Impl_T.inl"
#endif /* __ACE_INLINE__ */

#include "tao/AnyTypeCode/Any_Special_Impl_T.cpp"

#include /**/ "ace/post.h"

#endif /* TAO_ANY_SPECIAL_IMPL_T_H */

