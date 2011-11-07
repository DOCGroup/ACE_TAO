// -*- C++ -*-

//=============================================================================
/**
 *  @file    Object_SArgument_T.h
 *
 *  $Id$
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

    virtual CORBA::Boolean demarshal (TAO_InputCDR &);
#if TAO_HAS_INTERCEPTORS == 1
    virtual void interceptor_value (CORBA::Any *any) const;
#endif /* TAO_HAS_INTERCEPTORS == 1 */
    S_ptr arg (void) const;

  private:
    S_var x_;
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
    Inout_Object_SArgument_T (void);

    virtual CORBA::Boolean marshal (TAO_OutputCDR &cdr);
    virtual CORBA::Boolean demarshal (TAO_InputCDR &);
#if TAO_HAS_INTERCEPTORS == 1
    virtual void interceptor_value (CORBA::Any *any) const;
#endif /* TAO_HAS_INTERCEPTORS == 1 */
    S_ptr & arg (void);

  private:
    S_var x_;
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
    Out_Object_SArgument_T (void);

    virtual CORBA::Boolean marshal (TAO_OutputCDR &cdr);
#if TAO_HAS_INTERCEPTORS == 1
    virtual void interceptor_value (CORBA::Any *any) const;
#endif /* TAO_HAS_INTERCEPTORS == 1 */
    S_out arg (void);

  private:
    S_var x_;
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
    Ret_Object_SArgument_T (void);

    virtual CORBA::Boolean marshal (TAO_OutputCDR &cdr);
#if TAO_HAS_INTERCEPTORS == 1
    virtual void interceptor_value (CORBA::Any *any) const;
#endif /* TAO_HAS_INTERCEPTORS == 1 */
    S_ptr & arg (void);

  private:
    S_var x_;
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
    typedef T_ptr                                         ret_type;
    typedef T_ptr                                         in_type;
    typedef T_ptr &                                       inout_type;
    typedef T_out                                         out_type;

    typedef In_Object_SArgument_T<T_ptr,
                                  T_var,
                                  Insert_Policy>          in_arg_val;
    typedef Inout_Object_SArgument_T<T_ptr,
                                     T_var,
                                     Insert_Policy>       inout_arg_val;
    typedef Out_Object_SArgument_T<T_ptr,
                                   T_var,
                                   T_out,
                                   Insert_Policy>         out_arg_val;
    typedef Ret_Object_SArgument_T<T_ptr,
                                   T_var,
                                   Insert_Policy>         ret_val;

    // Typedefs corresponding to return value of arg() method in both
    // the client and server side argument class templates.
    typedef in_type                                       in_arg_type;
    typedef inout_type                                    inout_arg_type;
    typedef out_type                                      out_arg_type;
    typedef inout_type                                    ret_arg_type;
  };
}

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "tao/PortableServer/Object_SArgument_T.inl"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "tao/PortableServer/Object_SArgument_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Object_SArgument_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include /**/ "ace/post.h"

#endif /* TAO_OBJECT_SARGUMENT_T_H */
