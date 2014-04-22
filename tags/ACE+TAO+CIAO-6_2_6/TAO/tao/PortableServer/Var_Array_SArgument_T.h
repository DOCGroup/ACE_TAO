// -*- C++ -*-

//=============================================================================
/**
 *  @file    Var_Array_SArgument_T.h
 *
 *  $Id$
 *
 *  @authors Jeff Parsons, Carlos O'Ryan and Ossama Othman
 */
//=============================================================================

#ifndef TAO_VAR_ARRAY_SARGUMENT_T_H
#define TAO_VAR_ARRAY_SARGUMENT_T_H

#include /**/ "ace/pre.h"

#include "tao/Argument.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  /**
   * @class In_Var_Array_SArgument_T
   *
   * @brief IN skeleton argument of variable size element array.
   *
   */
  template<typename S_forany,
           template <typename> class Insert_Policy>
  class In_Var_Array_SArgument_T : public InArgument
  {
  public:

    In_Var_Array_SArgument_T (void);

    virtual CORBA::Boolean demarshal (TAO_InputCDR &);
#if TAO_HAS_INTERCEPTORS == 1
    virtual void interceptor_value (CORBA::Any *any) const;
#endif /* TAO_HAS_INTERCEPTORS == 1 */
    typename S_forany::_slice_type const * arg (void) const;

  private:
    mutable typename S_forany::_array_type x_;
  };

  /**
   * @class Inout_Var_Array_SArgument_T
   *
   * @brief INOUT skeleton argument of variable size element array.
   *
   */
  template<typename S_forany,
           template <typename> class Insert_Policy>
  class Inout_Var_Array_SArgument_T : public InoutArgument
  {
  public:
    Inout_Var_Array_SArgument_T (void);

    virtual CORBA::Boolean marshal (TAO_OutputCDR &cdr);
    virtual CORBA::Boolean demarshal (TAO_InputCDR &);
#if TAO_HAS_INTERCEPTORS == 1
    virtual void interceptor_value (CORBA::Any *any) const;
#endif /* TAO_HAS_INTERCEPTORS == 1 */
    typename S_forany::_slice_type * arg (void);

  private:
    typename S_forany::_array_type x_;
  };

  /**
   * @class Out_Var_Array_SArgument_T
   *
   * @brief OUT skeleton argument of variable size element array.
   *
   */
  template<typename S_var,
           typename S_forany,
           template <typename> class Insert_Policy>
  class Out_Var_Array_SArgument_T : public OutArgument
  {
  public:
    Out_Var_Array_SArgument_T (void);

    virtual CORBA::Boolean marshal (TAO_OutputCDR &cdr);
#if TAO_HAS_INTERCEPTORS == 1
    virtual void interceptor_value (CORBA::Any *any) const;
#endif /* TAO_HAS_INTERCEPTORS == 1 */
    typename S_forany::_slice_type *& arg (void);

  private:
    S_var x_;
  };

  /**
   * @class Ret_Var_Array_SArgument_T
   *
   * @brief Skeleton value of variable size element array.
   *
   */
  template<typename S_var,
           typename S_forany,
           template <typename> class Insert_Policy>
  class Ret_Var_Array_SArgument_T : public RetArgument
  {
  public:
    Ret_Var_Array_SArgument_T (void);

    virtual CORBA::Boolean marshal (TAO_OutputCDR &cdr);
#if TAO_HAS_INTERCEPTORS == 1
    virtual void interceptor_value (CORBA::Any *any) const;
#endif /* TAO_HAS_INTERCEPTORS == 1 */
    typename S_forany::_slice_type *& arg (void);

  private:
    S_var x_;
  };

  /**
   * @struct Var_Array_SArg_Traits_T
   *
   * @brief Argument traits of variable size element array.
   *
   */
  template<typename T_out,
           typename T_forany,
           template <typename> class Insert_Policy>
  struct Var_Array_SArg_Traits_T
  {
    typedef typename T_forany::_slice_type *            ret_type;
    typedef const typename T_forany::_array_type        in_type;
    typedef typename T_forany::_array_type              inout_type;
    typedef T_out                                       out_type;

    typedef In_Var_Array_SArgument_T<T_forany,
                                     Insert_Policy>     in_arg_val;
    typedef Inout_Var_Array_SArgument_T<T_forany,
                                        Insert_Policy>  inout_arg_val;
    typedef Out_Var_Array_SArgument_T<typename T_out::_var_type,
                                      T_forany,
                                      Insert_Policy>    out_arg_val;
    typedef Ret_Var_Array_SArgument_T<typename T_out::_var_type,
                                      T_forany,
                                      Insert_Policy>    ret_val;

    // Typedefs corresponding to return value of arg() method in both
    // the client and server side argument class templates.
    typedef typename T_forany::_slice_type const *      in_arg_type;
    typedef ret_type                                    inout_arg_type;
    typedef ret_type &                                  out_arg_type;
    typedef ret_type &                                  ret_arg_type;
  };
}

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "tao/PortableServer/Var_Array_SArgument_T.inl"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "tao/PortableServer/Var_Array_SArgument_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Var_Array_SArgument_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include /**/ "ace/post.h"

#endif /* TAO_VAR_ARRAY_SARGUMENT_T_H */
