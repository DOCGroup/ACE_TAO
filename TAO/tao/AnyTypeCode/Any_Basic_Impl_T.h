// -*- C++ -*-

//=============================================================================
/**
 *  @file    Any_Basic_Impl_T.h
 *
 *  @authors  Carlos O'Ryan and Jeff Parsons
 */
//=============================================================================


#ifndef TAO_ANY_BASIC_IMPL_T_H
#define TAO_ANY_BASIC_IMPL_T_H

#include /**/ "ace/pre.h"

#include "tao/AnyTypeCode/Any_Impl.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  /**
   * @class Any_Basic_Impl_T
   *
   * @brief Template Any class for basic types.
   *
   * Used for all basic IDL types except bounded (w)strings.
   */
  template<typename T>
  class Any_Basic_Impl_T : public Any_Impl
  {
  public:
    Any_Basic_Impl_T (CORBA::TypeCode_ptr,
                      T const & val);
    ~Any_Basic_Impl_T () override = default;

    static void insert (CORBA::Any &,
                        CORBA::TypeCode_ptr,
                        T const &);
    static CORBA::Boolean extract (CORBA::Any const &,
                                   CORBA::TypeCode_ptr,
                                   T &);

    CORBA::Boolean marshal_value (TAO_OutputCDR &) override;
    CORBA::Boolean demarshal_value (TAO_InputCDR &);
    void _tao_decode (TAO_InputCDR &) override;

    virtual void const *value () const;
    static Any_Basic_Impl_T<T> *create_empty (CORBA::TypeCode_ptr);

  private:
    T value_;
  };
}

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
# include "tao/AnyTypeCode/Any_Basic_Impl_T.inl"
#endif /* __ACE_INLINE__ */

#include "tao/AnyTypeCode/Any_Basic_Impl_T.cpp"

#include /**/ "ace/post.h"

#endif /* TAO_ANY_BASIC_IMPL_T_H */

