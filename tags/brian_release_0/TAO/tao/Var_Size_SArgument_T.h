// This may look like C, but it's really -*- C++ -*-

//=============================================================================
/**
 *  @file    Var_Size_SArgument_T.h
 *
 *  $Id$
 *
 *  @authors Jeff Parsons and Carlos O'Ryan
 */
//=============================================================================

#ifndef TAO_VAR_SIZE_SARGUMENT_T_H
#define TAO_VAR_SIZE_SARGUMENT_T_H

#include /**/ "ace/pre.h"
#include "tao/Argument.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

namespace TAO
{
  /**
   * @class In_Var_Size_SArgument_T
   *
   * @brief Template class for IN skeleton argument of fixed size IDL types.
   *
   */
  template<typename S>
  class In_Var_Size_SArgument_T : public Argument
  {
  public:
    In_Var_Size_SArgument_T (void);

    virtual CORBA::Boolean demarshal (TAO_InputCDR &);

    virtual void interceptor_param (Dynamic::Parameter &);

    const S & arg (void) const;

  private:
    S * x_;
  };

  /**
   * @class Inout_Var_Size_SArgument_T
   *
   * @brief Template class for INOUT skeleton arg of fixed size IDL types.
   *
   */
  template<typename S>
  class Inout_Var_Size_SArgument_T : public Argument
  {
  public:
    Inout_Var_Size_SArgument_T (void);

    virtual CORBA::Boolean marshal (TAO_OutputCDR &);
    virtual CORBA::Boolean demarshal (TAO_InputCDR &);

    virtual void interceptor_param (Dynamic::Parameter &);

    S & arg (void);

  private:
    S * x_;
  };

  /**
   * @class Out_Var_Size_SArgument_T
   *
   * @brief Template class for OUT skeleton argument of fixed size IDL types.
   *
   */
  template<typename S, typename S_var>
  class Out_Var_Size_SArgument_T : public Argument
  {
  public:
    Out_Var_Size_SArgument_T (void);

    virtual CORBA::Boolean marshal (TAO_OutputCDR &);

    virtual void interceptor_param (Dynamic::Parameter &);

    S *& arg (void);

  private:
    S_var x_;
  };

  /**
   * @class Ret_Var_Size_SArgument_T
   *
   * @brief Template class for return skeleton value of fixed size IDL types.
   *
   */
  template<typename S, typename S_var>
  class Ret_Var_Size_SArgument_T : public Argument
  {
  public:
    Ret_Var_Size_SArgument_T (void);

    virtual CORBA::Boolean marshal (TAO_OutputCDR &);

    virtual void interceptor_result (CORBA::Any *);

    S *& arg (void);

  private:
    S_var x_;
  };

  /**
   * @struct Basic_SArg_Traits_T
   *
   * @brief Template class for skeleton argument traits of
   *  variable size IDL types.
   *
   */
  template<typename T, typename T_var, typename T_out>
  struct Var_Size_SArg_Traits_T
  {
    typedef T *                                         ret_type;
    typedef const T &                                   in_type;
    typedef T &                                         inout_type;
    typedef T_out                                       out_type;

    typedef In_Var_Size_SArgument_T<T>                  in_arg_val;
    typedef Inout_Var_Size_SArgument_T<T>               inout_arg_val;
    typedef Out_Var_Size_SArgument_T<T,T_var>           out_arg_val;
    typedef Ret_Var_Size_SArgument_T<T,T_var>           ret_val;
  };
};

#if defined (__ACE_INLINE__)
#include "tao/Var_Size_SArgument_T.inl"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "tao/Var_Size_SArgument_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Var_Size_SArgument_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include /**/ "ace/post.h"

#endif /* TAO_VAR_SIZE_SARGUMENT_T_H */
