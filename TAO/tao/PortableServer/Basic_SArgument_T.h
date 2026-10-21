// -*- C++ -*-

//=============================================================================
/**
 *  @file    Basic_SArgument_T.h
 *
 *  @authors Jeff Parsons, Carlos O'Ryan and Ossama Othman
 */
//=============================================================================


#ifndef TAO_BASIC_SARGUMENT_T_H
#define TAO_BASIC_SARGUMENT_T_H

#include /**/ "ace/pre.h"

#include "tao/Argument.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  /**
   * @class In_Basic_SArgument_T
   *
   * @brief Template class for IN skeleton argument of basic IDL types.
   *
   */
  template<typename S, template <typename> class Insert_Policy>
  class In_Basic_SArgument_T : public InArgument
  {
  public:
    In_Basic_SArgument_T ();

    CORBA::Boolean demarshal (TAO_InputCDR &) override;
#if TAO_HAS_INTERCEPTORS == 1
    void interceptor_value (CORBA::Any *any) const override;
#endif /* TAO_HAS_INTERCEPTORS == 1 */
    S arg () const;

  private:
    S x_ {};
  };

  /**
   * @class Inout_Basic_SArgument_T
   *
   * @brief Template class for INOUT skeleton argument of basic IDL types.
   *
   */
  template<typename S, template <typename> class Insert_Policy>
  class Inout_Basic_SArgument_T : public InoutArgument
  {
  public:
    Inout_Basic_SArgument_T ();

    CORBA::Boolean marshal (TAO_OutputCDR &cdr) override;
    CORBA::Boolean demarshal (TAO_InputCDR &) override;
#if TAO_HAS_INTERCEPTORS == 1
    void interceptor_value (CORBA::Any *any) const override;
#endif /* TAO_HAS_INTERCEPTORS == 1 */
    S & arg ();

  private:
    S x_ {};
  };

  /**
   * @class Out_Basic_SArgument_T
   *
   * @brief Template class for OUT skeleton argument of basic IDL types.
   *
   */
  template<typename S, template <typename> class Insert_Policy>
  class Out_Basic_SArgument_T : public OutArgument
  {
  public:
    Out_Basic_SArgument_T ();

    CORBA::Boolean marshal (TAO_OutputCDR &cdr) override;
#if TAO_HAS_INTERCEPTORS == 1
    void interceptor_value (CORBA::Any *any) const override;
#endif /* TAO_HAS_INTERCEPTORS == 1 */
    S & arg ();

  private:
    S x_ {};
  };

  /**
   * @class Ret_Basic_SArgument_T
   *
   * @brief Template class for return skeleton value of basic IDL types.
   *
   */
  template<typename S, template <typename> class Insert_Policy>
  class Ret_Basic_SArgument_T : public RetArgument
  {
  public:
    Ret_Basic_SArgument_T ();

    CORBA::Boolean marshal (TAO_OutputCDR &cdr) override;
#if TAO_HAS_INTERCEPTORS == 1
    void interceptor_value (CORBA::Any *any) const override;
#endif /* TAO_HAS_INTERCEPTORS == 1 */
    S & arg ();

  private:
    S x_ {};
  };

  /**
   * @struct Basic_SArg_Traits_T
   *
   * @brief Template class for skeleton argument traits of basic IDL types.
   *
   */
  template<typename T, template <typename> class Insert_Policy>
  struct Basic_SArg_Traits_T
  {
    using ret_type = T;
    using in_type = T;
    using inout_type = T &;
    using out_type = T &;

    using in_arg_val = In_Basic_SArgument_T<T, Insert_Policy>;
    using inout_arg_val = Inout_Basic_SArgument_T<T, Insert_Policy>;
    using out_arg_val = Out_Basic_SArgument_T<T, Insert_Policy>;
    using ret_val = Ret_Basic_SArgument_T<T, Insert_Policy>;

    // Typedefs corresponding to return value of arg() method in both
    // the client and server side argument class templates.
    using in_arg_type = in_type;
    using inout_arg_type = inout_type;
    using out_arg_type = out_type;
    using ret_arg_type = out_type;
  };
}

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "tao/PortableServer/Basic_SArgument_T.inl"
#endif /* __ACE_INLINE__ */

#include "tao/PortableServer/Basic_SArgument_T.cpp"

#include /**/ "ace/post.h"

#endif /* TAO_BASIC_SARGUMENT_T_H */
