// -*- C++ -*-

//=============================================================================
/**
 *  @file    Object_Argument_T.h
 *
 *  $Id$
 *
 *  @authors Jeff Parsons and Carlos O'Ryan
 */
//=============================================================================


#ifndef TAO_OBJECT_ARGUMENT_T_H
#define TAO_OBJECT_ARGUMENT_T_H

#include /**/ "ace/pre.h"

#include "tao/Argument.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  /**
   * @class In_Object_Argument_T
   *
   * @brief Template class for IN object argument.
   *
   */
  template<typename S_ptr, typename Insert_Policy>
  class In_Object_Argument_T : public InArgument, private Insert_Policy
  {
  public:
    In_Object_Argument_T (S_ptr x);

    virtual CORBA::Boolean marshal (TAO_OutputCDR &cdr);
#if TAO_HAS_INTERCEPTORS == 1
    virtual void interceptor_value (CORBA::Any *any) const;
#endif /* TAO_HAS_INTERCEPTORS == 1 */
    S_ptr arg (void) const;

  private:
    S_ptr x_;
  };

  /**
   * @class Inout_Object_Argument_T
   *
   * @brief Template class for INOUT object argument.
   *
   */
  template<typename S_ptr, typename S_traits, typename Insert_Policy>
  class Inout_Object_Argument_T : public InoutArgument, private Insert_Policy
  {
  public:
    Inout_Object_Argument_T (S_ptr & x);

    virtual CORBA::Boolean marshal (TAO_OutputCDR &cdr);
    virtual CORBA::Boolean demarshal (TAO_InputCDR &cdr);
#if TAO_HAS_INTERCEPTORS == 1
    virtual void interceptor_value (CORBA::Any *any) const;
#endif /* TAO_HAS_INTERCEPTORS == 1 */
    S_ptr & arg (void);

  private:
    S_ptr & x_;
  };

  /**
   * @class Out_Object_Argument_T
   *
   * @brief Template class for OUT object argument.
   *
   */
  template<typename S_ptr, typename S_out, typename Insert_Policy>
  class Out_Object_Argument_T : public OutArgument, private Insert_Policy
  {
  public:
    Out_Object_Argument_T (S_out & x);

    virtual CORBA::Boolean demarshal (TAO_InputCDR &cdr);
#if TAO_HAS_INTERCEPTORS == 1
    virtual void interceptor_value (CORBA::Any *any) const;
#endif /* TAO_HAS_INTERCEPTORS == 1 */
    S_out arg (void);

  private:
    S_ptr & x_;
  };

  /**
   * @class Ret_Object_Argument_T
   *
   * @brief Template class for return stub value of object argument.
   *
   */
  template<typename S_ptr, typename S_var, typename Insert_Policy>
  class Ret_Object_Argument_T : public RetArgument, private Insert_Policy
  {
  public:

    Ret_Object_Argument_T (void);

    virtual CORBA::Boolean demarshal (TAO_InputCDR &);
#if TAO_HAS_INTERCEPTORS == 1
    virtual void interceptor_value (CORBA::Any *any) const;
#endif /* TAO_HAS_INTERCEPTORS == 1 */
    S_ptr & arg (void);

    S_ptr excp (void);
    S_ptr retn (void);

  private:
    S_var x_;
  };

  /**
   * @struct Object_Tag
   *
   * @brief Struct for object arguments id tag.
   *
   */
  struct TAO_Export Object_Tag {};

  /**
   * @struct Object_Arg_Traits_T
   *
   * @brief Template class for stub argument traits of objects.
   */
  template<typename T_ptr, typename T_var, typename T_out, typename T_traits, typename Insert_Policy>
  struct Object_Arg_Traits_T
  {
    typedef T_ptr                                         ret_type;
    typedef T_ptr                                         in_type;
    typedef T_ptr &                                       inout_type;
    typedef T_out                                         out_type;

    typedef In_Object_Argument_T<T_ptr, Insert_Policy>
      in_arg_val;
    typedef Inout_Object_Argument_T<T_ptr, T_traits, Insert_Policy>
      inout_arg_val;
    typedef Out_Object_Argument_T<T_ptr,T_out, Insert_Policy>
      out_arg_val;
    typedef Ret_Object_Argument_T<T_ptr,T_var, Insert_Policy>
      ret_val;

    typedef Object_Tag                                    idl_tag;
  };
}

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "tao/Object_Argument_T.inl"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "tao/Object_Argument_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Object_Argument_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include /**/ "ace/post.h"

#endif /* TAO_OBJECT_ARGUMENT_T_H */
