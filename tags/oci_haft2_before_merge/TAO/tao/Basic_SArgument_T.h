// This may look like C, but it's really -*- C++ -*-

//=============================================================================
/**
 *  @file    Basic_SArgument_T.h
 *
 *  $Id$
 *
 *  @authors Jeff Parsons and Carlos O'Ryan
 */
//=============================================================================


#ifndef TAO_BASIC_SARGUMENT_T_H
#define TAO_BASIC_SARGUMENT_T_H

#include /**/ "ace/pre.h"

#include "tao/Argument.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

namespace TAO
{
  /**
   * @class In_Basic_SArgument_T
   *
   * @brief Template class for IN skeleton argument of basic IDL types.
   *
   */
  template<typename S>
  class In_Basic_SArgument_T : public Argument
  {
  public:
    In_Basic_SArgument_T (void);

    virtual CORBA::Boolean demarshal (TAO_InputCDR &);

    virtual void interceptor_param (Dynamic::Parameter &);

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
  template<typename S>
  class Inout_Basic_SArgument_T : public Argument
  {
  public:
    Inout_Basic_SArgument_T (void);

    virtual CORBA::Boolean marshal (TAO_OutputCDR &);
    virtual CORBA::Boolean demarshal (TAO_InputCDR &);

    virtual void interceptor_param (Dynamic::Parameter &);

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
  template<typename S>
  class Out_Basic_SArgument_T : public Argument
  {
  public:
    Out_Basic_SArgument_T (void);

    virtual CORBA::Boolean marshal (TAO_OutputCDR &);

    virtual void interceptor_param (Dynamic::Parameter &);

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
  template<typename S>
  class Ret_Basic_SArgument_T : public Argument
  {
  public:
    Ret_Basic_SArgument_T (void);

    virtual CORBA::Boolean marshal (TAO_OutputCDR &);

    virtual void interceptor_result (CORBA::Any *);

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
  template<typename T>
  struct Basic_SArg_Traits_T
  {
    typedef T                           ret_type;
    typedef T                           in_type;
    typedef T &                         inout_type;
    typedef T &                         out_type;

    typedef In_Basic_SArgument_T<T>     in_arg_val;
    typedef Inout_Basic_SArgument_T<T>  inout_arg_val;
    typedef Out_Basic_SArgument_T<T>    out_arg_val;
    typedef Ret_Basic_SArgument_T<T>    ret_val;
  };
};

#if defined (__ACE_INLINE__)
#include "tao/Basic_SArgument_T.inl"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "tao/Basic_SArgument_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Basic_SArgument_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include /**/ "ace/post.h"

#endif /* TAO_BASIC_SARGUMENT_T_H */
