// -*- C++ -*-

//=============================================================================
/**
 *  @file    Var_Size_SArgument_T.h
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
  template<typename S,
           template <typename> class Insert_Policy>
  class In_Var_Size_SArgument_T : public InArgument
  {
  public:
    CORBA::Boolean demarshal (TAO_InputCDR &) override;
#if TAO_HAS_INTERCEPTORS == 1
    void interceptor_value (CORBA::Any *any) const override;
#endif /* TAO_HAS_INTERCEPTORS == 1 */
    S const & arg () const;

  private:
    S x_;
  };

  /**
   * @class Inout_Var_Size_SArgument_T
   *
   * @brief Template class for INOUT skeleton arg of fixed size IDL types.
   *
   */
  template<typename S,
           template <typename> class Insert_Policy>
  class Inout_Var_Size_SArgument_T : public InoutArgument
  {
  public:
    Inout_Var_Size_SArgument_T ();

    CORBA::Boolean marshal (TAO_OutputCDR &cdr) override;
    CORBA::Boolean demarshal (TAO_InputCDR &) override;
#if TAO_HAS_INTERCEPTORS == 1
    void interceptor_value (CORBA::Any *any) const override;
#endif /* TAO_HAS_INTERCEPTORS == 1 */
    S & arg ();

  private:
    S x_;
  };

  /**
   * @class Out_Var_Size_SArgument_T
   *
   * @brief Template class for OUT skeleton argument of fixed size IDL types.
   *
   */
  template<typename S,
           template <typename> class Insert_Policy>
  class Out_Var_Size_SArgument_T : public OutArgument
  {
  public:
    Out_Var_Size_SArgument_T ();

    CORBA::Boolean marshal (TAO_OutputCDR &cdr) override;
#if TAO_HAS_INTERCEPTORS == 1
    void interceptor_value (CORBA::Any *any) const override;
#endif /* TAO_HAS_INTERCEPTORS == 1 */
    S *& arg ();

  private:
    typename S::_var_type x_;
  };

  /**
   * @class Ret_Var_Size_SArgument_T
   *
   * @brief Template class for return skeleton value of fixed size IDL types.
   *
   */
  template<typename S,
           template <typename> class Insert_Policy>
  class Ret_Var_Size_SArgument_T : public RetArgument
  {
  public:
    Ret_Var_Size_SArgument_T ();

    CORBA::Boolean marshal (TAO_OutputCDR &cdr) override;
#if TAO_HAS_INTERCEPTORS == 1
    void interceptor_value (CORBA::Any *any) const override;
#endif /* TAO_HAS_INTERCEPTORS == 1 */
    S *& arg ();

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
  template<typename T,
           template <typename> class Insert_Policy>
  struct Var_Size_SArg_Traits_T
  {
    using ret_type = T *;
    using in_type = T const &;
    using inout_type = T &;
    using out_type = typename T::_out_type;

    using in_arg_val = In_Var_Size_SArgument_T<T, Insert_Policy>;
    using inout_arg_val = Inout_Var_Size_SArgument_T<T, Insert_Policy>;
    using out_arg_val = Out_Var_Size_SArgument_T<T, Insert_Policy>;
    using ret_val = Ret_Var_Size_SArgument_T<T, Insert_Policy>;

    // Typedefs corresponding to return value of arg() method in both
    // the client and server side argument class templates.
    using in_arg_type = in_type;
    using inout_arg_type = inout_type;
    using out_arg_type = ret_type &;
    using ret_arg_type = ret_type &;
  };
}

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "tao/PortableServer/Var_Size_SArgument_T.inl"
#endif /* __ACE_INLINE__ */

#include "tao/PortableServer/Var_Size_SArgument_T.cpp"

#include /**/ "ace/post.h"

#endif /* TAO_VAR_SIZE_SARGUMENT_T_H */
