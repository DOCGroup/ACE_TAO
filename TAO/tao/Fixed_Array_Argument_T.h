// -*- C++ -*-

//=============================================================================
/**
 *  @file    Fixed_Array_Argument_T.h
 *
 *  $Id$
 *
 *  @authors Jeff Parsons and Carlos O'Ryan
 */
//=============================================================================


#ifndef TAO_FIXED_ARRAY_ARGUMENT_T_H
#define TAO_FIXED_ARRAY_ARGUMENT_T_H

#include /**/ "ace/pre.h"
#include "tao/Argument.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

namespace TAO
{
  /**
   * @class In_Fixed_Array_Argument_T
   *
   * @brief IN stub argument of fixed size element array.
   *
   */
  template<typename S,
           typename S_slice,
           typename S_forany,
           typename Insert_Policy>
  class In_Fixed_Array_Argument_T : public InArgument, private Insert_Policy
  {
  public:
    In_Fixed_Array_Argument_T (const S_slice * x);

    virtual CORBA::Boolean marshal (TAO_OutputCDR &cdr);
#if TAO_HAS_INTERCEPTORS == 1
    virtual void interceptor_value (CORBA::Any *any) const;
#endif /* TAO_HAS_INTERCEPTORS == 1 */
    S_slice const * arg (void) const;

  private:
    S_forany x_;
  };

  /**
   * @class Inout_Fixed_Array_Argument_T
   *
   * @brief INOUT stub argument of fixed size element array.
   *
   */
  template<typename S,
           typename S_slice,
           typename S_forany,
           typename Insert_Policy>
  class Inout_Fixed_Array_Argument_T : public InoutArgument, private Insert_Policy
  {
  public:
    Inout_Fixed_Array_Argument_T (S_slice *&x);

    virtual CORBA::Boolean marshal (TAO_OutputCDR &cdr);
    virtual CORBA::Boolean demarshal (TAO_InputCDR &);
#if TAO_HAS_INTERCEPTORS == 1
    virtual void interceptor_value (CORBA::Any *any) const;
#endif /* TAO_HAS_INTERCEPTORS == 1 */
    S_slice * arg (void);

  private:
    S_forany x_;
  };

  /**
   * @class Out_Fixed_Array_Argument_T
   *
   * @brief OUT stub argument of fixed size element array.
   *
   */
  template<typename S,
           typename S_slice,
           typename S_forany,
           typename Insert_Policy>
  class Out_Fixed_Array_Argument_T : public OutArgument, private Insert_Policy
  {
  public:
    Out_Fixed_Array_Argument_T (S_slice *& x);

    virtual CORBA::Boolean demarshal (TAO_InputCDR &);
#if TAO_HAS_INTERCEPTORS == 1
    virtual void interceptor_value (CORBA::Any *any) const;
#endif /* TAO_HAS_INTERCEPTORS == 1 */
    S_slice *& arg (void);

  private:
    S_forany x_;
  };

  /**
   * @class Ret_Fixed_Array_Argument_T
   *
   * @brief Return stub value of fixed size element array.
   *
   */
  template<typename S,
           typename S_slice,
           typename S_var,
           typename S_forany,
           typename S_tag,
           typename Insert_Policy>
  class Ret_Fixed_Array_Argument_T : public RetArgument, private Insert_Policy
  {
  public:
    Ret_Fixed_Array_Argument_T (void);

    virtual CORBA::Boolean demarshal (TAO_InputCDR &);
#if TAO_HAS_INTERCEPTORS == 1
    virtual void interceptor_value (CORBA::Any *any) const;
#endif /* TAO_HAS_INTERCEPTORS == 1 */
    S_slice *& arg (void);

    S_slice * excp (void);
    S_slice * retn (void);

  private:
    S_var x_;
  };

  /**
   * @struct Fixed_Array_Tag
   *
   * @brief Struct for fixed size element array argument id tag.
   *
   */
  struct TAO_Export Fixed_Array_Tag {};

  /**
   * @struct Fixed_Array_Arg_Traits_T
   *
   * @brief Argument traits of fixed size element array.
   *
   */
  template<typename T,
           typename T_slice,
           typename T_var,
           typename T_forany,
           typename T_tag,
           typename Insert_Policy>
  struct Fixed_Array_Arg_Traits_T
  {
    typedef T_slice *                                   ret_type;
    typedef T const                                     in_type;
    typedef T                                           inout_type;
    typedef T                                           out_type;

    typedef In_Fixed_Array_Argument_T<T,
                                      T_slice,
                                      T_forany,
                                      Insert_Policy>         in_arg_val;
    typedef Inout_Fixed_Array_Argument_T<T,
                                         T_slice,
                                         T_forany,
                                         Insert_Policy>      inout_arg_val;
    typedef Out_Fixed_Array_Argument_T<T,
                                       T_slice,
                                       T_forany,
                                       Insert_Policy>        out_arg_val;
    typedef Ret_Fixed_Array_Argument_T<T,
                                       T_slice,
                                       T_var,
                                       T_forany,
                                       T_tag,
                                       Insert_Policy>        ret_val;

    typedef Fixed_Array_Tag                                  idl_tag;
  };
}

#if defined (__ACE_INLINE__)
#include "tao/Fixed_Array_Argument_T.inl"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "tao/Fixed_Array_Argument_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Fixed_Array_Argument_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include /**/ "ace/post.h"

#endif /* TAO_FIXED_ARRAY_ARGUMENT_T_H */
