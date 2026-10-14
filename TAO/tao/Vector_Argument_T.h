// -*- C++ -*-

//=============================================================================
/**
 *  @file    Vector_Argument_T.h
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
    S const & arg () const;

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
    virtual ~In_Vector_Clonable_Argument_T ();

    virtual Argument* clone ();

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
    S & arg ();

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
    S & arg ();

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
    Ret_Vector_Argument_T ();

    virtual CORBA::Boolean demarshal (TAO_InputCDR &);
#if TAO_HAS_INTERCEPTORS == 1
    virtual void interceptor_value (CORBA::Any *any) const;
#endif /* TAO_HAS_INTERCEPTORS == 1 */
    S & arg ();

    S excp ();
    S retn ();

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
    using ret_type = T;
    using in_type = T const &;
    using inout_type = T &;
    using out_type = T &;

    using in_arg_val = In_Vector_Argument_T<T, Insert_Policy>;
    using in_clonable_arg_val = In_Vector_Clonable_Argument_T<T, Insert_Policy>;
    using inout_arg_val = Inout_Vector_Argument_T<T, Insert_Policy>;
    using out_arg_val = Out_Vector_Argument_T<T, Insert_Policy>;
    using ret_val = Ret_Vector_Argument_T<T, Insert_Policy>;
  };
}

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "tao/Vector_Argument_T.inl"
#endif /* __ACE_INLINE__ */

#include "tao/Vector_Argument_T.cpp"

#include /**/ "ace/post.h"

#endif /* TAO_VECTOR_ARGUMENT_T_H */
