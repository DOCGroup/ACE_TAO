// This may look like C, but it's really -*- C++ -*-

//=============================================================================
/**
 *  @file    Special_Basic_Argument_T.h
 *
 *  $Id$
 *
 *  @authors Jeff Parsons and Carlos O'Ryan
 */
//=============================================================================


#ifndef TAO_SPECIAL_BASIC_ARGUMENT_T_H
#define TAO_SPECIAL_BASIC_ARGUMENT_T_H

#include "ace/pre.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Argument.h"

namespace TAO
{
  /**
   * @class In_Special_Basic_Argument_T
   *
   * @brief Template class for IN stub argument of (w)char/boolean/octet.
   *
   */
  template<typename S, typename to_S, typename from_S>
  class TAO_Export In_Special_Basic_Argument_T : public Argument
  {
  public:
    In_Special_Basic_Argument_T (S const &);

    virtual CORBA::Boolean marshal (TAO_OutputCDR &);

    virtual void interceptor_param (Dynamic::Parameter &);
    virtual CORBA::Boolean interceptor_replace (CORBA::Any &);

  private:
    S const & x_;
  };

  /**
   * @class Inout_Special_Basic_Argument_T
   *
   * @brief Template class for INOUT stub argument of (w)char/boolean/octet.
   *
   */
  template<typename S, typename to_S, typename from_S>
  class TAO_Export Inout_Special_Basic_Argument_T : public Argument
  {
  public:
    Inout_Special_Basic_Argument_T (S & x);

    virtual CORBA::Boolean marshal (TAO_OutputCDR &);
    virtual CORBA::Boolean demarshal (TAO_InputCDR &);

    virtual void interceptor_param (Dynamic::Parameter &);
    virtual CORBA::Boolean interceptor_replace (CORBA::Any &);

  private:
    mutable S & x_;
  };

  /**
   * @class Out_Special_Basic_Argument_T
   *
   * @brief Template class for OUT stub argument of (w)char/boolean/octet.
   *
   */
  template<typename S, typename to_S, typename from_S>
  class TAO_Export Out_Special_Basic_Argument_T : public Argument
  {
  public:
    Out_Special_Basic_Argument_T (S & x);

    virtual CORBA::Boolean demarshal (TAO_InputCDR &);

  private:
    mutable S & x_;
  };

  /**
   * @class Ret_Basic_Argument_T
   *
   * @brief Template class for return stub value of (w)char/boolean/octet.
   *
   */
  template<typename S, typename to_S, typename from_S>
  class TAO_Export Ret_Special_Basic_Argument_T : public Argument
  {
  public:
    Ret_Special_Basic_Argument_T (void);

    virtual CORBA::Boolean demarshal (TAO_InputCDR &);

    virtual void interceptor_result (CORBA::Any *);
    virtual CORBA::Boolean interceptor_replace (CORBA::Any &);

    S excp (void);
    S retn (void);

  private:
    S x_;
  };

  /**
   * @class In_Special_Basic_SArgument_T
   *
   * @brief Template class for IN skeleton argument of (w)char/boolean/octet.
   *
   */
  template<typename S, typename to_S, typename from_S>
  class TAO_Export In_Special_Basic_SArgument_T : public Argument
  {
  public:
    In_Special_Basic_SArgument_T (void);

    virtual CORBA::Boolean demarshal (TAO_InputCDR &);

    virtual void interceptor_param (Dynamic::Parameter &);
    virtual CORBA::Boolean interceptor_replace (CORBA::Any &);

    S arg (void) const;

  private:
    S x_;
  };

  /**
   * @class Inout_Special_Basic_SArgument_T
   *
   * @brief Template class for INOUT (w)char/boolean/octet skeleton argument.
   *
   */
  template<typename S, typename to_S, typename from_S>
  class TAO_Export Inout_Special_Basic_SArgument_T : public Argument
  {
  public:
    Inout_Special_Basic_SArgument_T (void);

    virtual CORBA::Boolean marshal (TAO_OutputCDR &);
    virtual CORBA::Boolean demarshal (TAO_InputCDR &);

    virtual void interceptor_param (Dynamic::Parameter &);
    virtual CORBA::Boolean interceptor_replace (CORBA::Any &);

    S & arg (void);

  private:
    S x_;
  };

  /**
   * @class Out_Special_Basic_SArgument_T
   *
   * @brief Template class for OUT skeleton argument of (w)char/boolean/octet.
   *
   */
  template<typename S, typename to_S, typename from_S>
  class TAO_Export Out_Special_Basic_SArgument_T : public Argument
  {
  public:
    Out_Special_Basic_SArgument_T (void);

    virtual CORBA::Boolean marshal (TAO_OutputCDR &);

    S & arg (void);

  private:
    S x_;
  };

  /**
   * @class Ret_Basic_SArgument_T
   *
   * @brief Template class for return skeleton value of (w)char/boolean/octet.
   *
   */
  template<typename S, typename to_S, typename from_S>
  class TAO_Export Ret_Special_Basic_SArgument_T : public Argument
  {
  public:
    Ret_Special_Basic_SArgument_T (void);

    virtual CORBA::Boolean marshal (TAO_OutputCDR &);

    virtual void interceptor_result (CORBA::Any *);
    virtual CORBA::Boolean interceptor_replace (CORBA::Any &);

    S & arg (void);

  private:
    S x_;
  };

  /**
   * @struct Special_Basic_Tag
   *
   * @brief Struct for basic IDL type arguments id tag.
   *
   */
  struct TAO_Export Special_Basic_Tag {};

  /**
   * @struct Special_Basic_Traits_T
   *
   * @brief Template class for argument traits of (w)char/boolean/octet.
   *
   */
  template<typename T, typename to_T, typename from_T>
  struct TAO_Export Special_Basic_Arg_Traits_T
  {
    typedef T                                               ret_type;
    typedef T                                               in_type;
    typedef T &                                             inout_type;
    typedef T &                                             out_type;

    typedef In_Special_Basic_Argument_T<T,to_T,from_T>      in_arg_val;
    typedef Inout_Special_Basic_Argument_T<T,to_T,from_T>   inout_arg_val;
    typedef Out_Special_Basic_Argument_T<T,to_T,from_T>     out_arg_val;
    typedef Ret_Special_Basic_Argument_T<T,to_T,from_T>     stub_ret_val;

    typedef In_Special_Basic_SArgument_T<T,to_T,from_T>     in_sarg_val;
    typedef Inout_Special_Basic_SArgument_T<T,to_T,from_T>  inout_sarg_val;
    typedef Out_Special_Basic_SArgument_T<T,to_T,from_T>    out_sarg_val;
    typedef Ret_Special_Basic_SArgument_T<T,to_T,from_T>    skel_ret_val;

    typedef Special_Basic_Tag                               idl_tag;
  };
};

#if defined (__ACE_INLINE__)
#include "tao/Special_Basic_Argument_T.inl"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "tao/Special_Basic_Argument_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Special_Basic_Argument_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include "ace/post.h"

#endif /* TAO_SPECIAL_BASIC_ARGUMENT_T_H */
