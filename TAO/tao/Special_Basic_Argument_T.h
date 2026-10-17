// -*- C++ -*-

//=============================================================================
/**
 *  @file    Special_Basic_Argument_T.h
 *
 *  @authors Jeff Parsons and Carlos O'Ryan
 */
//=============================================================================


#ifndef TAO_SPECIAL_BASIC_ARGUMENT_T_H
#define TAO_SPECIAL_BASIC_ARGUMENT_T_H

#include /**/ "ace/pre.h"

#include "tao/Argument.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  /**
   * @class In_Special_Basic_Argument_T
   *
   * @brief Template class for IN stub argument of (w)char/boolean/octet.
   *
   */
  template<typename S,
           typename to_S,
           typename from_S,
           template <typename> class Insert_Policy>
  class In_Special_Basic_Argument_T : public InArgument
  {
  public:
    In_Special_Basic_Argument_T (S const &);

    CORBA::Boolean marshal (TAO_OutputCDR &cdr) override;
#if TAO_HAS_INTERCEPTORS == 1
    void interceptor_value (CORBA::Any *any) const override;
#endif /* TAO_HAS_INTERCEPTORS == 1 */
    S const & arg () const;

  protected:
    S const & x_;
  };

  /**
   * @class In_Special_Basic_Clonable_Argument_T
   *
   * @brief Template class for IN stub argument of (w)char/boolean/octet.
   */
  template<typename S,
           typename to_S,
           typename from_S,
           template <typename> class Insert_Policy>
  class In_Special_Basic_Clonable_Argument_T :
        public In_Special_Basic_Argument_T<S, to_S, from_S, Insert_Policy>
  {
  public:
    In_Special_Basic_Clonable_Argument_T (S const &);
    ~In_Special_Basic_Clonable_Argument_T () override;

    Argument* clone () override;

  private:
    bool is_clone_ {};
  };

  /**
   * @class Inout_Special_Basic_Argument_T
   *
   * @brief Template class for INOUT stub argument of (w)char/boolean/octet.
   */
  template<typename S,
           typename to_S,
           typename from_S,
           template <typename> class Insert_Policy>
  class Inout_Special_Basic_Argument_T : public InoutArgument
  {
  public:
    Inout_Special_Basic_Argument_T (S & x);

    CORBA::Boolean marshal (TAO_OutputCDR &cdr) override;
    CORBA::Boolean demarshal (TAO_InputCDR &) override;
#if TAO_HAS_INTERCEPTORS == 1
    void interceptor_value (CORBA::Any *any) const override;
#endif /* TAO_HAS_INTERCEPTORS == 1 */
    S & arg ();

  private:
    S & x_;
  };

  /**
   * @class Out_Special_Basic_Argument_T
   *
   * @brief Template class for OUT stub argument of (w)char/boolean/octet.
   */
  template<typename S,
           typename to_S,
           typename from_S,
           template <typename> class Insert_Policy>
  class Out_Special_Basic_Argument_T : public OutArgument
  {
  public:
    Out_Special_Basic_Argument_T (S & x);

    CORBA::Boolean demarshal (TAO_InputCDR &) override;
#if TAO_HAS_INTERCEPTORS == 1
    void interceptor_value (CORBA::Any *any) const override;
#endif /* TAO_HAS_INTERCEPTORS == 1 */
    S & arg ();

  private:
    S & x_;
  };

  /**
   * @class Ret_Basic_Argument_T
   *
   * @brief Template class for return stub value of (w)char/boolean/octet.
   */
  template<typename S,
           typename to_S,
           typename from_S,
           template <typename> class Insert_Policy>
  class Ret_Special_Basic_Argument_T : public RetArgument
  {
  public:
    Ret_Special_Basic_Argument_T ();

    CORBA::Boolean demarshal (TAO_InputCDR &) override;
#if TAO_HAS_INTERCEPTORS == 1
    void interceptor_value (CORBA::Any *any) const override;
#endif /* TAO_HAS_INTERCEPTORS == 1 */
    S & arg ();

    S excp ();
    S retn ();

  private:
    S x_ {};
  };

  /**
   * @struct Special_Basic_Tag
   *
   * @brief Struct for basic IDL type arguments id tag.
   *
   */
  struct TAO_Export Special_Basic_Tag {};

  /**
   * @struct Special_Basic_Arg_Traits_T
   *
   * @brief Template class for stub argument traits of (w)char/boolean/octet.
   */
  template<typename T,
           typename to_T,
           typename from_T,
           template <typename> class Insert_Policy>
  struct Special_Basic_Arg_Traits_T
  {
    using ret_type = T;
    using in_type = T;
    using inout_type = T &;
    using out_type = T &;

    using in_arg_val = In_Special_Basic_Argument_T<T, to_T, from_T, Insert_Policy>;
    using in_clonable_arg_val = In_Special_Basic_Clonable_Argument_T<T, to_T, from_T, Insert_Policy>;
    using inout_arg_val = Inout_Special_Basic_Argument_T<T, to_T, from_T, Insert_Policy>;
    using out_arg_val = Out_Special_Basic_Argument_T<T, to_T, from_T, Insert_Policy>;
    using ret_val = Ret_Special_Basic_Argument_T<T, to_T, from_T, Insert_Policy>;

    using idl_tag = Special_Basic_Tag;
  };
}

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "tao/Special_Basic_Argument_T.inl"
#endif /* __ACE_INLINE__ */

#include "tao/Special_Basic_Argument_T.cpp"

#include /**/ "ace/post.h"

#endif /* TAO_SPECIAL_BASIC_ARGUMENT_T_H */
