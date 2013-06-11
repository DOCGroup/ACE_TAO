// -*- C++ -*-

//=============================================================================
/**
 *  @file    Any_Dual_Impl_T.h
 *
 *  $Id$
 *
 *  @authors  Carlos O'Ryan and Jeff Parsons
 */
//=============================================================================


#ifndef TAO_ANY_DUAL_IMPL_T_H
#define TAO_ANY_DUAL_IMPL_T_H

#include /**/ "ace/pre.h"

#include "tao/AnyTypeCode/Any_Impl.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */


TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  /**
   * @class Any_Dual_Impl_T
   *
   * @brief Template Any class for IDL types with 2 modes of insertion
   *
   * Used for the IDL types that have copying and non-copying insertion,
   * but which are not (de)marshaled as pointers - struct, union, sequence,
   * and exception.
   */
  template<typename T>
  class Any_Dual_Impl_T : public Any_Impl
  {
  public:
    Any_Dual_Impl_T (_tao_destructor destructor,
                     CORBA::TypeCode_ptr,
                     T * const);
    Any_Dual_Impl_T (_tao_destructor destructor,
                     CORBA::TypeCode_ptr,
                     const T &);
    Any_Dual_Impl_T (CORBA::TypeCode_ptr);
    virtual ~Any_Dual_Impl_T (void);

    static void insert (CORBA::Any &,
                        _tao_destructor,
                        CORBA::TypeCode_ptr,
                        T * const);
    static void insert_copy (CORBA::Any &,
                             _tao_destructor destructor,
                             CORBA::TypeCode_ptr,
                             const T &);
    static CORBA::Boolean extract (const CORBA::Any &,
                                   _tao_destructor,
                                   CORBA::TypeCode_ptr,
                                   const T *&);

    virtual CORBA::Boolean marshal_value (TAO_OutputCDR &);
    CORBA::Boolean demarshal_value (TAO_InputCDR &);
    virtual void _tao_decode (TAO_InputCDR &);

    virtual const void *value (void) const;
    virtual void free_value (void);

  protected:
    void value (const T &);

    T * value_;

  private:
    static CORBA::Boolean replace (TAO_InputCDR &,
                                   const CORBA::Any &,
                                   _tao_destructor,
                                   CORBA::TypeCode_ptr,
                                   const T *&);
  };
}

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
# include "tao/AnyTypeCode/Any_Dual_Impl_T.inl"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "tao/AnyTypeCode/Any_Dual_Impl_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Any_Dual_Impl_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include /**/ "ace/post.h"

#endif /* TAO_ANY_DUAL_IMPL_T_H */

