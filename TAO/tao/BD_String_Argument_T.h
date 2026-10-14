// -*- C++ -*-

//=============================================================================
/**
 *  @file    BD_String_Argument_T.h
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
#include "tao/Any_Insert_Policy_T.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  /**
   * @class In_BD_String_Argument_T
   *
   * @brief Template class for IN bounded (w)string argument.
   *
   */
  template<typename S_var,
           size_t BOUND,
           template <typename> class Insert_Policy>
  class In_BD_String_Argument_T : public InArgument
  {
  public:
    In_BD_String_Argument_T (const typename S_var::s_traits::char_type * x);

    virtual CORBA::Boolean marshal (TAO_OutputCDR &cdr);

#if TAO_HAS_INTERCEPTORS == 1
    virtual void interceptor_value (CORBA::Any *any) const;
#endif /* TAO_HAS_INTERCEPTORS == 1 */

    typename S_var::s_traits::char_type const * arg () const;

  protected:
    typename S_var::s_traits::char_type const * x_;
  };

  /**
   * @class In_BD_String_Clonable_Argument_T
   *
   * @brief Template class for IN bounded (w)string argument.
   *
   */
  template<typename S_var,
           size_t BOUND,
           template <typename> class Insert_Policy>
  class In_BD_String_Clonable_Argument_T :
           public In_BD_String_Argument_T<S_var, BOUND, Insert_Policy>
  {
  public:
    In_BD_String_Clonable_Argument_T (const typename S_var::s_traits::char_type * x);
    virtual ~In_BD_String_Clonable_Argument_T ();

    virtual Argument* clone ();
  private:
    bool is_clone_;
  };

  /**
   * @class Inout_BD_String_Argument_T
   *
   * @brief Template class for INOUT bounded (w)string argument.
   *
   */
  template<typename S_var,
           size_t BOUND,
           template <typename> class Insert_Policy>
  class Inout_BD_String_Argument_T : public InoutArgument
  {
  public:
    Inout_BD_String_Argument_T (typename S_var::s_traits::char_type *& x);

    virtual CORBA::Boolean marshal (TAO_OutputCDR &cdr);
    virtual CORBA::Boolean demarshal (TAO_InputCDR &cdr);
#if TAO_HAS_INTERCEPTORS == 1
    virtual void interceptor_value (CORBA::Any *any) const;
#endif /* TAO_HAS_INTERCEPTORS == 1 */
    typename S_var::s_traits::char_type *& arg ();

  private:
    typename S_var::s_traits::char_type *& x_;
  };

  /**
   * @class Out_BD_String_Argument_T
   *
   * @brief Template class for OUT bounded (w)string argument.
   *
   */
  template<typename S_var,
           size_t BOUND,
           template <typename> class Insert_Policy>
  class Out_BD_String_Argument_T : public OutArgument
  {
  public:
    Out_BD_String_Argument_T (typename S_var::s_traits::string_out x);

    virtual CORBA::Boolean demarshal (TAO_InputCDR &);
#if TAO_HAS_INTERCEPTORS == 1
    virtual void interceptor_value (CORBA::Any *any) const;
#endif /* TAO_HAS_INTERCEPTORS == 1 */
    typename S_var::s_traits::char_type *& arg ();

  private:
    typename S_var::s_traits::char_type *& x_;
  };

  /**
   * @class Ret_BD_String_Argument_T
   *
   * @brief Template class for return stub value of bd (w)string argument.
   *
   */
  template<typename S_var,
           size_t BOUND,
           template <typename> class Insert_Policy>
  class Ret_BD_String_Argument_T : public RetArgument
  {
  public:
    Ret_BD_String_Argument_T ();

    virtual CORBA::Boolean demarshal (TAO_InputCDR &);
#if TAO_HAS_INTERCEPTORS == 1
    virtual void interceptor_value (CORBA::Any *any) const;
#endif /* TAO_HAS_INTERCEPTORS == 1 */
    typename S_var::s_traits::char_type *& arg ();

    typename S_var::s_traits::char_type * excp ();
    typename S_var::s_traits::char_type * retn ();

  private:
    S_var x_;
  };

  /**
   * @struct BD_String_Arg_Traits_T
   *
   * @brief Template class for stub argument traits of bounded (w)strings.
   *
   */
  template<typename T_var,
           size_t BOUND,
           template <typename> class Insert_Policy>
  struct BD_String_Arg_Traits_T
  {
    using ret_type = typename T_var::s_traits::char_type *;
    using in_type = typename T_var::s_traits::char_type const *;
    using inout_type = typename T_var::s_traits::char_type *&;
    using out_type = typename T_var::s_traits::string_out;

    using in_arg_val = In_BD_String_Argument_T<T_var, BOUND, Insert_Policy>;
    using in_clonable_arg_val = In_BD_String_Clonable_Argument_T<T_var, BOUND, Insert_Policy>;
    using inout_arg_val = Inout_BD_String_Argument_T<T_var, BOUND, Insert_Policy>;
    using out_arg_val = Out_BD_String_Argument_T<T_var, BOUND, Insert_Policy>;
    using ret_val = Ret_BD_String_Argument_T<T_var, BOUND, Insert_Policy>;
  };
}

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "tao/BD_String_Argument_T.inl"
#endif /* __ACE_INLINE__ */

#include "tao/BD_String_Argument_T.cpp"

#include /**/ "ace/post.h"

#endif /* TAO_BD_STRING_ARGUMENT_T_H */
