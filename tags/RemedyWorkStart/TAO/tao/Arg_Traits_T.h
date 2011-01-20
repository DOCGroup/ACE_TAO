// This may look like C, but it's really -*- C++ -*-

//=============================================================================
/**
 *  @file    Arg_Traits_T.h
 *
 *  $Id$
 *
 *  @authors Jeff Parsons and Carlos O'Ryan
 */
//=============================================================================

#ifndef TAO_ARG_TRAITS_T_H
#define TAO_ARG_TRAITS_T_H

#include /**/ "ace/pre.h"

#include /**/ "tao/Versioned_Namespace.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  /**
   *
   * @brief Base class for all stub arg traits specializations.
   *
   */
  template<typename T>
  class Arg_Traits
  {
  };
}

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* TAO_ARG_TRAITS_T_H */
