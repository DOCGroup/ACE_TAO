// This may look like C, but it's really -*- C++ -*-

//=============================================================================
/**
 *  @file    Any_Impl_T.h
 *
 *  $Id$
 *
 *  @authors  Carlos O'Ryan and Jeff Parsons
 */
//=============================================================================


#ifndef TAO_ANY_IMPL_T_H
#define TAO_ANY_IMPL_T_H

#include /**/ "ace/pre.h"

#include "tao/Any.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

namespace TAO
{
  /**
   * @class Any_Impl_T
   *
   * @brief Template Any class for pointer types.
   *
   * Used for interfaces and typecodes
   */
  template<typename T> 
  class Any_Impl_T : public Any_Impl
  {
  public:
    Any_Impl_T (_tao_destructor destructor,
                CORBA::TypeCode_ptr,
                T * const);
    virtual ~Any_Impl_T (void);

    static void insert (CORBA::Any &, 
                        _tao_destructor destructor,
                        CORBA::TypeCode_ptr,
                        T * const);
    static CORBA::Boolean extract (const CORBA::Any &,
                                   _tao_destructor,
                                   CORBA::TypeCode_ptr,
                                   T *&);

    virtual CORBA::Boolean to_object (CORBA::Object_ptr &) const;
    virtual CORBA::Boolean to_value (CORBA::ValueBase *&) const;
    virtual CORBA::Boolean to_abstract_base (CORBA::AbstractBase_ptr &) const;

    virtual CORBA::Boolean marshal_value (TAO_OutputCDR &);
    CORBA::Boolean demarshal_value (TAO_InputCDR &);
    virtual void _tao_decode (TAO_InputCDR &
                              ACE_ENV_ARG_DECL_WITH_DEFAULTS);

    virtual const void *value (void) const;
    virtual void free_value (void);

  private:
    T * value_;
  };
};

#if defined (__ACE_INLINE__)
# include "tao/Any_Impl_T.inl"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "tao/Any_Impl_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Any_Impl_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include /**/ "ace/post.h"

#endif /* TAO_ANY_IMPL_T_H */


