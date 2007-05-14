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
  template<typename S_ptr,
           class Insert_Policy>
  class In_Object_Argument_T : public InArgument
  {
  public:
    In_Object_Argument_T (S_ptr x);

    virtual CORBA::Boolean marshal (TAO_OutputCDR &cdr);
#if TAO_HAS_INTERCEPTORS == 1
    virtual void interceptor_value (CORBA::Any *any) const;
#endif /* TAO_HAS_INTERCEPTORS == 1 */
    S_ptr arg (void) const;

  protected:
    S_ptr x_;
  };

  template<typename S_ptr>
  struct In_Object_Argument_Cloner_T
  {
    static void duplicate(S_ptr objref);
    static void release(S_ptr objref);
  };

  /**
   * @class In_Object_Clonable_Argument_T
   *
   * @brief Template class for IN object argument.
   *
   */
  template<typename S_ptr,
           class Insert_Policy>
  class In_Object_Clonable_Argument_T :
          public In_Object_Argument_T<S_ptr, Insert_Policy>
  {
  public:
    In_Object_Clonable_Argument_T (S_ptr x);
    virtual ~In_Object_Clonable_Argument_T (void);

    virtual Argument* clone (void);

  private:
    bool is_clone_;
  };

  /**
   * @class Inout_Object_Argument_T
   *
   * @brief Template class for INOUT object argument.
   *
   */
  template<typename S_ptr,
           typename S_traits,
           class Insert_Policy>
  class Inout_Object_Argument_T : public InoutArgument
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
  template<typename S_ptr,
           typename S_out,
           class Insert_Policy>
  class Out_Object_Argument_T : public OutArgument
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
  template<typename S_ptr,
           typename S_var,
           class Insert_Policy>
  class Ret_Object_Argument_T : public RetArgument
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
   * @struct Object_Arg_Traits_T
   *
   * @brief Template class for stub argument traits of objects.
   */
  template<typename T_ptr,
           typename T_var,
           typename T_out,
           typename T_traits,
           class Insert_Policy>
  struct Object_Arg_Traits_T
  {
    typedef T_ptr                                         ret_type;
    typedef T_ptr                                         in_type;
    typedef T_ptr &                                       inout_type;
    typedef T_out                                         out_type;

    typedef In_Object_Argument_T<T_ptr,
                                 Insert_Policy>           in_arg_val;
    typedef In_Object_Clonable_Argument_T<T_ptr,
                                          Insert_Policy>  in_clonable_arg_val;
    typedef Inout_Object_Argument_T<T_ptr,
                                    T_traits,
                                    Insert_Policy>        inout_arg_val;
    typedef Out_Object_Argument_T<T_ptr,
                                  T_out,
                                  Insert_Policy>          out_arg_val;
    typedef Ret_Object_Argument_T<T_ptr,
                                  T_var,
                                  Insert_Policy>          ret_val;
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
