// This may look like C, but it's really -*- C++ -*-

//=============================================================================
/**
 *  @file    UB_String_Argument_T.h
 *
 *  $Id$
 *
 *  @authors Jeff Parsons and Carlos O'Ryan
 */
//=============================================================================


#ifndef TAO_UB_STRING_ARGUMENT_T_H
#define TAO_UB_STRING_ARGUMENT_T_H

#include "ace/pre.h"
#include "tao/Argument.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

namespace TAO
{
  /**
   * @class In_UB_String_Argument_T
   *
   * @brief Template class for IN unbounded (w)string argument.
   *
   */
  template<typename S>
  class In_UB_String_Argument_T : public Argument
  {
  public:
    In_UB_String_Argument_T (const S * x);

    virtual CORBA::Boolean marshal (TAO_OutputCDR &);

    virtual void interceptor_param (Dynamic::Parameter &);
    virtual CORBA::Boolean interceptor_replace (CORBA::Any &);

  private:
    const S * x_;
  };

  /**
   * @class Inout_UB_String_Argument_T
   *
   * @brief Template class for INOUT unbounded (w)string argument.
   *
   */
  template<typename S>
  class Inout_UB_String_Argument_T : public Argument
  {
  public:
    Inout_UB_String_Argument_T (S *& x);

    virtual CORBA::Boolean marshal (TAO_OutputCDR &);
    virtual CORBA::Boolean demarshal (TAO_InputCDR &);

    virtual void interceptor_param (Dynamic::Parameter &);
    virtual CORBA::Boolean interceptor_replace (CORBA::Any &);

  private:
    S *& x_;
  };

  /**
   * @class Out_UB_String_Argument_T
   *
   * @brief Template class for INOUT unbounded (w)string argument.
   *
   */
  template<typename S, typename S_out>
  class Out_UB_String_Argument_T : public Argument
  {
  public:
    Out_UB_String_Argument_T (S_out & x);

    virtual CORBA::Boolean demarshal (TAO_InputCDR &);

  private:
    mutable S *& x_;
  };

  /**
   * @class Ret_UB_String_Argument_T
   *
   * @brief Template class for return stub value of ub (w)string argument.
   *
   */
  template<typename S, typename S_var>
  class Ret_UB_String_Argument_T : public Argument
  {
  public:
    Ret_UB_String_Argument_T (void);

    virtual CORBA::Boolean demarshal (TAO_InputCDR &);

    virtual void interceptor_result (CORBA::Any *);
    virtual CORBA::Boolean interceptor_replace (CORBA::Any &);

    S * excp (void);
    S * retn (void);

  private:
    S_var x_;
  };

  /**
   * @class In_UB_String_SArgument_T
   *
   * @brief Template class for IN skeleton UB (w)string argument.
   *
   */
  template<typename S, typename S_var>
  class In_UB_String_SArgument_T : public Argument
  {
  public:
    In_UB_String_SArgument_T (void);

    virtual CORBA::Boolean demarshal (TAO_InputCDR &);

    virtual void interceptor_param (Dynamic::Parameter &);
    virtual CORBA::Boolean interceptor_replace (CORBA::Any &);

    const S * arg (void) const;

  private:
    S_var x_;
  };

  /**
   * @class Inout_UB_String_SArgument_T
   *
   * @brief Template class for INOUT skeleton UB (w)string argument.
   *
   */
  template<typename S, typename S_var>
  class Inout_UB_String_SArgument_T : public Argument
  {
  public:
    Inout_UB_String_SArgument_T (void);

    virtual CORBA::Boolean marshal (TAO_OutputCDR &);
    virtual CORBA::Boolean demarshal (TAO_InputCDR &);

    virtual void interceptor_param (Dynamic::Parameter &);
    virtual CORBA::Boolean interceptor_replace (CORBA::Any &);

    S *& arg (void);

  private:
    S_var x_;
  };

  /**
   * @class Out_UB_String_SArgument_T
   *
   * @brief Template class for INOUT skeleton UB (w)string argument.
   *
   */
  template<typename S, typename S_var>
  class Out_UB_String_SArgument_T : public Argument
  {
  public:
    Out_UB_String_SArgument_T (void);

    virtual CORBA::Boolean marshal (TAO_OutputCDR &);

    S *& arg (void);

  private:
    S_var x_;
  };

  /**
   * @class Ret_UB_String_SArgument_T
   *
   * @brief Template class for return skeleton value of UB (w)string.
   *
   */
  template<typename S, typename S_var>
  class Ret_UB_String_SArgument_T : public Argument
  {
  public:
    Ret_UB_String_SArgument_T (void);

    virtual CORBA::Boolean marshal (TAO_OutputCDR &);

    virtual void interceptor_result (CORBA::Any *);
    virtual CORBA::Boolean interceptor_replace (CORBA::Any &);

    S *& arg (void);

  private:
    S_var x_;
  };

  /**
   * @struct UB_String_Tag
   *
   * @brief Struct for unbounded (w)string arguments id tag.
   *
   */
  struct TAO_Export UB_String_Tag {};

  /**
   * @struct UB_String_Arg_Traits_T
   *
   * @brief Template class for argument traits of unbounded (w)strings.
   *
   */
  template<typename T, typename T_var, typename T_out>
  struct UB_String_Arg_Traits_T
  {
    typedef T *                                         ret_type;
    typedef const T *                                   in_type;
    typedef T *&                                        inout_type;
    typedef T_out                                       out_type;

    typedef In_UB_String_Argument_T<T>                  in_arg_val;
    typedef Inout_UB_String_Argument_T<T>               inout_arg_val;
    typedef Out_UB_String_Argument_T<T,T_out>           out_arg_val;
    typedef Ret_UB_String_Argument_T<T,T_var>           stub_ret_val;

    typedef In_UB_String_SArgument_T<T,T_var>           in_sarg_val;
    typedef Inout_UB_String_SArgument_T<T,T_var>        inout_sarg_val;
    typedef Out_UB_String_SArgument_T<T,T_var>          out_sarg_val;
    typedef Ret_UB_String_SArgument_T<T,T_var>          skel_ret_val;

    typedef UB_String_Tag                               idl_tag;
  };
};

#if defined (__ACE_INLINE__)
#include "tao/UB_String_Argument_T.inl"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "tao/UB_String_Argument_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("UB_String_Argument_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include "ace/post.h"

#endif /* TAO_UB_STRING_ARGUMENT_T_H */

