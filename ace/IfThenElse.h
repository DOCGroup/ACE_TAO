// -*- C++ -*-

//=============================================================================
/**
 *  @file    IfThenElse.h
 *
 *  ACE::IfThenElse traits template based on the IfThenElse template
 *  described in the book "C++ Templates" by Vandevoorde and
 *  Josuttis.
 *
 *  $Id$
 *
 *  @author Ossama Othman <ossama@dre.vanderbilt.edu>
 */
//=============================================================================

#ifndef ACE_IF_THEN_ELSE_H
#define ACE_IF_THEN_ELSE_H

namespace ACE
{

  /**
   * @struct IfThenElse
   *
   * @brief Compile-time selection of type based on a boolean value.
   *
   * This primary template selects the second or third argument based
   * on the value of the boolean first argument.
   *
   * Usage example:
   *
   *   template <typename T>
   *   class Foo
   *   {
   *   public:
   *     // Set @c TheType to be the larger of @a T and @a int.
   *     typedef typename IfThenElse<(sizeof (T) > sizeof (int)),
   *                                 T,
   *                                 int>::result_type TheType;
   *   };
   *
   * You will of course need to
   * @note This merely a forward declaration since we really only care
   *       about the partial specializations below.
   */
  template <bool C, typename Ta, typename Tb>
  struct IfThenElse;

  /**
   * @struct IfThenElse
   *
   * @brief Select of type @a Ta if boolean value is @c true.
   *
   * This partial specialization selects the type @a Ta if the boolean
   * first argument is @c true.
   */
  template <typename Ta, typename Tb>
  struct IfThenElse<true, Ta, Tb>
  {
    typedef Ta result_type;
  };

  /**
   * @struct IfThenElse
   *
   * @brief Select of type @a Tb if boolean value is @c false.
   *
   * This partial specialization selects the type @a Tb if the boolean
   * first argument is @c false.
   */
  template <typename Ta, typename Tb>
  struct IfThenElse<false, Ta, Tb>
  {
    typedef Tb result_type;
  };

}

#endif  /* ACE_IF_THEN_ELSE_H */
