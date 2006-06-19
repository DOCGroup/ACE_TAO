// -*- C++ -*-

//=============================================================================
/**
 *  @file    Var_Size_SArgument_T.h
 *
 *  $Id$
 *
 *  @authors Jeff Parsons, Carlos O'Ryan and Ossama Othman
 */
//=============================================================================

#ifndef TAO_VAR_SIZE_SARGUMENT_T_H
#define TAO_VAR_SIZE_SARGUMENT_T_H

#include /**/ "ace/pre.h"
#include "tao/Argument.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  /**
   * @class In_Var_Size_SArgument_T
   *
   * @brief Template class for IN skeleton argument of fixed size IDL types.
   *
   */
  template<typename S, typename Insert_Policy>
  class In_Var_Size_SArgument_T : public InArgument, private Insert_Policy
  {
  public:

    virtual CORBA::Boolean demarshal (TAO_InputCDR &);
#if TAO_HAS_INTERCEPTORS == 1
    virtual void interceptor_value (CORBA::Any *any) const;
#endif /* TAO_HAS_INTERCEPTORS == 1 */
    S const & arg (void) const;

  private:
    S x_;
  };

  /**
   * @class Inout_Var_Size_SArgument_T
   *
   * @brief Template class for INOUT skeleton arg of fixed size IDL types.
   *
   */
  template<typename S, typename Insert_Policy>
  class Inout_Var_Size_SArgument_T : public InoutArgument,
                                     private Insert_Policy
  {
  public:
    Inout_Var_Size_SArgument_T (void);

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
   * @class Out_Var_Size_SArgument_T
   *
   * @brief Template class for OUT skeleton argument of fixed size IDL types.
   *
   */
  template<typename S, typename Insert_Policy>
  class Out_Var_Size_SArgument_T : public OutArgument, private Insert_Policy
  {
  public:
    Out_Var_Size_SArgument_T (void);

    virtual CORBA::Boolean marshal (TAO_OutputCDR &cdr);
#if TAO_HAS_INTERCEPTORS == 1
    virtual void interceptor_value (CORBA::Any *any) const;
#endif /* TAO_HAS_INTERCEPTORS == 1 */
    S *& arg (void);

  private:
    typename S::_var_type x_;
  };

  /**
   * @class Ret_Var_Size_SArgument_T
   *
   * @brief Template class for return skeleton value of fixed size IDL types.
   *
   */
  template<typename S, typename Insert_Policy>
  class Ret_Var_Size_SArgument_T : public RetArgument, private Insert_Policy
  {
  public:
    Ret_Var_Size_SArgument_T (void);

    virtual CORBA::Boolean marshal (TAO_OutputCDR &cdr);
#if TAO_HAS_INTERCEPTORS == 1
    virtual void interceptor_value (CORBA::Any *any) const;
#endif /* TAO_HAS_INTERCEPTORS == 1 */
    S *& arg (void);

  private:
    typename S::_var_type x_;
  };

  /**
   * @struct Var_Size_SArg_Traits_T
   *
   * @brief Template class for skeleton argument traits of
   *  variable size IDL types.
   *
   */
  template<typename T, typename Insert_Policy>
  struct Var_Size_SArg_Traits_T
  {
    typedef T *                                             ret_type;
    typedef const T &                                       in_type;
    typedef T &                                             inout_type;
    typedef typename T::_out_type                           out_type;

    typedef In_Var_Size_SArgument_T<T,Insert_Policy>        in_arg_val;
    typedef Inout_Var_Size_SArgument_T<T,Insert_Policy>     inout_arg_val;
    typedef Out_Var_Size_SArgument_T<T,Insert_Policy>       out_arg_val;
    typedef Ret_Var_Size_SArgument_T<T,Insert_Policy>       ret_val;

    // Typedefs corresponding to return value of arg() method in both
    // the client and server side argument class templates.
    typedef in_type                                         in_arg_type;
    typedef inout_type                                      inout_arg_type;
    typedef ret_type &                                      out_arg_type;
    typedef ret_type &                                      ret_arg_type;
  };
}

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "tao/PortableServer/Var_Size_SArgument_T.inl"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "tao/PortableServer/Var_Size_SArgument_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Var_Size_SArgument_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include /**/ "ace/post.h"

#endif /* TAO_VAR_SIZE_SARGUMENT_T_H */
