// -*- C++ -*-

//=============================================================================
/**
 *  @file Argument_T.h
 *
 *  $Id$
 *
 *  @author Ossama Othman
 */
//=============================================================================


#ifndef TAO_ARGUMENT_T_H
#define TAO_ARGUMENT_T_H

#include /**/ "ace/pre.h"

#include "tao/Argument.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */


namespace TAO
{
  /**
   * @class Const_Argument_T
   *
   * @brief Type-specific base class subclassed by stub and skeleton
   *        and argument class templates that have a @c const @c
   *        arg() method.
   *
   * The purpose of this class is to make it possible to
   * polymorphically retrieve an argument from either stub or skeleton
   * argument class template.  Both the corresponding stub and
   * skeleton argument class templates must of course have the same
   * template parameter type in order for the polymorphism to work.
   * Use of polymorphism in this manner allows us to avoid having to
   * convert between client and server side arguments, which can be
   * tedious, and ultimately allows the server side thru-POA
   * collocated and uncollocated code paths to share the same single
   * set of skeleton code.
   *
   * This abstract base class requires that subclasses define @c const
   * @c arg() method for the sake of const-correctness.  The template
   * parameter @c T corresponds to the type of argument passed to an
   * IDL-defined operation according to the C++ mapping.  For example,
   * given the following IDL:
   *
   * @code
   *   typedef sequence<short> ShortSeq;
   *
   *   interface Foo {
   *     void op (in ShortSeq s);
   *   };
   * @endcode
   *
   * the @c Const_Argument_T template parameter @c T would be:
   *
   * @code
   *   ShortSeq const &
   * @endcode
   *
   * since that would be parameter type for the corresponding C++
   * @c Foo::op() function.
   *
   * @note This class template is generally only suitable for IDL "@c in"
   *       parameters since they are read-only.
   *
   * @see Mutable_Argument_T
   */
  template <typename T>
  class Const_Argument_T : public Argument
  {
  public:

    virtual T arg (void) const = 0;

  };

  /**
   * @class Mutable_Argument_T
   *
   * @brief Type-specific base class subclassed by stub and skeleton
   *        and argument class templates that have a non- @c const @c
   *        arg() method.
   *
   * This class template serves the same purpose as the
   * @c Const_Argument_T class template except that it is suitable for
   * argument subclasses corresponding to mutable parameters (e.g. IDL
   * "@c inout", "@c out" and return values.  It is not meant for use
   * with IDL "@c in" parameters.
   *
   * @see Const_Argument_T
   */
  template <typename T>
  class Mutable_Argument_T : public Argument
  {
  public:

    virtual T arg (void) = 0;

  };

}

#include /**/ "ace/post.h"

#endif /* TAO_ARGUMENT_T_H */
