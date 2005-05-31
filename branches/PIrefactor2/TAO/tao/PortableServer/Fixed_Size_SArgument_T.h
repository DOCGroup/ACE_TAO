// -*- C++ -*-

//=============================================================================
/**
 *  @file    Fixed_Size_SArgument_T.h
 *
 *  $Id$
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
  template<typename S>
  class In_Fixed_Size_SArgument_T : public Argument
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
    virtual CORBA::Boolean demarshal (TAO_InputCDR & cdr);

#if TAO_HAS_INTERCEPTORS == 1
    virtual void interceptor_param (Dynamic::Parameter & p);
#endif /* TAO_HAS_INTERCEPTORS == 1 */
    //@}

    /// Retrieve underlying argument.
    S const & arg (void) const;

  private:

    /// Reference to the "IN" argument.
    S x_;

  };

  // ------------------------------------------------------------

  /**
   * @class Inout_Fixed_Size_SArgument_T
   *
   * @brief Template class for INOUT skeleton arg of fixed size IDL types.
   *
   */
  template<typename S>
  class Inout_Fixed_Size_SArgument_T : public Argument
  {
  public:

    /// Constructor.
    Inout_Fixed_Size_SArgument_T (void);

    /**
     * @name @c TAO::Argument Method Overrides
     *
     * @c TAO::Argument method overrides specific to the type of
     * operation argument represented by this class.
     *
     * @see @c TAO::Argument.
     */
    //@{
    virtual CORBA::Boolean marshal (TAO_OutputCDR &);
    virtual CORBA::Boolean demarshal (TAO_InputCDR &);
#if TAO_HAS_INTERCEPTORS == 1
    virtual void interceptor_param (Dynamic::Parameter &);
#endif /* TAO_HAS_INTERCEPTORS == 1 */
    //@}

    S & arg (void);

  private:

    S x_;
  };

  // ------------------------------------------------------------

  /**
   * @class Out_Fixed_Size_SArgument_T
   *
   * @brief Template class for OUT skeleton argument of fixed size IDL types.
   *
   */
  template<typename S>
  class Out_Fixed_Size_SArgument_T : public Argument
  {
  public:

    /// Constructor.
    Out_Fixed_Size_SArgument_T (void);

    /**
     * @name @c TAO::Argument Method Overrides
     *
     * @c TAO::Argument method overrides specific to the type of
     * operation argument represented by this class.
     *
     * @see @c TAO::Argument.
     */
    //@{
    virtual CORBA::Boolean marshal (TAO_OutputCDR &);
#if TAO_HAS_INTERCEPTORS == 1
    virtual void interceptor_param (Dynamic::Parameter &);
#endif /* TAO_HAS_INTERCEPTORS == 1 */
    //@}

    S & arg (void);

  private:

    S x_;

  };

  // ------------------------------------------------------------

  /**
   * @class Ret_Fixed_Size_SArgument_T
   *
   * @brief Template class for return skeleton value of fixed size IDL types.
   *
   */
  template<typename S>
  class Ret_Fixed_Size_SArgument_T : public Argument
  {
  public:

    /// Constructor.
    Ret_Fixed_Size_SArgument_T (void);

    /**
     * @name @c TAO::Argument Method Overrides
     *
     * @c TAO::Argument method overrides specific to the type of
     * operation argument represented by this class.
     *
     * @see @c TAO::Argument.
     */
    //@{
    virtual CORBA::Boolean marshal (TAO_OutputCDR &);
#if TAO_HAS_INTERCEPTORS == 1
    virtual void interceptor_result (CORBA::Any *);
#endif /* TAO_HAS_INTERCEPTORS == 1 */
    //@}

    S & arg (void);

  private:

    S x_;

  };

  // ------------------------------------------------------------

  /**
   * @struct Fixed_Size_SArg_Traits_T
   *
   * @brief Template class for argument traits of fixed size IDL types.
   *
   */
  template<typename T>
  struct Fixed_Size_SArg_Traits_T
  {
    typedef T                                 ret_type;
    typedef T const &                         in_type;
    typedef T &                               inout_type;
    typedef T &                               out_type;

    typedef In_Fixed_Size_SArgument_T<T>      in_arg_val;
    typedef Inout_Fixed_Size_SArgument_T<T>   inout_arg_val;
    typedef Out_Fixed_Size_SArgument_T<T>     out_arg_val;
    typedef Ret_Fixed_Size_SArgument_T<T>     ret_val;

    // Typedefs corresponding to return value of arg() method in both
    // the client and server side argument class templates.
    typedef in_type                           in_arg_type;
    typedef inout_type                        inout_arg_type;
    typedef out_type                          out_arg_type;
    typedef out_type                          ret_arg_type;

  };

}


#if defined (__ACE_INLINE__)
#include "tao/PortableServer/Fixed_Size_SArgument_T.inl"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "tao/PortableServer/Fixed_Size_SArgument_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Fixed_Size_SArgument_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include /**/ "ace/post.h"

#endif /* TAO_FIXED_SIZE_SARGUMENT_T_H */
