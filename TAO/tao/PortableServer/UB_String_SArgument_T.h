// -*- C++ -*-

//=============================================================================
/**
 *  @file    UB_String_SArgument_T.h
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
    In_UB_String_SArgument_T ();

    CORBA::Boolean demarshal (TAO_InputCDR &) override;
#if TAO_HAS_INTERCEPTORS == 1
    void interceptor_value (CORBA::Any *any) const override;
#endif /* TAO_HAS_INTERCEPTORS == 1 */
    S const * arg () const;

  private:
    S_var x_ {};
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
    Inout_UB_String_SArgument_T ();

    CORBA::Boolean marshal (TAO_OutputCDR &cdr) override;
    CORBA::Boolean demarshal (TAO_InputCDR &) override;
#if TAO_HAS_INTERCEPTORS == 1
    void interceptor_value (CORBA::Any *any) const override;
#endif /* TAO_HAS_INTERCEPTORS == 1 */
    S *& arg ();

  private:
    S_var x_ {};
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
    Out_UB_String_SArgument_T ();

    CORBA::Boolean marshal (TAO_OutputCDR &cdr) override;
#if TAO_HAS_INTERCEPTORS == 1
    void interceptor_value (CORBA::Any *any) const override;
#endif /* TAO_HAS_INTERCEPTORS == 1 */
    S *& arg ();

  private:
    S_var x_ {};
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
    Ret_UB_String_SArgument_T ();

    CORBA::Boolean marshal (TAO_OutputCDR &cdr) override;
#if TAO_HAS_INTERCEPTORS == 1
    void interceptor_value (CORBA::Any *any) const override;
#endif /* TAO_HAS_INTERCEPTORS == 1 */
    S *& arg ();

  private:
    S_var x_ {};
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
    using ret_type = T *;
    using in_type = T const *;
    using inout_type = T *&;
    using out_type = T_out;

    using in_arg_val = In_UB_String_SArgument_T<T, T_var>;
    using inout_arg_val = Inout_UB_String_SArgument_T<T, T_var>;
    using out_arg_val = Out_UB_String_SArgument_T<T, T_var>;
    using ret_val = Ret_UB_String_SArgument_T<T, T_var>;

    // Typedefs corresponding to return value of arg() method in both
    // the client and server side argument class templates.
    using in_arg_type = in_type;
    using inout_arg_type = inout_type;
    using out_arg_type = inout_type;
    using ret_arg_type = inout_type;
  };
}

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "tao/PortableServer/UB_String_SArgument_T.inl"
#endif /* __ACE_INLINE__ */

#include "tao/PortableServer/UB_String_SArgument_T.cpp"

#include /**/ "ace/post.h"

#endif /* TAO_UB_STRING_SARGUMENT_T_H */
