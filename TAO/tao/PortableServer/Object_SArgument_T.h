// -*- C++ -*-

//=============================================================================
/**
 *  @file    Object_SArgument_T.h
 *
 *  @authors Jeff Parsons, Carlos O'Ryan and Ossama Othman
 */
//=============================================================================


#ifndef TAO_OBJECT_SARGUMENT_T_H
#define TAO_OBJECT_SARGUMENT_T_H

#include /**/ "ace/pre.h"

#include "tao/Argument.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

// This set of classes is also used by valuetype arguments. If the
// specialization is done using S * for the parameter, the semantics
// are the same as for interfaces, so there's no need for another
// set of classes.

namespace TAO
{
  /**
   * @class In_Object_SArgument_T
   *
   * @brief Template class for IN skeleton object argument.
   *
   */
  template<typename S_ptr,
           typename S_var,
           template <typename> class Insert_Policy>
  class In_Object_SArgument_T : public InArgument
  {
  public:
    CORBA::Boolean demarshal (TAO_InputCDR &) override;
#if TAO_HAS_INTERCEPTORS == 1
    void interceptor_value (CORBA::Any *any) const override;
#endif /* TAO_HAS_INTERCEPTORS == 1 */
    S_ptr arg () const;

  private:
    S_var x_ {};
  };

  /**
   * @class Inout_Object_SArgument_T
   *
   * @brief Template class for INOUT skeleton object argument.
   *
   */
  template<typename S_ptr,
           typename S_var,
           template <typename> class Insert_Policy>
  class Inout_Object_SArgument_T : public InoutArgument
  {
  public:
    Inout_Object_SArgument_T ();

    CORBA::Boolean marshal (TAO_OutputCDR &cdr) override;
    CORBA::Boolean demarshal (TAO_InputCDR &) override;
#if TAO_HAS_INTERCEPTORS == 1
    void interceptor_value (CORBA::Any *any) const override;
#endif /* TAO_HAS_INTERCEPTORS == 1 */
    S_ptr & arg ();

  private:
    S_var x_ {};
  };

  /**
   * @class Out_Object_SArgument_T
   *
   * @brief Template class for INOUT skeleton object argument.
   *
   */
  template<typename S_ptr,
           typename S_var,
           typename S_out,
           template <typename> class Insert_Policy>
  class Out_Object_SArgument_T : public OutArgument
  {
  public:
    Out_Object_SArgument_T ();

    CORBA::Boolean marshal (TAO_OutputCDR &cdr) override;
#if TAO_HAS_INTERCEPTORS == 1
    void interceptor_value (CORBA::Any *any) const override;
#endif /* TAO_HAS_INTERCEPTORS == 1 */
    S_out arg ();

  private:
    S_var x_ {};
  };

  /**
   * @class Ret_Object_SArgument_T
   *
   * @brief Template class for return skeleton value of object.
   *
   */
  template<typename S_ptr,
           typename S_var,
           template <typename> class Insert_Policy>
  class Ret_Object_SArgument_T : public RetArgument
  {
  public:
    Ret_Object_SArgument_T ();

    CORBA::Boolean marshal (TAO_OutputCDR &cdr) override;
#if TAO_HAS_INTERCEPTORS == 1
    void interceptor_value (CORBA::Any *any) const override;
#endif /* TAO_HAS_INTERCEPTORS == 1 */
    S_ptr & arg ();

  private:
    S_var x_ {};
  };

  /**
   * @struct Object_SArg_Traits_T
   *
   * @brief Template class for skeleton argument traits of objects.
   *
   */
  template<typename T_ptr,
           typename T_var,
           typename T_out,
           template <typename> class Insert_Policy>
  struct Object_SArg_Traits_T
  {
    using ret_type = T_ptr;
    using in_type = T_ptr;
    using inout_type = T_ptr &;
    using out_type = T_out;

    using in_arg_val = In_Object_SArgument_T<T_ptr, T_var, Insert_Policy>;
    using inout_arg_val = Inout_Object_SArgument_T<T_ptr, T_var, Insert_Policy>;
    using out_arg_val = Out_Object_SArgument_T<T_ptr, T_var, T_out, Insert_Policy>;
    using ret_val = Ret_Object_SArgument_T<T_ptr, T_var, Insert_Policy>;

    // Typedefs corresponding to return value of arg() method in both
    // the client and server side argument class templates.
    using in_arg_type = in_type;
    using inout_arg_type = inout_type;
    using out_arg_type = out_type;
    using ret_arg_type = inout_type;
  };
}

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "tao/PortableServer/Object_SArgument_T.inl"
#endif /* __ACE_INLINE__ */

#include "tao/PortableServer/Object_SArgument_T.cpp"

#include /**/ "ace/post.h"

#endif /* TAO_OBJECT_SARGUMENT_T_H */
