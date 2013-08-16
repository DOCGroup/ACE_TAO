// -*- C++ -*-

//=============================================================================
/**
 *  @file    UB_String_Argument_T.h
 *
 *  $Id$
 *
 *  @author Jeff Parsons
 *  @author Carlos O'Ryan
 */
//=============================================================================

#ifndef TAO_UB_STRING_ARGUMENT_T_H
#define TAO_UB_STRING_ARGUMENT_T_H

#include /**/ "ace/pre.h"

#include "tao/Argument.h"
#include "tao/CORBA_String.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  /**
   * @class In_UB_String_Argument_T
   *
   * @brief Template class for IN unbounded (w)string argument.
   *
   */
  template<typename S_var,
           template <typename> class Insert_Policy>
  class In_UB_String_Argument_T : public InArgument
  {
  public:
    In_UB_String_Argument_T (const typename S_var::s_traits::char_type * x);

    virtual CORBA::Boolean marshal (TAO_OutputCDR &cdr);
#if TAO_HAS_INTERCEPTORS == 1
    virtual void interceptor_value (CORBA::Any *any) const;
#endif /* TAO_HAS_INTERCEPTORS == 1 */
    const typename S_var::s_traits::char_type * arg (void) const;

  protected:
    typename S_var::s_traits::char_type const * x_;
  };

  /**
   * @class In_UB_String_Clonable_Argument_T
   *
   * @brief Template class for IN unbounded (w)string argument.
   *
   */
  template<typename S_var,
           template <typename> class Insert_Policy>
  class In_UB_String_Clonable_Argument_T :
            public In_UB_String_Argument_T<S_var, Insert_Policy>
  {
  public:
    In_UB_String_Clonable_Argument_T (const typename S_var::s_traits::char_type * x);
    virtual ~In_UB_String_Clonable_Argument_T (void);

    virtual Argument* clone (void);

  private:
    bool is_clone_;
  };

  /**
   * @class Inout_UB_String_Argument_T
   *
   * @brief Template class for INOUT unbounded (w)string argument.
   *
   */
  template<typename S_var,
           template <typename> class Insert_Policy>
  class Inout_UB_String_Argument_T : public InoutArgument
  {
  public:
    Inout_UB_String_Argument_T (typename S_var::s_traits::char_type *& x);

    virtual CORBA::Boolean marshal (TAO_OutputCDR &cdr);
    virtual CORBA::Boolean demarshal (TAO_InputCDR &);
#if TAO_HAS_INTERCEPTORS == 1
    virtual void interceptor_value (CORBA::Any *any) const;
#endif /* TAO_HAS_INTERCEPTORS == 1 */
    typename S_var::s_traits::char_type *& arg (void);

  private:
    typename S_var::s_traits::char_type *& x_;
  };

  /**
   * @class Out_UB_String_Argument_T
   *
   * @brief Template class for OUT unbounded (w)string argument.
   *
   */
  template<typename S_var,
           template <typename> class Insert_Policy>
  class Out_UB_String_Argument_T : public OutArgument
  {
  public:
    Out_UB_String_Argument_T (typename S_var::s_traits::string_out & x);

    virtual CORBA::Boolean demarshal (TAO_InputCDR &);
#if TAO_HAS_INTERCEPTORS == 1
    virtual void interceptor_value (CORBA::Any *any) const;
#endif /* TAO_HAS_INTERCEPTORS == 1 */
    typename S_var::s_traits::char_type *& arg (void);

  private:
    typename S_var::s_traits::char_type *& x_;
  };

  /**
   * @class Ret_UB_String_Argument_T
   *
   * @brief Template class for return stub value of ub (w)string argument.
   *
   */
  template<typename S_var,
           template <typename> class Insert_Policy>
  class Ret_UB_String_Argument_T : public RetArgument
  {
  public:
    Ret_UB_String_Argument_T (void);

    virtual CORBA::Boolean demarshal (TAO_InputCDR &);
#if TAO_HAS_INTERCEPTORS == 1
    virtual void interceptor_value (CORBA::Any *any) const;
#endif /* TAO_HAS_INTERCEPTORS == 1 */
    typename S_var::s_traits::char_type *& arg (void);
    typename S_var::s_traits::char_type * excp (void);
    typename S_var::s_traits::char_type * retn (void);

  private:
    S_var x_;
  };

  /**
   * @struct UB_String_Arg_Traits_T
   *
   * @brief Template class for argument traits of unbounded (w)strings.
   *
   */
  template<typename T_var,
           template <typename> class Insert_Policy>
  struct UB_String_Arg_Traits_T
  {
    typedef typename T_var::s_traits::char_type *            ret_type;
    typedef typename T_var::s_traits::char_type const *      in_type;
    typedef typename T_var::s_traits::char_type *&           inout_type;
    typedef typename T_var::s_traits::string_out             out_type;

    typedef In_UB_String_Argument_T<T_var, Insert_Policy>    in_arg_val;
    typedef In_UB_String_Clonable_Argument_T<T_var,
                                             Insert_Policy>  in_clonable_arg_val;
    typedef Inout_UB_String_Argument_T<T_var, Insert_Policy> inout_arg_val;
    typedef Out_UB_String_Argument_T<T_var, Insert_Policy>   out_arg_val;
    typedef Ret_UB_String_Argument_T<T_var, Insert_Policy>   ret_val;
  };
}

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "tao/UB_String_Argument_T.inl"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "tao/UB_String_Argument_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("UB_String_Argument_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include /**/ "ace/post.h"

#endif /* TAO_UB_STRING_ARGUMENT_T_H */
