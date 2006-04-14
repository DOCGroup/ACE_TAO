// -*- C++ -*-

//=============================================================================
/**
 *  @file get_arg.h
 *
 *  $Id$
 *
 *  Thru-POA/skeleton argument selection function templates.
 *  @par
 *  These function templates are used to choose between arguments
 *  supplied in thru-POA collocated invocations and arguments in
 *  uncollocated invocations.
 *
 *  @note A function template approach is used to avoid complicated
 *        argument conversions between stub supplied arguments and
 *        skeleton supplied ones.  For example, it is possible to make
 *        argument selection more transparent by taking advantage of
 *        run-time polymorphism.  However, that approach would incur
 *        additional footprint overhead on the client side due to the
 *        introduction of virtual tables, etc.
 *
 *  @author  Ossama Othman
 */
//=============================================================================

#ifndef TAO_GET_ARG_H
#define TAO_GET_ARG_H

#include /**/ "ace/pre.h"

#include "tao/operation_details.h"


namespace TAO
{
  namespace Portable_Server
  {

    /// Get return value/argument.
    /**
     * @note It is possible to determine the return argument type
     *       using typedef traits found in the TAO::SArg_Traits<>
     *       template.  However, MSVC++ 6 does not support them
     *       properly.  Until we drop MSVC++ 6 support, we'll have to
     *       pass the return argument type in an additional template
     *       parameter.
     *
     * @todo Drop second template parameter, and replace
     *       " @c RET_ARG_TYPE " return type with
     *       " @c typename TAO::SArg_Traits<T>::ret_arg_type ".
     */
    template<typename T, typename RET_ARG_TYPE>
    RET_ARG_TYPE
    get_ret_arg (TAO_Operation_Details const * details,
                 TAO::Argument * const * skel_args)
    {
      return
        details
        ? static_cast<typename TAO::Arg_Traits<T>::ret_val *> (
            details->args ()[0])->arg ()
        : static_cast<typename TAO::SArg_Traits<T>::ret_val *> (
            skel_args[0])->arg ();
    }

    /// Get "in" argument.
    /**
     * @see @c get_ret_arg note to understand why we currently use a
     *      second template parameter.
     *
     * @todo Drop second template parameter, and replace
     *       " @c IN_ARG_TYPE " return type with
     *       " @c typename TAO::SArg_Traits<T>::in_arg_type ".
     */
    template<typename T, typename IN_ARG_TYPE>
    IN_ARG_TYPE
    get_in_arg (TAO_Operation_Details const * details,
                TAO::Argument * const * skel_args,
                size_t i)
    {
      return
        details
        ? static_cast<typename TAO::Arg_Traits<T>::in_arg_val *> (
            details->args ()[i])->arg ()
        : static_cast<typename TAO::SArg_Traits<T>::in_arg_val *> (
            skel_args[i])->arg ();
    }

    /// Get "inout" argument.
    /**
     * @see @c get_ret_arg note to understand why we currently use a
     *      second template parameter.
     *
     * @todo Drop second template parameter, and replace
     *       " @c INOUT_ARG_TYPE " return type with
     *       " @c typename TAO::SArg_Traits<T>::inout_arg_type ".
     */
    template<typename T, typename INOUT_ARG_TYPE>
    INOUT_ARG_TYPE
    get_inout_arg (TAO_Operation_Details const * details,
                   TAO::Argument * const * skel_args,
                   size_t i)
    {
      return
        details
        ? static_cast<typename TAO::Arg_Traits<T>::inout_arg_val *> (
            details->args ()[i])->arg ()
        : static_cast<typename TAO::SArg_Traits<T>::inout_arg_val *> (
            skel_args[i])->arg ();
    }

    /// Get "out" argument.
    /**
     * @see @c get_ret_arg note to understand why we currently use a
     *      second template parameter.
     *
     * @todo Drop second template parameter, and replace
     *       " @c OUT_ARG_TYPE " return type with
     *       " @c typename TAO::SArg_Traits<T>::out_arg_type ".
     */
    template<typename T, typename OUT_ARG_TYPE>
    OUT_ARG_TYPE
    get_out_arg (TAO_Operation_Details const * details,
                 TAO::Argument * const * skel_args,
                 size_t i)
    {
      return
        details
        ? static_cast<typename TAO::Arg_Traits<T>::out_arg_val *> (
            details->args ()[i])->arg ()
        : static_cast<typename TAO::SArg_Traits<T>::out_arg_val *> (
            skel_args[i])->arg ();
    }

  }
}

#include /**/ "ace/post.h"

#endif  /* TAO_GET_ARG_H*/
