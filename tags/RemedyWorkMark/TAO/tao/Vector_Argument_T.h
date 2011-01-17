// -*- C++ -*-

//=============================================================================
/**
 *  @file    Vector_Argument_T.h
 *
 *  $Id$
 *
 *  @author Jeff Parsons
 */
//=============================================================================

#ifndef TAO_VECTOR_ARGUMENT_T_H
#define TAO_VECTOR_ARGUMENT_T_H

#include /**/ "ace/pre.h"

#include "tao/Argument.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  /**
   * @class In_Vector_Argument_T
   *
   * @brief Template class for IN stub argument of
   * unbounded sequence IDL types.
   *
   */
  template<typename S,
           template <typename> class Insert_Policy>
  class In_Vector_Argument_T : public InArgument
  {
  public:
    In_Vector_Argument_T (S const & x);

    virtual CORBA::Boolean marshal (TAO_OutputCDR &cdr);
#if TAO_HAS_INTERCEPTORS == 1
    virtual void interceptor_value (CORBA::Any *any) const;
#endif /* TAO_HAS_INTERCEPTORS == 1 */
    S const & arg (void) const;

  protected:
    S const & x_;
  };

  /**
   * @class In_Vector_Clonable_Argument_T
   *
   * @brief Template class for IN stub argument of
   * unbounded sequence IDL types.
   *
   */
  template<typename S,
           template <typename> class Insert_Policy>
  class In_Vector_Clonable_Argument_T :
          public In_Vector_Argument_T<S, Insert_Policy>
  {
  public:
    In_Vector_Clonable_Argument_T (S const & x);
    virtual ~In_Vector_Clonable_Argument_T (void);

    virtual Argument* clone (void);

  private:
    bool is_clone_;
  };

  /**
   * @class Inout_Vector_Argument_T
   *
   * @brief Template class for INOUT stub argument of
   * unbounded sequence IDL types.
   *
   */
  template<typename S,
           template <typename> class Insert_Policy>
  class Inout_Vector_Argument_T : public InoutArgument
  {
  public:
    Inout_Vector_Argument_T (S & x);

    virtual CORBA::Boolean marshal (TAO_OutputCDR &cdr);
    virtual CORBA::Boolean demarshal (TAO_InputCDR &);
#if TAO_HAS_INTERCEPTORS == 1
    virtual void interceptor_value (CORBA::Any *any) const;
#endif /* TAO_HAS_INTERCEPTORS == 1 */
    S & arg (void);

  private:
    S & x_;
  };

  /**
   * @class Out_Vector_Argument_T
   *
   * @brief Template class for OUT stub argument of
   * unbounded sequence IDL types.
   *
   */
  template<typename S,
           template <typename> class Insert_Policy>
  class Out_Vector_Argument_T : public OutArgument
  {
  public:
    Out_Vector_Argument_T (S & x);

    virtual CORBA::Boolean demarshal (TAO_InputCDR &);
#if TAO_HAS_INTERCEPTORS == 1
    virtual void interceptor_value (CORBA::Any *any) const;
#endif /* TAO_HAS_INTERCEPTORS == 1 */
    S & arg (void);

  private:
    S & x_;
  };

  /**
   * @class Ret_Vector_Argument_T
   *
   * @brief Template class for return stub value of fixed size IDL types.
   *
   */
  template<typename S,
           template <typename> class Insert_Policy>
  class Ret_Vector_Argument_T : public RetArgument
  {
  public:
    Ret_Vector_Argument_T (void);

    virtual CORBA::Boolean demarshal (TAO_InputCDR &);
#if TAO_HAS_INTERCEPTORS == 1
    virtual void interceptor_value (CORBA::Any *any) const;
#endif /* TAO_HAS_INTERCEPTORS == 1 */
    S & arg (void);

    S excp (void);
    S retn (void);

  protected:
    S x_;
  };

  /**
   * @struct Vector_Arg_Traits_T
   *
   * @brief Template class for stub argument traits of
   *  sequence IDL types, when the STL mapping is used by
   *  the IDL compiler. The ret_type and out_type typedefs
   *  are different from Vector_Arg_Traits_T.
   *
   */
  template<typename T,
           template <typename> class Insert_Policy>
  struct Vector_Arg_Traits_T
  {
    typedef T                                           ret_type;
    typedef T const &                                   in_type;
    typedef T &                                         inout_type;
    typedef T &                                         out_type;

    typedef In_Vector_Argument_T<T, Insert_Policy>    in_arg_val;
    typedef In_Vector_Clonable_Argument_T<T, Insert_Policy>
                                                        in_clonable_arg_val;
    typedef Inout_Vector_Argument_T<T, Insert_Policy> inout_arg_val;
    typedef Out_Vector_Argument_T<T, Insert_Policy>   out_arg_val;
    typedef Ret_Vector_Argument_T<T, Insert_Policy>   ret_val;
  };
}

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "tao/Vector_Argument_T.inl"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "tao/Vector_Argument_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Vector_Argument_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include /**/ "ace/post.h"

#endif /* TAO_VECTOR_ARGUMENT_T_H */
