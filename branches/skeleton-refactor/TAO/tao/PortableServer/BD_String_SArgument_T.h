// -*- C++ -*-

//=============================================================================
/**
 *  @file    BD_String_SArgument_T.h
 *
 *  $Id$
 *
 *  @authors Jeff Parsons, Carlos O'Ryan and Ossama Othman
 */
//=============================================================================


#ifndef TAO_BD_STRING_SARGUMENT_T_H
#define TAO_BD_STRING_SARGUMENT_T_H

#include /**/ "ace/pre.h"

#include "ace/CDR_Stream.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Argument_T.h"

namespace TAO
{
  /**
   * @class In_BD_String_SArgument_T
   *
   * @brief Template class for IN skeleton bd (w)string argument.
   *
   */
  template<typename S,
           typename S_var,
           typename to_S,
           typename from_S,
           size_t BOUND>
  class In_BD_String_SArgument_T : public Argument
  {
  public:
    In_BD_String_SArgument_T (void);

    virtual CORBA::Boolean demarshal (TAO_InputCDR &);
#if TAO_HAS_INTERCEPTORS == 1
    virtual void interceptor_param (Dynamic::Parameter &);
#endif /* TAO_HAS_INTERCEPTORS == 1 */
    const S * arg (void) const;

  private:
    S_var x_;
  };

  /**
   * @class Inout_BD_String_SArgument_T
   *
   * @brief Template class for INOUT skeleton bd (w)string argument.
   *
   */
  template<typename S,
           typename S_var,
           typename to_S,
           typename from_S,
           size_t BOUND>
  class Inout_BD_String_SArgument_T : public Argument
  {
  public:
    Inout_BD_String_SArgument_T (void);

    virtual CORBA::Boolean marshal (TAO_OutputCDR &);
    virtual CORBA::Boolean demarshal (TAO_InputCDR &);
#if TAO_HAS_INTERCEPTORS == 1
    virtual void interceptor_param (Dynamic::Parameter &);
#endif /* TAO_HAS_INTERCEPTORS == 1 */
    S *& arg (void);

  private:
    S_var x_;
  };

  /**
   * @class Out_BD_String_SArgument_T
   *
   * @brief Template class for INOUT skeleton bd (w)string argument.
   *
   */
  template<typename S,
           typename S_var,
           typename S_out,
           typename to_S,
           typename from_S,
           size_t BOUND>
  class Out_BD_String_SArgument_T : public Argument
  {
  public:
    Out_BD_String_SArgument_T (void);

    virtual CORBA::Boolean marshal (TAO_OutputCDR &);
#if TAO_HAS_INTERCEPTORS == 1
    virtual void interceptor_param (Dynamic::Parameter &);
#endif /* TAO_HAS_INTERCEPTORS == 1 */
    S *& arg (void);

  private:
    S_var x_;
  };

  /**
   * @class Ret_BD_String_SArgument_T
   *
   * @brief Template class for return skeleton value of bd (w)string.
   *
   */
  template<typename S,
           typename S_var,
           typename to_S,
           typename from_S,
           size_t BOUND>
  class Ret_BD_String_SArgument_T : public Argument
  {
  public:
    Ret_BD_String_SArgument_T (void);

    virtual CORBA::Boolean marshal (TAO_OutputCDR &);
#if TAO_HAS_INTERCEPTORS == 1
    virtual void interceptor_result (CORBA::Any *);
#endif /* TAO_HAS_INTERCEPTORS == 1 */
    S *& arg (void);

  private:
    S_var x_;
  };

  /**
   * @struct Basic_SArg_Traits_T
   *
   * @brief Template class for skeleton argument traits of bounded (w)strings.
   *
   */
  template<typename T,
           typename T_var,
           typename T_out,
           typename to_T,
           typename from_T,
           size_t BOUND>
  struct BD_String_SArg_Traits_T
  {
    typedef T *                                   ret_type;
    typedef const T *                             in_type;
    typedef T *&                                  inout_type;
    typedef T_out                                 out_type;

    typedef In_BD_String_SArgument_T<T,
                                     T_var,
                                     to_T,
                                     from_T,
                                     BOUND>       in_arg_val;
    typedef Inout_BD_String_SArgument_T<T,
                                        T_var,
                                        to_T,
                                        from_T,
                                        BOUND>    inout_arg_val;
    typedef Out_BD_String_SArgument_T<T,
                                      T_var,
                                      T_out,
                                      to_T,
                                      from_T,
                                      BOUND>      out_arg_val;
    typedef Ret_BD_String_SArgument_T<T,
                                      T_var,
                                      to_T,
                                      from_T,
                                      BOUND>      ret_val;
  };

  /**
   *
   * @brief Specializations for bounded (w)strings
   *
   */

  template<size_t BOUND>
  class BD_String_SArg_Traits
    : public BD_String_SArg_Traits_T<CORBA::Char,
                                     CORBA::String_var,
                                     CORBA::String_out,
                                     ACE_InputCDR::to_string,
                                     ACE_OutputCDR::from_string,
                                     BOUND>
  {
  };

  template<size_t BOUND>
  class BD_WString_SArg_Traits
    : public BD_String_SArg_Traits_T<CORBA::WChar,
                                     CORBA::WString_var,
                                     CORBA::WString_out,
                                     ACE_InputCDR::to_wstring,
                                     ACE_OutputCDR::from_wstring,
                                     BOUND>
  {
  };
};

#if defined (__ACE_INLINE__)
#include "tao/PortableServer/BD_String_SArgument_T.inl"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "tao/PortableServer/BD_String_SArgument_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("BD_String_SArgument_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include /**/ "ace/post.h"

#endif /* TAO_BD_STRING_SARGUMENT_T_H */
