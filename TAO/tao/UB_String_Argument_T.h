// This may look like C, but it's really -*- C++ -*-

//=============================================================================
/**
 *  @file    UB_String_Argument_T.h
 *
 *  $Id$
 *
 *  @authors Jeff Parsons and Carlos O'Ryan
 */
//=============================================================================


#ifndef TAO_UB_STRING_ARGUMENT_T_H
#define TAO_UB_STRING_ARGUMENT_T_H

#include "ace/pre.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Argument.h"

namespace TAO
{
  /**
   * @class In_UB_String_Argument_T
   *
   * @brief Template class for IN unbounded (w)string argument.
   *
   */
  template<typename S>
  class In_UB_String_Argument_T : public Argument
  {
  public:
    In_UB_String_Argument_T (S * const & x);

    virtual CORBA::Boolean marshal (TAO_OutputCDR &);
    virtual CORBA::Boolean demarshal (TAO_InputCDR &);
    virtual void add_to_interceptor (Dynamic::Parameter &);

  private:
    S * const & x_;
  };

  /**
   * @class Inout_UB_String_Argument_T
   *
   * @brief Template class for INOUT unbounded (w)string argument.
   *
   */
  template<typename S>
  class Inout_UB_String_Argument_T : public Argument
  {
  public:
    Inout_UB_String_Argument_T (S *& x);

    virtual CORBA::Boolean marshal (TAO_OutputCDR &);
    virtual CORBA::Boolean demarshal (TAO_InputCDR &);
    virtual void add_to_interceptor (Dynamic::Parameter &);

  private:
    mutable S *& x_;
  };

  /**
   * @class Out_UB_String_Argument_T
   *
   * @brief Template class for INOUT unbounded (w)string argument.
   *
   */
  template<typename S>
  class Out_UB_String_Argument_T : public Argument
  {
  public:
    Out_UB_String_Argument_T (S *& x);

    virtual CORBA::Boolean marshal (TAO_OutputCDR &);
    virtual CORBA::Boolean demarshal (TAO_InputCDR &);
    virtual void add_to_interceptor (Dynamic::Parameter &);

  private:
    mutable S *& x_;
  };

  /**
   * @class Ret_UB_String_Argument_T
   *
   * @brief Template class for return stub value of ub (w)string argument.
   *
   */
  template<typename S>
  class Ret_UB_String_Argument_T : public Stub_Retval
  {
  public:
    Ret_UB_String_Argument_T (void);

    virtual CORBA::Boolean demarshal (TAO_InputCDR &);
    virtual void add_to_interceptor (CORBA::Any *);

    operator S * () const;

  private:
    S * x_;
  };

  /**
   * @class In_UB_String_SArgument_T
   *
   * @brief Template class for IN skeleton UB (w)string argument.
   *
   */
  template<typename S>
  class In_UB_String_SArgument_T : public Argument
  {
  public:
    In_UB_String_SArgument_T (void);

    virtual CORBA::Boolean marshal (TAO_OutputCDR &);
    virtual CORBA::Boolean demarshal (TAO_InputCDR &);
    virtual void add_to_interceptor (Dynamic::Parameter &);

    operator S * () const;

  private:
    S * x_;
  };

  /**
   * @class Inout_UB_String_SArgument_T
   *
   * @brief Template class for INOUT skeleton UB (w)string argument.
   *
   */
  template<typename S>
  class Inout_UB_String_SArgument_T : public Argument
  {
  public:
    Inout_UB_String_SArgument_T (void);

    virtual CORBA::Boolean marshal (TAO_OutputCDR &);
    virtual CORBA::Boolean demarshal (TAO_InputCDR &);
    virtual void add_to_interceptor (Dynamic::Parameter &);

    operator S *& ();

  private:
    S * x_;
  };

  /**
   * @class Out_UB_String_SArgument_T
   *
   * @brief Template class for INOUT skeleton UB (w)string argument.
   *
   */
  template<typename S>
  class Out_UB_String_SArgument_T : public Argument
  {
  public:
    Out_UB_String_SArgument_T (void);

    virtual CORBA::Boolean marshal (TAO_OutputCDR &);
    virtual CORBA::Boolean demarshal (TAO_InputCDR &);
    virtual void add_to_interceptor (Dynamic::Parameter &);

    operator S *& ();

  private:
    S * x_;
  };

  /**
   * @class Ret_UB_String_SArgument_T
   *
   * @brief Template class for return skeleton value of UB (w)string.
   *
   */
  template<typename S>
  class Ret_UB_String_SArgument_T : public Skel_Retval
  {
  public:
    Ret_UB_String_SArgument_T (void);

    virtual CORBA::Boolean marshal (TAO_OutputCDR &);
    virtual void add_to_interceptor (CORBA::Any *);

    operator S * () const;
    operator S *& ();

  private:
    S * x_;
  };

  /**
   * @struct UB_String_Tag
   *
   * @brief Struct for unbounded (w)string arguments id tag.
   *
   */
  struct TAO_Export UB_String_Tag {};

  /**
   * @struct Basic_Arg_Traits_T
   *
   * @brief Template class for argument traits of unbounded (w)strings.
   *
   */
  template<typename T, typename T_out>
  struct UB_String_Arg_Traits_T
  {
    typedef T *                             ret_type;
    typedef const T *                       in_type;
    typedef T *&                            inout_type;
    typedef T_out                           out_type;

    typedef In_UB_String_Argument_T<T>      in_arg_val;
    typedef Inout_UB_String_Argument_T<T>   inout_arg_val;
    typedef Out_UB_String_Argument_T<T>     out_arg_val;
    typedef Ret_UB_String_Argument_T<T>     stub_ret_val;

    typedef In_UB_String_SArgument_T<T>     in_sarg_val;
    typedef Inout_UB_String_SArgument_T<T>  inout_sarg_val;
    typedef Out_UB_String_SArgument_T<T>    out_sarg_val;
    typedef Ret_UB_String_SArgument_T<T>    skel_ret_val;

    typedef UB_String_Tag                   idl_tag;
  };
};

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "tao/UB_String_Argument_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("UB_String_Argument_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include "ace/post.h"

#endif /* TAO_UB_STRING_ARGUMENT_T_H */

