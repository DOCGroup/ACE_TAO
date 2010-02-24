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

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace Portable_Server
  {
    /// Get return value/argument.
    template<typename T>
    typename TAO::SArg_Traits<T>::ret_arg_type
    get_ret_arg (TAO_Operation_Details const * details,
                 TAO::Argument * const * skel_args)
    {
      return
        (details != 0 && details->use_stub_args ())
        ? static_cast<typename TAO::Arg_Traits<T>::ret_val *> (
            details->args ()[0])->arg ()
        : static_cast<typename TAO::SArg_Traits<T>::ret_val *> (
            skel_args[0])->arg ();
    }

    /// Get "in" argument.
    template<typename T>
    typename TAO::SArg_Traits<T>::in_arg_type
    get_in_arg (TAO_Operation_Details const * details,
                TAO::Argument * const * skel_args,
                size_t i)
    {
      return
        (details != 0 && details->use_stub_args ())
        ? static_cast<typename TAO::Arg_Traits<T>::in_arg_val *> (
            details->args ()[i])->arg ()
        : static_cast<typename TAO::SArg_Traits<T>::in_arg_val *> (
            skel_args[i])->arg ();
    }

    /// Get "inout" argument.
    template<typename T>
    typename TAO::SArg_Traits<T>::inout_arg_type
    get_inout_arg (TAO_Operation_Details const * details,
                   TAO::Argument * const * skel_args,
                   size_t i)
    {
      return
        (details != 0 && details->use_stub_args ())
        ? static_cast<typename TAO::Arg_Traits<T>::inout_arg_val *> (
            details->args ()[i])->arg ()
        : static_cast<typename TAO::SArg_Traits<T>::inout_arg_val *> (
            skel_args[i])->arg ();
    }

    /// Get "out" argument.
    template<typename T>
    typename TAO::SArg_Traits<T>::out_arg_type
    get_out_arg (TAO_Operation_Details const * details,
                 TAO::Argument * const * skel_args,
                 size_t i)
    {
      return
        (details != 0 && details->use_stub_args ())
        ? static_cast<typename TAO::Arg_Traits<T>::out_arg_val *> (
            details->args ()[i])->arg ()
        : static_cast<typename TAO::SArg_Traits<T>::out_arg_val *> (
            skel_args[i])->arg ();
    }
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif  /* TAO_GET_ARG_H*/
