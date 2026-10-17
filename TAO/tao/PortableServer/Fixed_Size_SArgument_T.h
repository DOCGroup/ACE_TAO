// -*- C++ -*-

//=============================================================================
/**
 *  @file    Fixed_Size_SArgument_T.h
 *
 *  @author Jeff Parsons
 *  @author Carlos O'Ryan
 *  @author Ossama Othman
 */
//=============================================================================

#ifndef TAO_FIXED_SIZE_SARGUMENT_T_H
#define TAO_FIXED_SIZE_SARGUMENT_T_H

#include /**/ "ace/pre.h"

#include "tao/Argument.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  /**
   * @class In_Fixed_Size_SArgument_T
   *
   * @brief Skeleton class template for operation "IN" arguments of
   *        fixed size IDL types.
   *
   * Skeleton class template for operation "IN" arguments of fixed
   * size IDL types.
   */
  template<typename S, template <typename> class Insert_Policy>
  class In_Fixed_Size_SArgument_T : public InArgument
  {
  public:
    /**
     * @name @c TAO::Argument Method Overrides
     *
     * @c TAO::Argument method overrides specific to the type of
     * operation argument represented by this class.
     *
     * @see @c TAO::Argument.
     */
    //@{
    CORBA::Boolean demarshal (TAO_InputCDR &cdr) override;

#if TAO_HAS_INTERCEPTORS == 1
    void interceptor_value (CORBA::Any *any) const override;
#endif /* TAO_HAS_INTERCEPTORS == 1 */
    //@}

    /// Retrieve underlying argument.
    S const & arg () const;

  private:
    /// Reference to the "IN" argument.
    S x_ {};
  };

  // ------------------------------------------------------------

  /**
   * @class Inout_Fixed_Size_SArgument_T
   *
   * @brief Template class for INOUT skeleton arg of fixed size IDL types.
   *
   */
  template<typename S, template <typename> class Insert_Policy>
  class Inout_Fixed_Size_SArgument_T : public InoutArgument
  {
  public:
    /// Constructor.
    Inout_Fixed_Size_SArgument_T ();

    /**
     * @name @c TAO::Argument Method Overrides
     *
     * @c TAO::Argument method overrides specific to the type of
     * operation argument represented by this class.
     *
     * @see @c TAO::Argument.
     */
    //@{
    CORBA::Boolean marshal (TAO_OutputCDR &cdr) override;
    CORBA::Boolean demarshal (TAO_InputCDR &) override;
#if TAO_HAS_INTERCEPTORS == 1
    void interceptor_value (CORBA::Any *any) const override;
#endif /* TAO_HAS_INTERCEPTORS == 1 */
    //@}

    S & arg ();

  private:
    S x_ {};
  };

  // ------------------------------------------------------------

  /**
   * @class Out_Fixed_Size_SArgument_T
   *
   * @brief Template class for OUT skeleton argument of fixed size IDL types.
   *
   */
  template<typename S, template <typename> class Insert_Policy>
  class Out_Fixed_Size_SArgument_T : public OutArgument
  {
  public:
    /// Constructor.
    Out_Fixed_Size_SArgument_T ();

    /**
     * @name @c TAO::Argument Method Overrides
     *
     * @c TAO::Argument method overrides specific to the type of
     * operation argument represented by this class.
     *
     * @see @c TAO::Argument.
     */
    //@{
    CORBA::Boolean marshal (TAO_OutputCDR &cdr) override;
#if TAO_HAS_INTERCEPTORS == 1
    void interceptor_value (CORBA::Any *any) const override;
#endif /* TAO_HAS_INTERCEPTORS == 1 */
    //@}

    S & arg ();

  private:
    S x_ {};
  };

  // ------------------------------------------------------------

  /**
   * @class Ret_Fixed_Size_SArgument_T
   *
   * @brief Template class for return skeleton value of fixed size IDL types.
   *
   */
  template<typename S, template <typename> class Insert_Policy>
  class Ret_Fixed_Size_SArgument_T : public RetArgument
  {
  public:
    /// Constructor.
    Ret_Fixed_Size_SArgument_T ();

    /**
     * @name @c TAO::Argument Method Overrides
     *
     * @c TAO::Argument method overrides specific to the type of
     * operation argument represented by this class.
     *
     * @see @c TAO::Argument.
     */
    //@{
    CORBA::Boolean marshal (TAO_OutputCDR &cdr) override;
#if TAO_HAS_INTERCEPTORS == 1
    void interceptor_value (CORBA::Any *any) const override;
#endif /* TAO_HAS_INTERCEPTORS == 1 */
    //@}

    S & arg ();

  private:
    S x_ {};
  };

  // ------------------------------------------------------------

  /**
   * @struct Fixed_Size_SArg_Traits_T
   *
   * @brief Template class for argument traits of fixed size IDL types.
   *
   */
  template<typename T,
           template <typename> class Insert_Policy>
  struct Fixed_Size_SArg_Traits_T
  {
    using ret_type = T;
    using in_type = T const &;
    using inout_type = T &;
    using out_type = T &;

    using in_arg_val = In_Fixed_Size_SArgument_T<T, Insert_Policy>;
    using inout_arg_val = Inout_Fixed_Size_SArgument_T<T, Insert_Policy>;
    using out_arg_val = Out_Fixed_Size_SArgument_T<T, Insert_Policy>;
    using ret_val = Ret_Fixed_Size_SArgument_T<T, Insert_Policy>;

    // Typedefs corresponding to return value of arg() method in both
    // the client and server side argument class templates.
    using in_arg_type = in_type;
    using inout_arg_type = inout_type;
    using out_arg_type = out_type;
    using ret_arg_type = out_type;
  };
}

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "tao/PortableServer/Fixed_Size_SArgument_T.inl"
#endif /* __ACE_INLINE__ */

#include "tao/PortableServer/Fixed_Size_SArgument_T.cpp"

#include /**/ "ace/post.h"

#endif /* TAO_FIXED_SIZE_SARGUMENT_T_H */
