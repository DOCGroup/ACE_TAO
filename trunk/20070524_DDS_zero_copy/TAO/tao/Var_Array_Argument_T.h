// -*- C++ -*-

//=============================================================================
/**
 *  @file    Var_Array_Argument_T.h
 *
 *  $Id$
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
           class Insert_Policy>
  class In_Var_Array_Argument_T : public InArgument
  {
  public:
    In_Var_Array_Argument_T (const typename S_forany::_slice_type * x);

    virtual CORBA::Boolean marshal (TAO_OutputCDR &cdr);
#if TAO_HAS_INTERCEPTORS == 1
    virtual void interceptor_value (CORBA::Any *any) const;
#endif /* TAO_HAS_INTERCEPTORS == 1 */
    typename S_forany::_slice_type const * arg (void) const;

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
           class Insert_Policy>
  class In_Var_Array_Clonable_Argument_T :
          public In_Var_Array_Argument_T<S_forany, Insert_Policy>
  {
  public:
    In_Var_Array_Clonable_Argument_T (const typename S_forany::_slice_type * x);
    virtual ~In_Var_Array_Clonable_Argument_T (void);

    virtual Argument* clone (void);

  private:
    bool is_clone_;
  };

  /**
   * @class Inout_Var_Array_Argument_T
   *
   * @brief INOUT stub argument of variable size element array.
   *
   */
  template<typename S_forany,
           class Insert_Policy>
  class Inout_Var_Array_Argument_T : public InoutArgument
  {
  public:
    Inout_Var_Array_Argument_T (typename S_forany::_slice_type *&x);

    virtual CORBA::Boolean marshal (TAO_OutputCDR &cdr);
    virtual CORBA::Boolean demarshal (TAO_InputCDR &);
#if TAO_HAS_INTERCEPTORS == 1
    virtual void interceptor_value (CORBA::Any *any) const;
#endif /* TAO_HAS_INTERCEPTORS == 1 */
    typename S_forany::_slice_type * arg (void);

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
           class Insert_Policy>
  class Out_Var_Array_Argument_T : public OutArgument
  {
  public:
    Out_Var_Array_Argument_T (S_out x);

    virtual CORBA::Boolean demarshal (TAO_InputCDR &);
#if TAO_HAS_INTERCEPTORS == 1
    virtual void interceptor_value (CORBA::Any *any) const;
#endif /* TAO_HAS_INTERCEPTORS == 1 */
    typename S_forany::_slice_type *& arg (void);

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
           class Insert_Policy>
  class Ret_Var_Array_Argument_T : public RetArgument
  {
  public:
    Ret_Var_Array_Argument_T (void);

    virtual CORBA::Boolean demarshal (TAO_InputCDR &);
#if TAO_HAS_INTERCEPTORS == 1
    virtual void interceptor_value (CORBA::Any *any) const;
#endif /* TAO_HAS_INTERCEPTORS == 1 */
    typename S_forany::_slice_type *& arg (void);

    typename S_forany::_slice_type * excp (void);
    typename S_forany::_slice_type * retn (void);

  private:
    S_var x_;
  };

  /**
   * @struct Var_Array_Arg_Traits_T
   *
   * @brief Argument traits of variable size element array.
   *
   */
  template<typename T_out,
           typename T_forany,
           class Insert_Policy>
  struct Var_Array_Arg_Traits_T
  {
    typedef typename T_forany::_slice_type *            ret_type;
    typedef const typename T_forany::_array_type        in_type;
    typedef typename T_forany::_array_type              inout_type;
    typedef T_out                                       out_type;

    typedef In_Var_Array_Argument_T<T_forany,
                                    Insert_Policy>      in_arg_val;
    typedef In_Var_Array_Clonable_Argument_T<T_forany,
                                             Insert_Policy>
                                                        in_clonable_arg_val;
    typedef Inout_Var_Array_Argument_T<T_forany,
                                       Insert_Policy>   inout_arg_val;
    typedef Out_Var_Array_Argument_T<T_out,
                                     T_forany,
                                     Insert_Policy>     out_arg_val;
    typedef Ret_Var_Array_Argument_T<typename T_out::_var_type,
                                     T_forany,
                                     Insert_Policy>     ret_val;
  };
}

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "tao/Var_Array_Argument_T.inl"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "tao/Var_Array_Argument_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Var_Array_Argument_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include /**/ "ace/post.h"

#endif /* TAO_VAR_ARRAY_ARGUMENT_T_H */
