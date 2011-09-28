// -*- C++ -*-

//=============================================================================
/**
 *  @file    Basic_SArgument_T.h
 *
 *  $Id$
 *
 *  @authors Jeff Parsons, Carlos O'Ryan and Ossama Othman
 */
//=============================================================================


#ifndef TAO_BASIC_SARGUMENT_T_H
#define TAO_BASIC_SARGUMENT_T_H

#include /**/ "ace/pre.h"

#include "tao/Argument.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  /**
   * @class In_Basic_SArgument_T
   *
   * @brief Template class for IN skeleton argument of basic IDL types.
   *
   */
  template<typename S, template <typename> class Insert_Policy>
  class In_Basic_SArgument_T : public InArgument
  {
  public:
    In_Basic_SArgument_T (void);

    virtual CORBA::Boolean demarshal (TAO_InputCDR &);
#if TAO_HAS_INTERCEPTORS == 1
    virtual void interceptor_value (CORBA::Any *any) const;
#endif /* TAO_HAS_INTERCEPTORS == 1 */
    S arg (void) const;

  private:
    S x_;
  };

  /**
   * @class Inout_Basic_SArgument_T
   *
   * @brief Template class for INOUT skeleton argument of basic IDL types.
   *
   */
  template<typename S, template <typename> class Insert_Policy>
  class Inout_Basic_SArgument_T : public InoutArgument
  {
  public:
    Inout_Basic_SArgument_T (void);

    virtual CORBA::Boolean marshal (TAO_OutputCDR &cdr);
    virtual CORBA::Boolean demarshal (TAO_InputCDR &);
#if TAO_HAS_INTERCEPTORS == 1
    virtual void interceptor_value (CORBA::Any *any) const;
#endif /* TAO_HAS_INTERCEPTORS == 1 */
    S & arg (void);

  private:
    S x_;
  };

  /**
   * @class Out_Basic_SArgument_T
   *
   * @brief Template class for OUT skeleton argument of basic IDL types.
   *
   */
  template<typename S, template <typename> class Insert_Policy>
  class Out_Basic_SArgument_T : public OutArgument
  {
  public:
    Out_Basic_SArgument_T (void);

    virtual CORBA::Boolean marshal (TAO_OutputCDR &cdr);
#if TAO_HAS_INTERCEPTORS == 1
    virtual void interceptor_value (CORBA::Any *any) const;
#endif /* TAO_HAS_INTERCEPTORS == 1 */
    S & arg (void);

  private:
    S x_;
  };

  /**
   * @class Ret_Basic_SArgument_T
   *
   * @brief Template class for return skeleton value of basic IDL types.
   *
   */
  template<typename S, template <typename> class Insert_Policy>
  class Ret_Basic_SArgument_T : public RetArgument
  {
  public:
    Ret_Basic_SArgument_T (void);

    virtual CORBA::Boolean marshal (TAO_OutputCDR &cdr);
#if TAO_HAS_INTERCEPTORS == 1
    virtual void interceptor_value (CORBA::Any *any) const;
#endif /* TAO_HAS_INTERCEPTORS == 1 */
    S & arg (void);

  private:
    S x_;
  };

  /**
   * @struct Basic_SArg_Traits_T
   *
   * @brief Template class for skeleton argument traits of basic IDL types.
   *
   */
  template<typename T, template <typename> class Insert_Policy>
  struct Basic_SArg_Traits_T
  {
    typedef T                           ret_type;
    typedef T                           in_type;
    typedef T &                         inout_type;
    typedef T &                         out_type;

    typedef In_Basic_SArgument_T<T, Insert_Policy>     in_arg_val;
    typedef Inout_Basic_SArgument_T<T, Insert_Policy>  inout_arg_val;
    typedef Out_Basic_SArgument_T<T, Insert_Policy>    out_arg_val;
    typedef Ret_Basic_SArgument_T<T, Insert_Policy>    ret_val;

    // Typedefs corresponding to return value of arg() method in both
    // the client and server side argument class templates.
    typedef in_type                     in_arg_type;
    typedef inout_type                  inout_arg_type;
    typedef out_type                    out_arg_type;
    typedef out_type                    ret_arg_type;
  };
}

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "tao/PortableServer/Basic_SArgument_T.inl"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "tao/PortableServer/Basic_SArgument_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Basic_SArgument_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include /**/ "ace/post.h"

#endif /* TAO_BASIC_SARGUMENT_T_H */
