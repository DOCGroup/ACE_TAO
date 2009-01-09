// -*- C++ -*-

//=============================================================================
/**
 *  @file    UB_String_SArgument_T.h
 *
 *  $Id$
 *
 *  @authors Jeff Parsons, Carlos O'Ryan and Ossama Othman
 */
//=============================================================================

#ifndef TAO_UB_STRING_SARGUMENT_T_H
#define TAO_UB_STRING_SARGUMENT_T_H

#include /**/ "ace/pre.h"

#include "tao/Argument.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  /**
   * @class In_UB_String_SArgument_T
   *
   * @brief Template class for IN skeleton UB (w)string argument.
   *
   */
  template<typename S, typename S_var>
  class In_UB_String_SArgument_T : public InArgument
  {
  public:
    In_UB_String_SArgument_T (void);

    virtual CORBA::Boolean demarshal (TAO_InputCDR &);
#if TAO_HAS_INTERCEPTORS == 1
    virtual void interceptor_value (CORBA::Any *any) const;
#endif /* TAO_HAS_INTERCEPTORS == 1 */
    S const * arg (void) const;

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
  class Inout_UB_String_SArgument_T : public InoutArgument
  {
  public:
    Inout_UB_String_SArgument_T (void);

    virtual CORBA::Boolean marshal (TAO_OutputCDR &cdr);
    virtual CORBA::Boolean demarshal (TAO_InputCDR &);
#if TAO_HAS_INTERCEPTORS == 1
    virtual void interceptor_value (CORBA::Any *any) const;
#endif /* TAO_HAS_INTERCEPTORS == 1 */
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
  class Out_UB_String_SArgument_T : public OutArgument
  {
  public:
    Out_UB_String_SArgument_T (void);

    virtual CORBA::Boolean marshal (TAO_OutputCDR &cdr);
#if TAO_HAS_INTERCEPTORS == 1
    virtual void interceptor_value (CORBA::Any *any) const;
#endif /* TAO_HAS_INTERCEPTORS == 1 */
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
  class Ret_UB_String_SArgument_T : public RetArgument
  {
  public:
    Ret_UB_String_SArgument_T (void);

    virtual CORBA::Boolean marshal (TAO_OutputCDR &cdr);
#if TAO_HAS_INTERCEPTORS == 1
    virtual void interceptor_value (CORBA::Any *any) const;
#endif /* TAO_HAS_INTERCEPTORS == 1 */
    S *& arg (void);

  private:
    S_var x_;
  };

  /**
   * @struct UB_String_SArg_Traits_T
   *
   * @brief Template class for argument traits of unbounded (w)strings.
   *
   */
  template<typename T, typename T_var, typename T_out>
  struct UB_String_SArg_Traits_T
  {
    typedef T *                                         ret_type;
    typedef const T *                                   in_type;
    typedef T *&                                        inout_type;
    typedef T_out                                       out_type;

    typedef In_UB_String_SArgument_T<T,T_var>           in_arg_val;
    typedef Inout_UB_String_SArgument_T<T,T_var>        inout_arg_val;
    typedef Out_UB_String_SArgument_T<T,T_var>          out_arg_val;
    typedef Ret_UB_String_SArgument_T<T,T_var>          ret_val;

    // Typedefs corresponding to return value of arg() method in both
    // the client and server side argument class templates.
    typedef in_type                                     in_arg_type;
    typedef inout_type                                  inout_arg_type;
    typedef inout_type                                  out_arg_type;
    typedef inout_type                                  ret_arg_type;

  };
}

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "tao/PortableServer/UB_String_SArgument_T.inl"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "tao/PortableServer/UB_String_SArgument_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("UB_String_SArgument_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include /**/ "ace/post.h"

#endif /* TAO_UB_STRING_SARGUMENT_T_H */
