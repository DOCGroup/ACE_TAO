// -*- C++ -*-

//=============================================================================
/**
 *  @file    UB_String_Argument_T.h
 *
 *  $Id$
 *
 *  @author Jeff Parsons
 *  @author Carlos O'Ryan
 */
//=============================================================================

#ifndef TAO_UB_STRING_ARGUMENT_T_H
#define TAO_UB_STRING_ARGUMENT_T_H

#include /**/ "ace/pre.h"

#include "tao/Argument.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  /**
   * @class In_UB_String_Argument_T
   *
   * @brief Template class for IN unbounded (w)string argument.
   *
   */
  template<typename S, typename Insert_Policy>
  class In_UB_String_Argument_T : public InArgument, private Insert_Policy
  {
  public:
    In_UB_String_Argument_T (const S * x);

    virtual CORBA::Boolean marshal (TAO_OutputCDR &cdr);
#if TAO_HAS_INTERCEPTORS == 1
    virtual void interceptor_value (CORBA::Any *any) const;
#endif /* TAO_HAS_INTERCEPTORS == 1 */
    S const * arg (void) const;

  private:
    S const * x_;
  };

  /**
   * @class Inout_UB_String_Argument_T
   *
   * @brief Template class for INOUT unbounded (w)string argument.
   *
   */
  template<typename S, typename Insert_Policy>
  class Inout_UB_String_Argument_T : public InoutArgument, private Insert_Policy
  {
  public:
    Inout_UB_String_Argument_T (S *& x);

    virtual CORBA::Boolean marshal (TAO_OutputCDR &cdr);
    virtual CORBA::Boolean demarshal (TAO_InputCDR &);
#if TAO_HAS_INTERCEPTORS == 1
    virtual void interceptor_value (CORBA::Any *any) const;
#endif /* TAO_HAS_INTERCEPTORS == 1 */
    S *& arg (void);

  private:
    S *& x_;
  };

  /**
   * @class Out_UB_String_Argument_T
   *
   * @brief Template class for OUT unbounded (w)string argument.
   *
   */
  template<typename S, typename S_out, typename Insert_Policy>
  class Out_UB_String_Argument_T : public OutArgument, private Insert_Policy
  {
  public:
    Out_UB_String_Argument_T (S_out & x);

    virtual CORBA::Boolean demarshal (TAO_InputCDR &);
#if TAO_HAS_INTERCEPTORS == 1
    virtual void interceptor_value (CORBA::Any *any) const;
#endif /* TAO_HAS_INTERCEPTORS == 1 */
    S *& arg (void);

  private:
    S *& x_;
  };

  /**
   * @class Ret_UB_String_Argument_T
   *
   * @brief Template class for return stub value of ub (w)string argument.
   *
   */
  template<typename S, typename S_var, typename Insert_Policy>
  class Ret_UB_String_Argument_T : public RetArgument, private Insert_Policy
  {
  public:
    Ret_UB_String_Argument_T (void);

    virtual CORBA::Boolean demarshal (TAO_InputCDR &);
#if TAO_HAS_INTERCEPTORS == 1
    virtual void interceptor_value (CORBA::Any *any) const;
#endif /* TAO_HAS_INTERCEPTORS == 1 */
    S *& arg (void);

    S * excp (void);
    S * retn (void);

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
  template<typename T, typename T_var, typename T_out, typename Insert_Policy>
  struct UB_String_Arg_Traits_T
  {
    typedef T *                                              ret_type;
    typedef T const *                                        in_type;
    typedef T *&                                             inout_type;
    typedef T_out                                            out_type;

    typedef In_UB_String_Argument_T<T, Insert_Policy>        in_arg_val;
    typedef Inout_UB_String_Argument_T<T, Insert_Policy>     inout_arg_val;
    typedef Out_UB_String_Argument_T<T,T_out, Insert_Policy> out_arg_val;
    typedef Ret_UB_String_Argument_T<T,T_var, Insert_Policy> ret_val;

    typedef UB_String_Tag                                    idl_tag;
  };
}

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "tao/UB_String_Argument_T.inl"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "tao/UB_String_Argument_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("UB_String_Argument_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include /**/ "ace/post.h"

#endif /* TAO_UB_STRING_ARGUMENT_T_H */
