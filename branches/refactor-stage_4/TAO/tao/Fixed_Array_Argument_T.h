// This may look like C, but it's really -*- C++ -*-

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

#include "ace/pre.h"
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
  template<typename S, typename S_slice, typename S_forany>
  class In_Fixed_Array_Argument_T : public Argument
  {
  public:
    In_Fixed_Array_Argument_T (const S x);

    virtual CORBA::Boolean marshal (TAO_OutputCDR &);

    virtual void interceptor_param (Dynamic::Parameter &);
    virtual CORBA::Boolean interceptor_replace (CORBA::Any &);

    const S & arg (void) const;

  private:
    S_forany x_;
  };

  /**
   * @class Inout_Fixed_Array_Argument_T
   *
   * @brief INOUT stub argument of fixed size element array.
   *
   */
  template<typename S, typename S_forany>
  class Inout_Fixed_Array_Argument_T : public Argument
  {
  public:
    Inout_Fixed_Array_Argument_T (S x);

    virtual CORBA::Boolean marshal (TAO_OutputCDR &);
    virtual CORBA::Boolean demarshal (TAO_InputCDR &);

    virtual void interceptor_param (Dynamic::Parameter &);
    virtual CORBA::Boolean interceptor_replace (CORBA::Any &);

    S & arg (void);

  private:
    S_forany x_;
  };

  /**
   * @class Out_Fixed_Array_Argument_T
   *
   * @brief OUT stub argument of fixed size element array.
   *
   */
  template<typename S, typename S_forany>
  class Out_Fixed_Array_Argument_T : public Argument
  {
  public:
    Out_Fixed_Array_Argument_T (S x);

    virtual CORBA::Boolean demarshal (TAO_InputCDR &);

    S & arg (void);

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
           typename S_forany>
  class Ret_Fixed_Array_Argument_T : public Argument
  {
  public:
    Ret_Fixed_Array_Argument_T (void);

    virtual CORBA::Boolean demarshal (TAO_InputCDR &);

    virtual void interceptor_result (CORBA::Any *);
    virtual CORBA::Boolean interceptor_replace (CORBA::Any &);

    S_slice *& arg (void);

    S_slice * excp (void);
    S_slice * retn (void);

  private:
    S_var x_;
  };

  /**
   * @class In_Fixed_Array_SArgument_T
   *
   * @brief IN skeleton argument of fixed size element array.
   *
   */
  template<typename S, typename S_forany>
  class In_Fixed_Array_SArgument_T : public Argument
  {
  public:
    In_Fixed_Array_SArgument_T (void);

    virtual CORBA::Boolean demarshal (TAO_InputCDR &);

    virtual void interceptor_param (Dynamic::Parameter &);
    virtual CORBA::Boolean interceptor_replace (CORBA::Any &);

    const S & arg (void) const;

  private:
    S x_;
  };

  /**
   * @class Inout_Fixed_Array_SArgument_T
   *
   * @brief INOUT skeleton argument of fixed size element array.
   *
   */
  template<typename S, typename S_forany>
  class Inout_Fixed_Array_SArgument_T : public Argument
  {
  public:
    Inout_Fixed_Array_SArgument_T (void);

    virtual CORBA::Boolean marshal (TAO_OutputCDR &);
    virtual CORBA::Boolean demarshal (TAO_InputCDR &);

    virtual void interceptor_param (Dynamic::Parameter &);
    virtual CORBA::Boolean interceptor_replace (CORBA::Any &);

    S & arg (void);

  private:
    S x_;
  };

  /**
   * @class Out_Fixed_Array_SArgument_T
   *
   * @brief OUT skeleton argument of fixed size element array.
   *
   */
  template<typename S, typename S_forany>
  class Out_Fixed_Array_SArgument_T : public Argument
  {
  public:
    Out_Fixed_Array_SArgument_T (void);

    virtual CORBA::Boolean marshal (TAO_OutputCDR &);

    S & arg (void);

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

    virtual void interceptor_result (CORBA::Any *);
    virtual CORBA::Boolean interceptor_replace (CORBA::Any &);

    S_slice *& arg (void);

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
           typename T_forany>
  struct Fixed_Array_Arg_Traits_T
  {
    typedef T_slice *                                   ret_type;
    typedef const T                                     in_type;
    typedef T                                           inout_type;
    typedef T                                           out_type;

    typedef In_Fixed_Array_Argument_T<T,
                                      T_slice,
                                      T_forany>         in_arg_val;
    typedef Inout_Fixed_Array_Argument_T<T,T_forany>    inout_arg_val;
    typedef Out_Fixed_Array_Argument_T<T,T_forany>      out_arg_val;
    typedef Ret_Fixed_Array_Argument_T<T,
                                       T_slice,
                                       T_var,
                                       T_forany>        stub_ret_val;

    typedef In_Fixed_Array_SArgument_T<T,T_forany>      in_sarg_val;
    typedef Inout_Fixed_Array_SArgument_T<T,T_forany>   inout_sarg_val;
    typedef Out_Fixed_Array_SArgument_T<T,T_forany>     out_sarg_val;
    typedef Ret_Fixed_Array_SArgument_T<T_slice,
                                        T_var,
                                        T_forany>       skel_ret_val;

    typedef Fixed_Array_Tag                             idl_tag;
  };
};

#if defined (__ACE_INLINE__)
#include "tao/Fixed_Array_Argument_T.inl"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "tao/Fixed_Array_Argument_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Fixed_Array_Argument_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include "ace/post.h"

#endif /* TAO_FIXED_ARRAY_ARGUMENT_T_H */
