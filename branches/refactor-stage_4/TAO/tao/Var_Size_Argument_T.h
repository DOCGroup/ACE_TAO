// This may look like C, but it's really -*- C++ -*-

//=============================================================================
/**
 *  @file    Basic_Argument_T.h
 *
 *  $Id$
 *
 *  @authors Jeff Parsons and Carlos O'Ryan
 */
//=============================================================================


#ifndef TAO_VAR_SIZE_ARGUMENT_T_H
#define TAO_VAR_SIZE_ARGUMENT_T_H

#include "ace/pre.h"
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
  class In_Var_Size_Argument_T : public Argument
  {
  public:
    In_Var_Size_Argument_T (S const & x);

    virtual CORBA::Boolean marshal (TAO_OutputCDR &);

    virtual void interceptor_param (Dynamic::Parameter &);
    virtual CORBA::Boolean interceptor_replace (CORBA::Any &);

    const S & arg (void) const;

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
  class Inout_Var_Size_Argument_T : public Argument
  {
  public:
    Inout_Var_Size_Argument_T (S & x);

    virtual CORBA::Boolean marshal (TAO_OutputCDR &);
    virtual CORBA::Boolean demarshal (TAO_InputCDR &);

    virtual void interceptor_param (Dynamic::Parameter &);
    virtual CORBA::Boolean interceptor_replace (CORBA::Any &);

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
  class Out_Var_Size_Argument_T : public Argument
  {
  public:
    Out_Var_Size_Argument_T (S_out x);

    virtual CORBA::Boolean demarshal (TAO_InputCDR &);

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
  class Ret_Var_Size_Argument_T : public Argument
  {
  public:
    Ret_Var_Size_Argument_T (void);

    virtual CORBA::Boolean demarshal (TAO_InputCDR &);

    virtual void interceptor_result (CORBA::Any *);
    virtual CORBA::Boolean interceptor_replace (CORBA::Any &);

    S *& arg (void);

    S * excp (void);
    S * retn (void);

  protected:
    S_var x_;
  };

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
    virtual CORBA::Boolean interceptor_replace (CORBA::Any &);

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
    virtual CORBA::Boolean interceptor_replace (CORBA::Any &);

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
    virtual CORBA::Boolean interceptor_replace (CORBA::Any &);

    S *& arg (void);

  private:
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
   * @brief Template class for argument traits of fixed size IDL types.
   *
   */
  template<typename T, typename T_var, typename T_out>
  struct Var_Size_Arg_Traits_T
  {
    typedef T *                                         ret_type;
    typedef const T &                                   in_type;
    typedef T &                                         inout_type;
    typedef T_out                                       out_type;

    typedef In_Var_Size_Argument_T<T>                   in_arg_val;
    typedef Inout_Var_Size_Argument_T<T>                inout_arg_val;
    typedef Out_Var_Size_Argument_T<T,T_out>            out_arg_val;
    typedef Ret_Var_Size_Argument_T<T,T_var>            stub_ret_val;

    typedef In_Var_Size_SArgument_T<T>                  in_sarg_val;
    typedef Inout_Var_Size_SArgument_T<T>               inout_sarg_val;
    typedef Out_Var_Size_SArgument_T<T,T_var>           out_sarg_val;
    typedef Ret_Var_Size_SArgument_T<T,T_var>           skel_ret_val;

    typedef Var_Size_Tag                                idl_tag;
  };
};

#if defined (__ACE_INLINE__)
#include "tao/Var_Size_Argument_T.inl"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "tao/Var_Size_Argument_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Var_Size_Argument_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include "ace/post.h"

#endif /* TAO_VAR_SIZE_ARGUMENT_T_H */
