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

#include "tao/Argument.h"
#include "tao/Any_Insert_Policy_T.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  /**
   * @class In_BD_String_SArgument_T
   *
   * @brief Template class for IN skeleton bd (w)string argument.
   *
   */
  template<typename S_var,
           size_t BOUND,
           template <typename> class Insert_Policy>
  class In_BD_String_SArgument_T : public InArgument
  {
  public:
    In_BD_String_SArgument_T (void);

    virtual CORBA::Boolean demarshal (TAO_InputCDR &);
#if TAO_HAS_INTERCEPTORS == 1
    virtual void interceptor_value (CORBA::Any *any) const;
#endif /* TAO_HAS_INTERCEPTORS == 1 */
    const typename S_var::s_traits::char_type * arg (void) const;

  private:
    S_var x_;
  };

  /**
   * @class Inout_BD_String_SArgument_T
   *
   * @brief Template class for INOUT skeleton bd (w)string argument.
   *
   */
  template<typename S_var,
           size_t BOUND,
           template <typename> class Insert_Policy>
  class Inout_BD_String_SArgument_T : public InoutArgument
  {
  public:
    Inout_BD_String_SArgument_T (void);

    virtual CORBA::Boolean marshal (TAO_OutputCDR &cdr);
    virtual CORBA::Boolean demarshal (TAO_InputCDR &);
#if TAO_HAS_INTERCEPTORS == 1
    virtual void interceptor_value (CORBA::Any *any) const;
#endif /* TAO_HAS_INTERCEPTORS == 1 */
    typename S_var::s_traits::char_type *& arg (void);

  private:
    S_var x_;
  };

  /**
   * @class Out_BD_String_SArgument_T
   *
   * @brief Template class for INOUT skeleton bd (w)string argument.
   *
   */
  template<typename S_var,
           size_t BOUND,
           template <typename> class Insert_Policy>
  class Out_BD_String_SArgument_T : public OutArgument
  {
  public:
    Out_BD_String_SArgument_T (void);

    virtual CORBA::Boolean marshal (TAO_OutputCDR &cdr);
#if TAO_HAS_INTERCEPTORS == 1
    virtual void interceptor_value (CORBA::Any *any) const;
#endif /* TAO_HAS_INTERCEPTORS == 1 */
    typename S_var::s_traits::char_type *& arg (void);

  private:
    S_var x_;
  };

  /**
   * @class Ret_BD_String_SArgument_T
   *
   * @brief Template class for return skeleton value of bd (w)string.
   *
   */
  template<typename S_var,
           size_t BOUND,
           template <typename> class Insert_Policy>
  class Ret_BD_String_SArgument_T : public RetArgument
  {
  public:
    Ret_BD_String_SArgument_T (void);

    virtual CORBA::Boolean marshal (TAO_OutputCDR &cdr);
#if TAO_HAS_INTERCEPTORS == 1
    virtual void interceptor_value (CORBA::Any *any) const;
#endif /* TAO_HAS_INTERCEPTORS == 1 */
    typename S_var::s_traits::char_type *& arg (void);

  private:
    S_var x_;
  };

  /**
   * @struct Basic_SArg_Traits_T
   *
   * @brief Template class for skeleton argument traits of bounded (w)strings.
   *
   */
  template<typename T_var,
           size_t BOUND,
           template <typename> class Insert_Policy>
  struct BD_String_SArg_Traits_T
  {
    typedef typename T_var::s_traits::char_type *       ret_type;
    typedef const typename T_var::s_traits::char_type * in_type;
    typedef typename T_var::s_traits::char_type *&      inout_type;
    typedef typename T_var::s_traits::string_out        out_type;

    typedef In_BD_String_SArgument_T<T_var,
                                     BOUND,
                                     Insert_Policy>     in_arg_val;
    typedef Inout_BD_String_SArgument_T<T_var,
                                        BOUND,
                                        Insert_Policy>  inout_arg_val;
    typedef Out_BD_String_SArgument_T<T_var,
                                      BOUND,
                                      Insert_Policy>    out_arg_val;
    typedef Ret_BD_String_SArgument_T<T_var,
                                      BOUND,
                                      Insert_Policy>    ret_val;

    // Typedefs corresponding to return value of arg() method in both
    // the client and server side argument class templates.
    typedef in_type                                     in_arg_type;
    typedef inout_type                                  inout_arg_type;
    typedef inout_type                                  out_arg_type;
    typedef inout_type                                  ret_arg_type;
  };
}

TAO_END_VERSIONED_NAMESPACE_DECL

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
