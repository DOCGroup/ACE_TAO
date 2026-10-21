// -*- C++ -*-

//=============================================================================
/**
 *  @file    Var_Size_Argument_T.h
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

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  /**
   * @class In_Var_Size_Argument_T
   *
   * @brief Template class for IN stub argument of fixed size IDL types.
   *
   */
  template<typename S,
           template <typename> class Insert_Policy>
  class In_Var_Size_Argument_T : public InArgument
  {
  public:
    In_Var_Size_Argument_T (S const & x);

    CORBA::Boolean marshal (TAO_OutputCDR &cdr) override;
#if TAO_HAS_INTERCEPTORS == 1
    void interceptor_value (CORBA::Any *any) const override;
#endif /* TAO_HAS_INTERCEPTORS == 1 */
    S const & arg () const;

  protected:
    S const * x_;
#if TAO_HAS_INTERCEPTORS == 1
    Insert_Policy<S> insert_policy_;
#endif /* TAO_HAS_INTERCEPTORS == 1 */
  };

  /**
   * @class In_Var_Size_Clonable_Argument_T
   *
   * @brief Template class for IN stub argument of fixed size IDL types.
   *
   */
  template<typename S,
           template <typename> class Insert_Policy>
  class In_Var_Size_Clonable_Argument_T :
          public In_Var_Size_Argument_T<S, Insert_Policy>
  {
  public:
    In_Var_Size_Clonable_Argument_T (S const & x);
    ~In_Var_Size_Clonable_Argument_T () override;

    Argument* clone () override;

  private:
    bool is_clone_ {};
  };

  /**
   * @class Inout_Var_Size_Argument_T
   *
   * @brief Template class for INOUT stub argument of fixed size IDL types.
   *
   */
  template<typename S,
           template <typename> class Insert_Policy>
  class Inout_Var_Size_Argument_T : public InoutArgument
  {
  public:
    Inout_Var_Size_Argument_T (S & x);

    CORBA::Boolean marshal (TAO_OutputCDR &cdr) override;
    CORBA::Boolean demarshal (TAO_InputCDR &) override;
#if TAO_HAS_INTERCEPTORS == 1
    void interceptor_value (CORBA::Any *any) const override;
#endif /* TAO_HAS_INTERCEPTORS == 1 */
    S & arg ();

  private:
    S * x_;
  };

  /**
   * @class Out_Var_Size_Argument_T
   *
   * @brief Template class for OUT stub argument of fixed size IDL types.
   *
   */
  template<typename S,
           template <typename> class Insert_Policy>
  class Out_Var_Size_Argument_T : public OutArgument
  {
  public:
    Out_Var_Size_Argument_T (typename S::_out_type x);

    CORBA::Boolean demarshal (TAO_InputCDR &) override;
#if TAO_HAS_INTERCEPTORS == 1
    void interceptor_value (CORBA::Any *any) const override;
#endif /* TAO_HAS_INTERCEPTORS == 1 */
    S *& arg ();

  private:
    S *& x_;
  };

  /**
   * @class Ret_Var_Size_Argument_T
   *
   * @brief Template class for return stub value of fixed size IDL types.
   *
   */
  template<typename S,
           template <typename> class Insert_Policy>
  class Ret_Var_Size_Argument_T : public RetArgument
  {
  public:
    Ret_Var_Size_Argument_T ();

    CORBA::Boolean demarshal (TAO_InputCDR &) override;
#if TAO_HAS_INTERCEPTORS == 1
    void interceptor_value (CORBA::Any *any) const override;
#endif /* TAO_HAS_INTERCEPTORS == 1 */
    S *& arg ();

    S * excp ();
    S * retn ();

  protected:
    typename S::_var_type x_ {};
  };

  /**
   * @struct Var_Size_Arg_Traits_T
   *
   * @brief Template class for stub argument traits of
   *  variable size IDL types.
   *
   */
  template<typename T,
           template <typename> class Insert_Policy>
  struct Var_Size_Arg_Traits_T
  {
    using ret_type = T *;
    using in_type = T const &;
    using inout_type = T &;
    using out_type = typename T::_out_type;

    using in_arg_val = In_Var_Size_Argument_T<T, Insert_Policy>;
    using in_clonable_arg_val = In_Var_Size_Clonable_Argument_T<T, Insert_Policy>;
    using inout_arg_val = Inout_Var_Size_Argument_T<T, Insert_Policy>;
    using out_arg_val = Out_Var_Size_Argument_T<T, Insert_Policy>;
    using ret_val = Ret_Var_Size_Argument_T<T, Insert_Policy>;
  };
}

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "tao/Var_Size_Argument_T.inl"
#endif /* __ACE_INLINE__ */

#include "tao/Var_Size_Argument_T.cpp"

#include /**/ "ace/post.h"

#endif /* TAO_VAR_SIZE_ARGUMENT_T_H */
