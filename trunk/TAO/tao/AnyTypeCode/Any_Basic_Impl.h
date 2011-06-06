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

#include "tao/AnyTypeCode/Any_Impl.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

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
  class TAO_AnyTypeCode_Export Any_Basic_Impl : public Any_Impl
  {
  public:
    Any_Basic_Impl (CORBA::TypeCode_ptr,
                    void *value);

    virtual ~Any_Basic_Impl (void);

    static void insert (CORBA::Any &,
                        CORBA::TypeCode_ptr,
                        const void *);
    static CORBA::Boolean extract (const CORBA::Any &,
                                   CORBA::TypeCode_ptr,
                                   void *);

    virtual CORBA::Boolean marshal_value (TAO_OutputCDR &);

    CORBA::Boolean demarshal_value (TAO_InputCDR &);
    CORBA::Boolean demarshal_value (TAO_InputCDR &,
                                    CORBA::Long);

    virtual void _tao_decode (TAO_InputCDR &);

    static Any_Basic_Impl *create_empty (CORBA::TypeCode_ptr);

  private:
    static void assign_value (void *, Any_Basic_Impl *);

    static void assign_value (void *,
                              Any_Basic_Impl *,
                              CORBA::Long tck);
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
#if !defined (ACE_LACKS_LONGLONG_T) && !defined (ACE_LACKS_UNSIGNEDLONGLONG_T)
      CORBA::ULongLong ull;
#elif defined (ACE_LACKS_UNSIGNEDLONGLONG_T)
      // Platform has signed long long but not unsigned long long.
      // Use signed long long as the container.
      CORBA::LongLong ull;
#endif
      CORBA::WChar wc;
    }u_;

    CORBA::LongDouble ld_;
  };
}

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* TAO_ANY_BASIC_IMPL_H */
