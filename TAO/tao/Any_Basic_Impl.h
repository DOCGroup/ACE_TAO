// -*- C++ -*-
//=============================================================================
/**
 *  @file    Any_Basic_Impl.h
 *
 *  $Id$
 *
 *  @authors  Carlos O'Ryan and Jeff Parsons
 */
//=============================================================================

#ifndef TAO_ANY_BASIC_IMPL_H
#define TAO_ANY_BASIC_IMPL_H

#include /**/ "ace/pre.h"

#include "tao/Any_Impl.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

namespace CORBA
{
  class Any;
}

namespace TAO
{
  /**
   * @class Any_Basic_Impl
   *
   * @brief Non-template class for all the basic types.
   *
   */
  class TAO_Export Any_Basic_Impl : public Any_Impl
  {
  public:
    Any_Basic_Impl (CORBA::TypeCode_ptr,
                    const void *value);

    virtual ~Any_Basic_Impl (void);

    static void insert (CORBA::Any &,
                        CORBA::TypeCode_ptr,
                        const void *);
    static CORBA::Boolean extract (const CORBA::Any &, 
                                   CORBA::TypeCode_ptr,
                                   void *);

    virtual CORBA::Boolean marshal_value (TAO_OutputCDR &);
    CORBA::Boolean demarshal_value (TAO_InputCDR &);
    virtual void _tao_decode (TAO_InputCDR &
                              ACE_ENV_ARG_DECL_WITH_DEFAULTS);

    static Any_Basic_Impl *create_empty (CORBA::TypeCode_ptr);

  private:
    static void assign_value (void *, Any_Basic_Impl *);

  private:
    CORBA::Long kind_;
    union
    {
      CORBA::Short s;
      CORBA::UShort us;
      CORBA::Long l;
      CORBA::ULong ul;
      CORBA::Float f;
      CORBA::Double d;
      CORBA::Boolean b;
      CORBA::Char c;
      CORBA::Octet o;
      CORBA::LongLong ll;
      CORBA::ULongLong ull;
      CORBA::LongDouble ld;
      CORBA::WChar wc;
    }u_;
  };
}

#include /**/ "ace/post.h"

#endif /* TAO_ANY_BASIC_IMPL_H */
