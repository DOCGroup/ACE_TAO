// -*- C++ -*-

//=============================================================================
/**
 *  @file    Special_Basic_SArgument_T.h
 *
 *  $Id$
 *
 *  @authors Jeff Parsons, Carlos O'Ryan and Ossama Othman
 */
//=============================================================================


#ifndef TAO_SPECIAL_BASIC_SARGUMENT_T_H
#define TAO_SPECIAL_BASIC_SARGUMENT_T_H

#include /**/ "ace/pre.h"

#include "tao/Argument.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

namespace TAO
{
  /**
   * @class In_Special_Basic_SArgument_T
   *
   * @brief Template class for IN skeleton argument of (w)char/boolean/octet.
   *
   */
  template<typename S, typename to_S, typename from_S>
  class In_Special_Basic_SArgument_T : public Argument
  {
  public:
    In_Special_Basic_SArgument_T (void);

    virtual CORBA::Boolean demarshal (TAO_InputCDR &);
#if TAO_HAS_INTERCEPTORS == 1
    virtual void interceptor_param (Dynamic::Parameter &);
#endif /* TAO_HAS_INTERCEPTORS == 1 */
    S const & arg (void) const;

  private:
    S x_;
  };

  /**
   * @class Inout_Special_Basic_SArgument_T
   *
   * @brief Template class for INOUT (w)char/boolean/octet skeleton argument.
   *
   */
  template<typename S, typename to_S, typename from_S>
  class Inout_Special_Basic_SArgument_T : public Argument
  {
  public:
    Inout_Special_Basic_SArgument_T (void);

    virtual CORBA::Boolean marshal (TAO_OutputCDR &);
    virtual CORBA::Boolean demarshal (TAO_InputCDR &);
#if TAO_HAS_INTERCEPTORS == 1
    virtual void interceptor_param (Dynamic::Parameter &);
#endif /* TAO_HAS_INTERCEPTORS == 1 */
    S & arg (void);

  private:
    S x_;
  };

  /**
   * @class Out_Special_Basic_SArgument_T
   *
   * @brief Template class for OUT skeleton argument of (w)char/boolean/octet.
   *
   */
  template<typename S, typename to_S, typename from_S>
  class Out_Special_Basic_SArgument_T : public Argument
  {
  public:
    Out_Special_Basic_SArgument_T (void);

    virtual CORBA::Boolean marshal (TAO_OutputCDR &);
#if TAO_HAS_INTERCEPTORS == 1
    virtual void interceptor_param (Dynamic::Parameter &);
#endif /* TAO_HAS_INTERCEPTORS == 1 */
    S & arg (void);

  private:
    S x_;
  };

  /**
   * @class Ret_Basic_SArgument_T
   *
   * @brief Template class for return skeleton value of (w)char/boolean/octet.
   *
   */
  template<typename S, typename to_S, typename from_S>
  class Ret_Special_Basic_SArgument_T : public Argument
  {
  public:
    Ret_Special_Basic_SArgument_T (void);

    virtual CORBA::Boolean marshal (TAO_OutputCDR &);
#if TAO_HAS_INTERCEPTORS == 1
    virtual void interceptor_result (CORBA::Any *);
#endif /* TAO_HAS_INTERCEPTORS == 1 */
    S & arg (void);

  private:
    S x_;
  };

  /**
   * @struct Special_Basic_Traits_T
   *
   * @brief Template class for skeleton argument traits
   *  of (w)char/boolean/octet.
   *
   */
  template<typename T, typename to_T, typename from_T>
  struct Special_Basic_SArg_Traits_T
  {
    typedef T                                               ret_type;
    typedef T                                               in_type;
    typedef T &                                             inout_type;
    typedef T &                                             out_type;

    typedef In_Special_Basic_SArgument_T<T,to_T,from_T>     in_arg_val;
    typedef Inout_Special_Basic_SArgument_T<T,to_T,from_T>  inout_arg_val;
    typedef Out_Special_Basic_SArgument_T<T,to_T,from_T>    out_arg_val;
    typedef Ret_Special_Basic_SArgument_T<T,to_T,from_T>    ret_val;

    // Typedefs corresponding to return value of arg() method in both
    // the client and server side argument class templates.
    typedef T const &                                       in_arg_type;
    typedef inout_type                                      inout_arg_type;
    typedef out_type                                        out_arg_type;
    typedef out_type                                        ret_arg_type;

  };
}

#if defined (__ACE_INLINE__)
#include "tao/PortableServer/Special_Basic_SArgument_T.inl"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "tao/PortableServer/Special_Basic_SArgument_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Special_Basic_SArgument_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include /**/ "ace/post.h"

#endif /* TAO_SPECIAL_BASIC_SARGUMENT_T_H */
