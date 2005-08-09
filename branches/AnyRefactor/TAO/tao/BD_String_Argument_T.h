// -*- C++ -*-

//=============================================================================
/**
 *  @file    BD_String_Argument_T.h
 *
 *  $Id$
 *
 *  @authors Jeff Parsons and Carlos O'Ryan
 */
//=============================================================================


#ifndef TAO_BD_STRING_ARGUMENT_T_H
#define TAO_BD_STRING_ARGUMENT_T_H

#include /**/ "ace/pre.h"

#include "ace/CDR_Stream.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Argument.h"

namespace TAO
{
  /**
   * @class In_BD_String_Argument_T
   *
   * @brief Template class for IN bounded (w)string argument.
   *
   */
  template<typename S,
           typename to_S,
           typename from_S,
           size_t BOUND,
           typename Insert_Policy>
  class In_BD_String_Argument_T : public InArgument, private Insert_Policy
  {
  public:
    In_BD_String_Argument_T (const S * x);

    virtual CORBA::Boolean marshal (TAO_OutputCDR &cdr);

#if TAO_HAS_INTERCEPTORS == 1
    virtual void interceptor_value (CORBA::Any *any) const;
#endif /* TAO_HAS_INTERCEPTORS == 1 */

    S const * arg (void) const;

  private:

    S const * x_;

  };

  /**
   * @class Inout_BD_String_Argument_T
   *
   * @brief Template class for INOUT bounded (w)string argument.
   *
   */
  template<typename S,
           typename to_S,
           typename from_S,
           size_t BOUND,
           typename Insert_Policy>
  class Inout_BD_String_Argument_T : public InoutArgument, private Insert_Policy
  {
  public:
    Inout_BD_String_Argument_T (S *& x);

    virtual CORBA::Boolean marshal (TAO_OutputCDR &cdr);
    virtual CORBA::Boolean demarshal (TAO_InputCDR &);
#if TAO_HAS_INTERCEPTORS == 1
    virtual void interceptor_value (CORBA::Any *any) const;
#endif /* TAO_HAS_INTERCEPTORS == 1 */
    S *& arg (void);

  private:
    S *& x_;
  };

  /**
   * @class Out_BD_String_Argument_T
   *
   * @brief Template class for OUT bounded (w)string argument.
   *
   */
  template<typename S,
           typename S_out,
           typename to_S,
           typename from_S,
           size_t BOUND,
           typename Insert_Policy>
  class Out_BD_String_Argument_T : public OutArgument, private Insert_Policy
  {
  public:
    Out_BD_String_Argument_T (S_out x);

    virtual CORBA::Boolean demarshal (TAO_InputCDR &);
#if TAO_HAS_INTERCEPTORS == 1
    virtual void interceptor_value (CORBA::Any *any) const;
#endif /* TAO_HAS_INTERCEPTORS == 1 */
    S *& arg (void);

  private:
    S *& x_;
  };

  /**
   * @class Ret_BD_String_Argument_T
   *
   * @brief Template class for return stub value of bd (w)string argument.
   *
   */
  template<typename S,
           typename S_var,
           typename to_S,
           typename from_S,
           size_t BOUND,
           typename Insert_Policy>
  class Ret_BD_String_Argument_T : public RetArgument, private Insert_Policy
  {
  public:
    Ret_BD_String_Argument_T (void);

    virtual CORBA::Boolean demarshal (TAO_InputCDR &);
#if TAO_HAS_INTERCEPTORS == 1
    virtual void interceptor_value (CORBA::Any *any) const;
#endif /* TAO_HAS_INTERCEPTORS == 1 */
    S *& arg (void);

    S * excp (void);
    S * retn (void);

  private:

    S_var x_;

  };

  /**
   * @struct BD_String_Tag
   *
   * @brief Struct for bounded (w)string arguments id tag.
   *
   */
  struct TAO_Export BD_String_Tag {};

  /**
   * @struct BD_String_Arg_Traits_T
   *
   * @brief Template class for stub argument traits of bounded (w)strings.
   *
   */
  template<typename T,
           typename T_var,
           typename T_out,
           typename to_T,
           typename from_T,
           size_t BOUND,
           typename Insert_Policy>
  struct BD_String_Arg_Traits_T
  {
    typedef T *                                   ret_type;
    typedef T const *                             in_type;
    typedef T *&                                  inout_type;
    typedef T_out                                 out_type;

    typedef In_BD_String_Argument_T<T,
                                    to_T,
                                    from_T,
                                    BOUND,
                                    Insert_Policy>        in_arg_val;
    typedef Inout_BD_String_Argument_T<T,
                                       to_T,
                                       from_T,
                                       BOUND,
                                       Insert_Policy>     inout_arg_val;
    typedef Out_BD_String_Argument_T<T,
                                     T_out,
                                     to_T,
                                     from_T,
                                     BOUND,
                                     Insert_Policy>       out_arg_val;
    typedef Ret_BD_String_Argument_T<T,
                                     T_var,
                                     to_T,
                                     from_T,
                                     BOUND,
                                     Insert_Policy>       ret_val;

    typedef BD_String_Tag                         idl_tag;
  };

  /**
   *
   * @brief Specializations for bounded (w)strings
   *
   */

  template<size_t BOUND>
  class BD_String_Arg_Traits
    : public BD_String_Arg_Traits_T<CORBA::Char,
                                    CORBA::String_var,
                                    CORBA::String_out,
                                    ACE_InputCDR::to_string,
                                    ACE_OutputCDR::from_string,
                                    BOUND>
  {
  };

  template<size_t BOUND>
  class BD_WString_Arg_Traits
    : public BD_String_Arg_Traits_T<CORBA::WChar,
                                    CORBA::WString_var,
                                    CORBA::WString_out,
                                    ACE_InputCDR::to_wstring,
                                    ACE_OutputCDR::from_wstring,
                                    BOUND>
  {
  };
}

#if defined (__ACE_INLINE__)
#include "tao/BD_String_Argument_T.inl"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "tao/BD_String_Argument_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("BD_String_Argument_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include /**/ "ace/post.h"

#endif /* TAO_BD_STRING_ARGUMENT_T_H */
