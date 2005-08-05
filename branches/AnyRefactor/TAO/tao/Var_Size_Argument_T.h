// -*- C++ -*-

//=============================================================================
/**
 *  @file    Var_Size_Argument_T.h
 *
 *  $Id$
 *
 *  @authors Jeff Parsons and Carlos O'Ryan
 */
//=============================================================================

#ifndef TAO_VAR_SIZE_ARGUMENT_T_H
#define TAO_VAR_SIZE_ARGUMENT_T_H

#include /**/ "ace/pre.h"

#include "tao/Argument.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

namespace TAO
{
  /**
   * @class In_Var_Size_Argument_T
   *
   * @brief Template class for IN stub argument of fixed size IDL types.
   *
   */
  template<typename S>
  class In_Var_Size_Argument_T : public InArgument
  {
  public:
    In_Var_Size_Argument_T (S const & x);

    virtual CORBA::Boolean marshal (TAO_OutputCDR &);
#if TAO_HAS_INTERCEPTORS == 1
    virtual void interceptor_value (CORBA::Any *any) const;
#endif /* TAO_HAS_INTERCEPTORS == 1 */
    S const & arg (void) const;

  private:
    const S * x_;
  };

  /**
   * @class Inout_Var_Size_Argument_T
   *
   * @brief Template class for INOUT stub argument of fixed size IDL types.
   *
   */
  template<typename S>
  class Inout_Var_Size_Argument_T : public InoutArgument
  {
  public:
    Inout_Var_Size_Argument_T (S & x);

    virtual CORBA::Boolean marshal (TAO_OutputCDR &);
    virtual CORBA::Boolean demarshal (TAO_InputCDR &);
#if TAO_HAS_INTERCEPTORS == 1
    virtual void interceptor_value (CORBA::Any *any) const;
#endif /* TAO_HAS_INTERCEPTORS == 1 */
    S & arg (void);

  private:
    S * x_;
  };

  /**
   * @class Out_Var_Size_Argument_T
   *
   * @brief Template class for OUT stub argument of fixed size IDL types.
   *
   */
  template<typename S, typename S_out>
  class Out_Var_Size_Argument_T : public OutArgument
  {
  public:
    Out_Var_Size_Argument_T (S_out x);

    virtual CORBA::Boolean demarshal (TAO_InputCDR &);
#if TAO_HAS_INTERCEPTORS == 1
    virtual void interceptor_value (CORBA::Any *any) const;
#endif /* TAO_HAS_INTERCEPTORS == 1 */
    S *& arg (void);

  private:
    S *& x_;
  };

  /**
   * @class Ret_Var_Size_Argument_T
   *
   * @brief Template class for return stub value of fixed size IDL types.
   *
   */
  template<typename S, typename S_var>
  class Ret_Var_Size_Argument_T : public RetArgument
  {
  public:
    Ret_Var_Size_Argument_T (void);

    virtual CORBA::Boolean demarshal (TAO_InputCDR &);
#if TAO_HAS_INTERCEPTORS == 1
    virtual void interceptor_value (CORBA::Any *any) const;
#endif /* TAO_HAS_INTERCEPTORS == 1 */
    S *& arg (void);

    S * excp (void);
    S * retn (void);

  protected:
    S_var x_;
  };

  /**
   * @struct Basic_Tag
   *
   * @brief Struct for fixed size IDL type arguments id tag.
   *
   */
  struct TAO_Export Var_Size_Tag {};

  /**
   * @struct Basic_Arg_Traits_T
   *
   * @brief Template class for stub argument traits of
   *  variable size IDL types.
   *
   */
  template<typename T, typename T_var, typename T_out>
  struct Var_Size_Arg_Traits_T
  {
    typedef T *                                         ret_type;
    typedef T const &                                   in_type;
    typedef T &                                         inout_type;
    typedef T_out                                       out_type;

    typedef In_Var_Size_Argument_T<T>                   in_arg_val;
    typedef Inout_Var_Size_Argument_T<T>                inout_arg_val;
    typedef Out_Var_Size_Argument_T<T,T_out>            out_arg_val;
    typedef Ret_Var_Size_Argument_T<T,T_var>            ret_val;

    typedef Var_Size_Tag                                idl_tag;
  };
}

#if defined (__ACE_INLINE__)
#include "tao/Var_Size_Argument_T.inl"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "tao/Var_Size_Argument_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Var_Size_Argument_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include /**/ "ace/post.h"

#endif /* TAO_VAR_SIZE_ARGUMENT_T_H */
