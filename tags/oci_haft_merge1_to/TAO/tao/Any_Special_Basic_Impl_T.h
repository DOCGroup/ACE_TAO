// This may look like C, but it's really -*- C++ -*-

//=============================================================================
/**
 *  @file    Any_Special_Basic_Impl_T.h
 *
 *  $Id$
 *
 *  @authorS  Carlos O'Ryan and Jeff Parsons
 */
//=============================================================================


#ifndef TAO_ANY_SPECIAL_BASIC_IMPL_T_H
#define TAO_ANY_SPECIAL_BASIC_IMPL_T_H

#include "ace/pre.h"

#include "tao/Any.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

namespace TAO
{
  /**
   * @class Any_Special_Basic_Impl_T
   *
   * @brief Template Any class for special basic types.
   *
   * Used for the basic IDL types that usually map to unsigned char
   * or unsigned short - boolean, char, wchar and octet.
   */
  template<typename T, typename from_T, typename to_T>
  class Any_Special_Basic_Impl_T : public Any_Impl
  {
  public:
    Any_Special_Basic_Impl_T (CORBA::TypeCode_ptr,
                              const T & val);
    virtual ~Any_Special_Basic_Impl_T (void);

    static void insert (CORBA::Any &,
                        CORBA::TypeCode_ptr,
                        const T &);
    static CORBA::Boolean extract (const CORBA::Any &, 
                                   CORBA::TypeCode_ptr,
                                   T &);

    virtual CORBA::Boolean marshal_value (TAO_OutputCDR &);
    CORBA::Boolean demarshal_value (TAO_InputCDR &);
    virtual void _tao_decode (TAO_InputCDR &
                              ACE_ENV_ARG_DECL_WITH_DEFAULTS);

    virtual const void *value (void) const;

  private:
    T value_;
  };
};

#if defined (__ACE_INLINE__)
# include "tao/Any_Special_Basic_Impl_T.inl"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "tao/Any_Special_Basic_Impl_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Any_Special_Basic_Impl_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include "ace/post.h"

#endif /* TAO_ANY_SPECIAL_BASIC_IMPL_T_H */

