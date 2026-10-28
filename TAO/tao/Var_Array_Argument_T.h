// -*- C++ -*-

//=============================================================================
/**
 *  @file    Var_Array_Argument_T.h
 *
 *  @authors Jeff Parsons and Carlos O'Ryan
 */
//=============================================================================

#ifndef TAO_VAR_ARRAY_ARGUMENT_T_H
#define TAO_VAR_ARRAY_ARGUMENT_T_H

#include /**/ "ace/pre.h"

#include "tao/Argument.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  /**
   * @class In_Var_Array_Argument_T
   *
   * @brief IN stub argument of variable size element array.
   *
   */
  template<typename S_forany,
           template <typename> class Insert_Policy>
  class In_Var_Array_Argument_T : public InArgument
  {
  public:
    In_Var_Array_Argument_T (typename S_forany::_slice_type const * x);

    CORBA::Boolean marshal (TAO_OutputCDR &cdr) override;
#if TAO_HAS_INTERCEPTORS == 1
    void interceptor_value (CORBA::Any *any) const override;
#endif /* TAO_HAS_INTERCEPTORS == 1 */
    typename S_forany::_slice_type const * arg () const;

  protected:
    S_forany x_;
  };

  /**
   * @class In_Var_Array_Clonable_Argument_T
   *
   * @brief IN stub argument of variable size element array.
   *
   */
  template<typename S_forany,
           template <typename> class Insert_Policy>
  class In_Var_Array_Clonable_Argument_T :
          public In_Var_Array_Argument_T<S_forany, Insert_Policy>
  {
  public:
    In_Var_Array_Clonable_Argument_T (typename S_forany::_slice_type const * x);
    ~In_Var_Array_Clonable_Argument_T () override;

    Argument* clone () override;

  private:
    bool is_clone_ {};
  };

  /**
   * @class Inout_Var_Array_Argument_T
   *
   * @brief INOUT stub argument of variable size element array.
   *
   */
  template<typename S_forany,
           template <typename> class Insert_Policy>
  class Inout_Var_Array_Argument_T : public InoutArgument
  {
  public:
    Inout_Var_Array_Argument_T (typename S_forany::_slice_type *&x);

    CORBA::Boolean marshal (TAO_OutputCDR &cdr) override;
    CORBA::Boolean demarshal (TAO_InputCDR &) override;
#if TAO_HAS_INTERCEPTORS == 1
    void interceptor_value (CORBA::Any *any) const override;
#endif /* TAO_HAS_INTERCEPTORS == 1 */
    typename S_forany::_slice_type * arg ();

  private:
    S_forany x_;
  };

  /**
   * @class Out_Var_Array_Argument_T
   *
   * @brief OUT stub argument of variable size element array.
   *
   */
  template<typename S_out,
           typename S_forany,
           template <typename> class Insert_Policy>
  class Out_Var_Array_Argument_T : public OutArgument
  {
  public:
    Out_Var_Array_Argument_T (S_out x);

    CORBA::Boolean demarshal (TAO_InputCDR &) override;
#if TAO_HAS_INTERCEPTORS == 1
    void interceptor_value (CORBA::Any *any) const override;
#endif /* TAO_HAS_INTERCEPTORS == 1 */
    typename S_forany::_slice_type *& arg ();

  private:
    typename S_forany::_slice_type *& x_;
  };

  /**
   * @class Ret_Var_Array_Argument_T
   *
   * @brief Return stub value of variable size element array.
   *
   */
  template<typename S_var,
           typename S_forany,
           template <typename> class Insert_Policy>
  class Ret_Var_Array_Argument_T : public RetArgument
  {
  public:
    Ret_Var_Array_Argument_T ();

    CORBA::Boolean demarshal (TAO_InputCDR &) override;
#if TAO_HAS_INTERCEPTORS == 1
    void interceptor_value (CORBA::Any *any) const override;
#endif /* TAO_HAS_INTERCEPTORS == 1 */
    typename S_forany::_slice_type *& arg ();

    typename S_forany::_slice_type * excp ();
    typename S_forany::_slice_type * retn ();

  private:
    S_var x_ {};
  };

  /**
   * @struct Var_Array_Arg_Traits_T
   *
   * @brief Argument traits of variable size element array.
   *
   */
  template<typename T_out,
           typename T_forany,
           template <typename> class Insert_Policy>
  struct Var_Array_Arg_Traits_T
  {
    using ret_type = typename T_forany::_slice_type *;
    using in_type = typename T_forany::_array_type const;
    using inout_type = typename T_forany::_array_type;
    using out_type = T_out;

    using in_arg_val = In_Var_Array_Argument_T<T_forany, Insert_Policy>;
    using in_clonable_arg_val = In_Var_Array_Clonable_Argument_T<T_forany, Insert_Policy>;
    using inout_arg_val = Inout_Var_Array_Argument_T<T_forany, Insert_Policy>;
    using out_arg_val = Out_Var_Array_Argument_T<T_out, T_forany, Insert_Policy>;
    using ret_val = Ret_Var_Array_Argument_T<typename T_out::_var_type, T_forany, Insert_Policy>;
  };
}

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "tao/Var_Array_Argument_T.inl"
#endif /* __ACE_INLINE__ */

#include "tao/Var_Array_Argument_T.cpp"

#include /**/ "ace/post.h"

#endif /* TAO_VAR_ARRAY_ARGUMENT_T_H */
