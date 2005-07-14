// -*- C++ -*-

//=============================================================================
/**
 *  @file    Fixed_Array_SArgument_T.h
 *
 *  $Id$
 *
 *  @authors Jeff Parsons, Carlos O'Ryan and Ossama Othman
 */
//=============================================================================


#ifndef TAO_FIXED_ARRAY_SARGUMENT_T_H
#define TAO_FIXED_ARRAY_SARGUMENT_T_H

#include /**/ "ace/pre.h"

#include "tao/Argument.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

namespace TAO
{
  /**
   * @class In_Fixed_Array_SArgument_T
   *
   * @brief IN skeleton argument of fixed size element array.
   *
   */
  template<typename S, typename S_slice, typename S_forany>
  class In_Fixed_Array_SArgument_T : public Argument
  {
  public:
    In_Fixed_Array_SArgument_T (void);

    virtual CORBA::Boolean demarshal (TAO_InputCDR &);
#if TAO_HAS_INTERCEPTORS == 1
    virtual void interceptor_param (Dynamic::Parameter &);
#endif /* TAO_HAS_INTERCEPTORS == 1 */
    S_slice const * arg (void) const;

  private:
    mutable S x_;
  };

  /**
   * @class Inout_Fixed_Array_SArgument_T
   *
   * @brief INOUT skeleton argument of fixed size element array.
   *
   */
  template<typename S, typename S_slice, typename S_forany>
  class Inout_Fixed_Array_SArgument_T : public Argument
  {
  public:
    Inout_Fixed_Array_SArgument_T (void);

    virtual CORBA::Boolean marshal (TAO_OutputCDR &);
    virtual CORBA::Boolean demarshal (TAO_InputCDR &);
#if TAO_HAS_INTERCEPTORS == 1
    virtual void interceptor_param (Dynamic::Parameter &);
#endif /* TAO_HAS_INTERCEPTORS == 1 */
    S_slice * arg (void);

  private:
    S x_;
  };

  /**
   * @class Out_Fixed_Array_SArgument_T
   *
   * @brief OUT skeleton argument of fixed size element array.
   *
   */
  template<typename S, typename S_slice, typename S_forany>
  class Out_Fixed_Array_SArgument_T : public Argument
  {
  public:
    Out_Fixed_Array_SArgument_T (void);

    virtual CORBA::Boolean marshal (TAO_OutputCDR &);
#if TAO_HAS_INTERCEPTORS == 1
    virtual void interceptor_param (Dynamic::Parameter &);
#endif /* TAO_HAS_INTERCEPTORS == 1 */
    S_slice * arg (void);

  private:
    S x_;
  };

  /**
   * @class Ret_Fixed_Array_SArgument_T
   *
   * @brief Skeleton value of fixed size element array.
   *
   */
  template<typename S_slice, typename S_var, typename S_forany>
  class Ret_Fixed_Array_SArgument_T : public Argument
  {
  public:
    Ret_Fixed_Array_SArgument_T (void);

    virtual CORBA::Boolean marshal (TAO_OutputCDR &);
#if TAO_HAS_INTERCEPTORS == 1
    virtual void interceptor_result (CORBA::Any *);
#endif /* TAO_HAS_INTERCEPTORS == 1 */
    S_slice *& arg (void);

  private:
    S_var x_;
  };

  /**
   * @struct Fixed_Array_Arg_Traits_T
   *
   * @brief Argument traits of fixed size element array.
   *
   */
  template<typename T,
           typename T_slice,
           typename T_var,
           typename T_forany>
  struct Fixed_Array_SArg_Traits_T
  {
    typedef T_slice *                                   ret_type;
    typedef const T                                     in_type;
    typedef T                                           inout_type;
    typedef T                                           out_type;

    typedef In_Fixed_Array_SArgument_T<T,
                                       T_slice,
                                       T_forany>        in_arg_val;
    typedef Inout_Fixed_Array_SArgument_T<T,
                                          T_slice,
                                          T_forany>     inout_arg_val;
    typedef Out_Fixed_Array_SArgument_T<T,
                                        T_slice,
                                        T_forany>       out_arg_val;
    typedef Ret_Fixed_Array_SArgument_T<T_slice,
                                        T_var,
                                        T_forany>       ret_val;

    // Typedefs corresponding to return value of arg() method in both
    // the client and server side argument class templates.
    typedef T_slice const *                             in_arg_type;
    typedef ret_type                                    inout_arg_type;
    typedef ret_type                                    out_arg_type;
    typedef ret_type &                                  ret_arg_type;

  };
}

#if defined (__ACE_INLINE__)
#include "tao/PortableServer/Fixed_Array_SArgument_T.inl"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "tao/PortableServer/Fixed_Array_SArgument_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Fixed_Array_SArgument_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include /**/ "ace/post.h"

#endif /* TAO_FIXED_ARRAY_SARGUMENT_T_H */
